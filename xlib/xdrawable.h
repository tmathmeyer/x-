#pragma once

#include <memory>

#include "xdisplay.h"
#include "xorg_typemap.h"

namespace xpp::xlib {

class XDrawable;
class XGraphics;

#define CONVERT_RETURN(fn, traits, ...)                              \
  template <typename... Args>                                        \
  traits::XppType fn(Args&&... args) {                               \
    return traits::Import(                                           \
        display_->fn(this->Drawable(), std::forward<Args>(args)...), \
        shared_from_this(), ##__VA_ARGS__);                          \
  }

#define NO_CONVERSIONS(fn, ret)                                         \
  template <typename... Args>                                           \
  ret fn(Args&&... args) {                                              \
    return display_->fn(this->Drawable(), std::forward<Args>(args)...); \
  }

// Define converters
template <>
struct Traits<XGraphics> {
  using XppType = std::shared_ptr<XGraphics>;
  using XorgType = ::GC;
  static XppType Import(const XorgType& graphics,
                        std::shared_ptr<XDrawable> drawable,
                        std::shared_ptr<XDisplay> display,
                        std::shared_ptr<XColorMap> colormap);
};

class XDrawable : public std::enable_shared_from_this<XDrawable> {
 public:
  virtual ~XDrawable() = default;
  virtual ::Drawable Drawable() = 0;
  XDrawable(std::shared_ptr<XDisplay>);

  Traits<XGraphics>::XppType XCreateGC(std::shared_ptr<XColorMap> cmap) {
    return Traits<XGraphics>::Import(
        display_->XCreateGC(this->Drawable(), 0, nullptr), shared_from_this(),
        display_, std::move(cmap));
  }

  NO_CONVERSIONS(XFillRectangle, void);
  NO_CONVERSIONS(XDrawRectangle, void);
  NO_CONVERSIONS(XDrawString, void);
  NO_CONVERSIONS(XDrawArc, void);
  NO_CONVERSIONS(XFillArc, void);
  NO_CONVERSIONS(XDrawLine, void);
  NO_CONVERSIONS(XftFontOpenName, XftFont*);
  NO_CONVERSIONS(XftDrawCreate, XftDraw*);

 protected:
  std::shared_ptr<XDisplay> display_;
};

#undef CONVERT_RETURN
#undef NO_CONVERSIONS

}  // namespace xpp::xlib