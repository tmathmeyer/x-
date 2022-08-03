#include "xdrawable.h"

#include "base/check.h"
#include "xdisplay.h"
#include "xgraphics.h"

namespace xpp::xlib {

XDrawable::XDrawable(std::shared_ptr<XDisplay> display)
    : display_(std::move(display)) {}

Traits<XGraphics>::XppType Traits<XGraphics>::Import(
    const XorgType& graphics,
    std::shared_ptr<XDrawable> drawable,
    std::shared_ptr<XDisplay> display,
    std::shared_ptr<XColorMap> colormap) {
  return std::shared_ptr<XGraphics>(new XGraphics(
      graphics, std::move(drawable), std::move(display), std::move(colormap)));
}

}  // namespace xpp::xlib
