
#include "xgraphics.h"

namespace xpp::xlib {

XGraphics::~XGraphics() {
  display_->XFreeGC(graphics_);
}

XGraphics::XGraphics(::GC graphics,
                     std::shared_ptr<XDrawable> drawable,
                     std::shared_ptr<XDisplay> display,
                     std::shared_ptr<XColorMap> colormap) {
  display_ = std::move(display);
  drawable_ = std::move(drawable);
  colormap_ = std::move(colormap);
  graphics_ = graphics;
}

::GC XGraphics::operator*() {
  return graphics_;
}

}  // namespace xpp::xlib
