#pragma once

#include <memory>

#include "xdisplay.h"
#include "xorg_typemap.h"

namespace xpp::xlib {

#define DISPLAY_METHOD(fn, ret)                                  \
  template <typename... Args>                                    \
  ret fn(Args&&... args) {                                       \
    return display_->fn(colormap_, std::forward<Args>(args)...); \
  }

class XColorMap {
 public:
  ~XColorMap();
  DISPLAY_METHOD(XAllocColor, void);

  ::Colormap colormap() { return colormap_; }

 private:
  std::shared_ptr<XWindow> window_;
  std::shared_ptr<XDisplay> display_;
  ::Colormap colormap_;

  // Private constructor. Must come from the display!
  XColorMap(::Colormap colormap,
            std::shared_ptr<XWindow> window,
            std::shared_ptr<XDisplay> display);

  // Allow XWindow to create XColorMap
  friend struct Traits<XColorMap>;
};

#undef DISPLAY_METHOD

}  // namespace xpp::xlib