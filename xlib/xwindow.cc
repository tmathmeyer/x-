
#include "xwindow.h"

#include "base/check.h"
#include "xdisplay.h"
#include "xgraphics.h"

namespace xpp::xlib {

XWindow::~XWindow() {
  if (destroy_on_delete_)
    display_->XDestroyWindow(window_);
}

XWindow::XWindow(std::shared_ptr<XDisplay> display,
                 ::Window window,
                 bool destroy_on_delete)
    : XDrawable(std::move(display)) {
  window_ = window;
  destroy_on_delete_ = destroy_on_delete;
}

Traits<XColorMap>::XppType Traits<XColorMap>::Import(
    const XorgType& colormap,
    std::shared_ptr<XWindow> window,
    std::shared_ptr<XDisplay> display) {
  return std::shared_ptr<XColorMap>(
      new XColorMap(colormap, std::move(window), std::move(display)));
}

::Window XWindow::operator*() {
  return window_;
}

::Drawable XWindow::Drawable() {
  return window_;
}

}  // namespace xpp::xlib
