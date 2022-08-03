
#include "xpixmap.h"

#include "base/check.h"
#include "xdisplay.h"
#include "xgraphics.h"

namespace xpp::xlib {

XPixmap::XPixmap(std::shared_ptr<XDisplay> display, ::Pixmap pixmap)
    : XDrawable(std::move(display)), pixmap_(pixmap) {}

XPixmap::~XPixmap() {
  display_->XFreePixmap(pixmap_);
}

::Pixmap XPixmap::operator*() {
  return pixmap_;
}

::Drawable XPixmap::Drawable() {
  return pixmap_;
}

}  // namespace xpp::xlib
