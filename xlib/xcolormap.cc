
#include "xcolormap.h"

namespace xpp::xlib {

XColorMap::~XColorMap() = default;

XColorMap::XColorMap(::Colormap colormap,
                     std::shared_ptr<XWindow> window,
                     std::shared_ptr<XDisplay> display) {
  display_ = display;
  window_ = std::move(window);
  colormap_ = colormap;
}

}  // namespace xpp::xlib
