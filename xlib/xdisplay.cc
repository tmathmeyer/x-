
#include "xdisplay.h"
#include "xpixmap.h"
#include "xwindow.h"

#include "base/check.h"

namespace xpp::xlib {

Traits<XWindow>::XppType Traits<XWindow>::Import(
    const XorgType& window,
    std::shared_ptr<XDisplay> display,
    bool destroy_on_delete) {
  return std::shared_ptr<XWindow>(
      new XWindow(std::move(display), window, destroy_on_delete));
}

Traits<XPixmap>::XppType Traits<XPixmap>::Import(
    const XorgType& pixmap,
    std::shared_ptr<XDisplay> display) {
  return std::shared_ptr<XPixmap>(new XPixmap(std::move(display), pixmap));
}

XDisplay::~XDisplay() {
  XCloseDisplay(display_);
}

XDisplay::XDisplay(const char* id) {
  display_ = XOpenDisplay(id);
  MCHECK(display_, "Could not open display\n");
}

std::shared_ptr<XDisplay> XDisplay::Create(const char* id) {
  std::shared_ptr<XDisplay> ptr(new XDisplay(id));
  return ptr;
}

// static
std::map<std::string, gfx::Rect> XDisplay::GetMonitorSizes() {
  std::map<std::string, gfx::Rect> result;
  auto display = XDisplay::Create();
  auto root = display->XRootWindow(display->XDefaultScreen());

  XRRScreenResources* xrr_resources = root->XRRGetScreenResourcesCurrent();
  if (!xrr_resources || xrr_resources->noutput <= 0) {
    XRRFreeScreenResources(xrr_resources);
    return result;
  }

  for (int i = 0; i < xrr_resources->ncrtc; i++) {
    XRROutputInfo* xrr_output =
        display->XRRGetOutputInfo(xrr_resources, xrr_resources->outputs[i]);

    if (!xrr_output) {
      XRRFreeScreenResources(xrr_resources);
      return result;
    }

    if (!xrr_output->mm_height) {
      XRRFreeOutputInfo(xrr_output);
      XRRFreeScreenResources(xrr_resources);
      return result;
    }

    XRRCrtcInfo* xrr_crtc =
        display->XRRGetCrtcInfo(xrr_resources, xrr_output->crtc);

    if (!xrr_crtc) {
      XRRFreeOutputInfo(xrr_output);
      XRRFreeScreenResources(xrr_resources);
      return result;
    }

    result.insert({xrr_output->name, {xrr_crtc->width, xrr_crtc->height}});

    XRRFreeCrtcInfo(xrr_crtc);
    XRRFreeOutputInfo(xrr_output);
  }

  XRRFreeScreenResources(xrr_resources);
  return result;
}

}  // namespace xpp::xlib
