#include "window.h"

#include <thread>

namespace xpp::ui {

namespace {

gfx::Coord FindTopLeft(XWindow::PositionPin type,
                       gfx::Coord pin,
                       gfx::Rect size) {
  switch (type) {
    case XWindow::PositionPin::kTopLeft:
      return pin;
    case XWindow::PositionPin::kTopCenter:
      return {pin.x - size.width / 2, pin.y};
    case XWindow::PositionPin::kTopRight:
      return {pin.x - size.width, pin.y};
    case XWindow::PositionPin::kMiddleRight:
      return {pin.x - size.width, pin.y - size.height / 2};
    case XWindow::PositionPin::kBottomRight:
      return {pin.x - size.width, pin.y - size.height};
    case XWindow::PositionPin::kBottomCenter:
      return {pin.x - size.width / 2, pin.y - size.height};
    case XWindow::PositionPin::kBottomLeft:
      return {pin.x, pin.y - size.height};
    case XWindow::PositionPin::kMiddleLeft:
      return {pin.x, pin.y - size.height / 2};
    default:
      return pin;
  }
}

bool IsScrollButton(unsigned int button) {
  switch (button) {
    case Button4:
    case Button5:
    case 6:
    case 7:
      return true;
  }
  return false;
}

gfx::Coord GetMotionVector(unsigned int button) {
  switch (button) {
    case Button4:
      return {0, 1};
    case Button5:
      return {0, -1};
    case 6:
      return {1, 0};
    case 7:
      return {-1, 0};
  }
  NOTREACHED();
  return {0, 0};
}

}  // namespace

XWindow::XWindow() : XContainer() {
  display_ = xlib::XDisplay::Create();
  root_ = display_->XRootWindow(display_->XDefaultScreen());
}

XWindow::~XWindow() {}

std::unique_ptr<XWindow> XWindow::Create(WindowType type,
                                         PositionPin position,
                                         gfx::Rect size,
                                         gfx::Coord location) {
  XWindow* window = new XWindow();
  window->Initialize(type, position, size, location);
  return std::unique_ptr<XWindow>(window);
}

std::unique_ptr<XWindow> XWindow::Create() {
  return XWindow::Create(WindowType::kNormal, PositionPin::kTopLeft,
                         {1000, 1000}, {0, 0});
}

bool XWindow::Initialize(WindowType mode,
                         PositionPin positioning,
                         gfx::Rect size,
                         gfx::Coord loc) {
  position_pin_ = positioning;
  dimensions_ = exposed_to_ = size;
  preferred_position_ = loc;
  type_ = mode;
  laf_ = std::make_shared<LookAndFeel>();
  XVisualInfo vinfo;
  display_->XMatchVisualInfo(display_->XDefaultScreen(), 32, TrueColor, &vinfo);

  colormap_ = root_->XCreateColormap(vinfo.visual, AllocNone);

  XSetWindowAttributes attribs;
  attribs.colormap = colormap_->colormap();
  attribs.border_pixel = 0;
  attribs.background_pixel = 0;
  attribs.event_mask = 0;
  attribs.background_pixmap =
      display_->XCreatePixmapRaw(**root_, size.width, size.height, vinfo.depth);

  unsigned int mask =
      CWBackPixmap | CWColormap | CWEventMask | CWBorderPixel | CWBackPixel;

  if (mode == WindowType::kDesktopBackdrop ||
      mode == WindowType::kDesktopDock) {
    attribs.override_redirect = 1;
    mask |= CWOverrideRedirect;
  }

  loc = FindTopLeft(position_pin_, loc, size);
  window_ = root_->XCreateWindow(loc.x, loc.y, size.width, size.height,
                                 /*border_width=*/0, vinfo.depth, InputOutput,
                                 vinfo.visual, mask, &attribs);

  Atom property = display_->XInternAtom("_NET_WM_WINDOW_TYPE", False);
  Atom type;
  switch (mode) {
    case WindowType::kDesktopBackdrop: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);
      break;
    }
    case WindowType::kDesktopDock: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_DOCK", False);
      break;
    }
    case WindowType::kApplicationToolbar: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_TOOLBAR", False);
      break;
    }
    case WindowType::kApplicationMenu: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_MENU", False);
      break;
    }
    case WindowType::kUtilityWindow: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_UTILITY", False);
      break;
    }
    case WindowType::kSplashScreen: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_SPLASH", False);
      break;
    }
    case WindowType::kDialogPopup: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_DIALOG", False);
      break;
    }
    case WindowType::kNormal: {
      type = display_->XInternAtom("_NET_WM_WINDOW_TYPE_NORMAL", False);
      break;
    }
  }
  window_->XChangeProperty(property, xlib::XDisplay::kAtom, 32, PropModeReplace,
                           (unsigned char*)&type, 1);

  window_gc_ = window_->XCreateGC(colormap_);
  depth_ = vinfo.depth;
  return true;
}

void XWindow::SetVisible(bool visibility) {
  if (is_shown_ && visibility)
    return;

  if (is_shown_) {
    window_->XUnmapWindow();
    request_hide_flag_ = true;
  }

  if (visibility) {
    request_hide_flag_ = false;
    is_shown_ = true;
    window_->XMapRaised();
    auto thread = std::thread(&XWindow::RunEventLoop, this);
    if (thread.joinable())
      thread.join();
  }
}

void XWindow::Repaint() {
  puts("XW::Repaint");
  if (exposed_to_ != dimensions_) {
    dimensions_ = exposed_to_;
    SetDimensions(dimensions_);
    // TODO: do we want a back-buffer for shrink-resizes?
  }
  Graphics graphics(window_gc_, colormap_, laf_, {0, 0}, dimensions_);
  graphics.SetFont(
      laf_->AllocateFont(window_gc_, "default", "Fantasque Sans Mono", 10));
  Paint(&graphics);
}

void XWindow::RunEventLoop() {
  XEvent event;
  window_->XSelectInput(
      ExposureMask | VisibilityChangeMask | StructureNotifyMask |
      PropertyChangeMask | SubstructureNotifyMask | ButtonPressMask |
      ButtonReleaseMask | PointerMotionMask | EnterWindowMask |
      LeaveWindowMask | KeyPressMask | KeyReleaseMask);

  if (type_ == WindowType::kDesktopBackdrop ||
      type_ == WindowType::kDesktopDock) {
    SetDimensions(dimensions_); 
    Repaint();
  }

  while (!request_hide_flag_) {
    display_->XNextEvent(&event);
    if (request_hide_flag_)
      break;
    switch (event.type) {
      case EnterNotify: {
        gfx::Coord location = {event.xbutton.x, event.xbutton.y};
        previous_mouse_location_ = location;
        MouseMotionEvent e = {.location = location,
                              .previous_location = location,
                              .component = this};
        MouseEntered(e);
        break;
      }
      case LeaveNotify: {
        gfx::Coord location = {event.xbutton.x, event.xbutton.y};
        previous_mouse_location_ = location;
        MouseMotionEvent e = {.location = location,
                              .previous_location = location,
                              .component = this};
        MouseExited(e);
        break;
      }
      case MotionNotify: {
        gfx::Coord location = {event.xbutton.x, event.xbutton.y};
        MouseMotionEvent e = {.location = location,
                              .previous_location = previous_mouse_location_,
                              .component = this,
                              .mouse_button = mouse_button_};
        if (mouse_button_)
          MouseDragged(e);
        else
          MouseMoved(e);
        previous_mouse_location_ = location;
        break;
      }
      case ButtonPress: {
        // For some ungodly reason, scrolling is also a "click"
        press_location_ = {event.xbutton.x, event.xbutton.y};
        if (IsScrollButton(event.xbutton.button)) {
          MouseWheelEvent e = {.location = press_location_,
                               .vector = GetMotionVector(event.xbutton.button),
                               .component = this};
          WheelScrolled(e);
        } else {
          mouse_button_ = event.xbutton.button;
          MouseEvent e = {.location = press_location_,
                          .mouse_button = mouse_button_,
                          .component = this};
          MousePressed(e);
        }
        break;
      }
      case ButtonRelease: {
        gfx::Coord location = {event.xbutton.x, event.xbutton.y};
        if (!IsScrollButton(event.xbutton.button)) {
          mouse_button_ = event.xbutton.button;
          MouseEvent e = {.location = location,
                          .mouse_button = mouse_button_,
                          .component = this};
          MouseReleased(e);
          if (location == press_location_) {
            e.active = true;
            MouseClicked(e);
          }
          press_location_ = {0, 0};
          mouse_button_ = 0;
        }
        break;
      }
      case ConfigureNotify:
        exposed_to_ = {event.xconfigure.width, event.xconfigure.height};
        if (exposed_to_ != dimensions_)
          Repaint();
        break;
      case KeyPress:
        // puts("KeyPress");
        break;
      case KeyRelease:
        // puts("KeyRelease");
        break;
      case Expose:
        exposed_to_ = {event.xexpose.width, event.xexpose.height};
        if (exposed_to_ != dimensions_)
          Repaint();
        break;
      case PointerMotionMask:
        // puts("PMM");
        break;
      case NoExpose:
        // puts("NOEXP");
        break;
      case VisibilityNotify:
        // puts("VIS");
        break;
      case PropertyNotify:
        // puts("PROP");
        break;
      case MapNotify:
        Repaint();
        break;
      case UnmapNotify:
        // puts("Unmap");
        break;
      default:
        printf("Unhandled event ID: %i\n", event.type);
    }
  }
}

}  // namespace xpp::ui