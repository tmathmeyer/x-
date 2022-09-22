#pragma once

#include <memory>

#include "xpp/gfx/coord.h"
#include "xpp/gfx/rect.h"

#include "xcolormap.h"
#include "xdisplay.h"
#include "xdrawable.h"
#include "xorg_typemap.h"
#include "xstatus.h"

namespace xpp::xlib {

#define NO_CONVERSIONS(fn, ret)                                \
  template <typename... Args>                                  \
  ret fn(Args&&... args) {                                     \
    return display_->fn(window_, std::forward<Args>(args)...); \
  }

#define CONVERT_RETURN(fn, traits, ...)                                      \
  template <typename... Args>                                                \
  traits::XppType fn(Args&&... args) {                                       \
    auto pre_converted = display_->fn(window_, std::forward<Args>(args)...); \
    std::shared_ptr<XWindow> w =                                             \
        std::static_pointer_cast<XWindow>(shared_from_this());               \
    return traits::Import(pre_converted, std::move(w), ##__VA_ARGS__);       \
  }

template <>
struct Traits<XColorMap> {
  using XppType = std::shared_ptr<XColorMap>;
  using XorgType = ::Colormap;
  static XppType Import(const XorgType& colormap,
                        std::shared_ptr<XWindow> window,
                        std::shared_ptr<XDisplay> display);
};

class XWindow : public XDrawable {
 public:
  ~XWindow() override;
  CONVERT_RETURN(XCreateColormap, Traits<XColorMap>, display_);

  NO_CONVERSIONS(XCreatePixmap, auto);
  NO_CONVERSIONS(XCreateWindow, auto);
  NO_CONVERSIONS(XRRGetScreenResourcesCurrent, auto);
  NO_CONVERSIONS(XMapWindow, void);
  NO_CONVERSIONS(XMapRaised, void);
  NO_CONVERSIONS(XUnmapWindow, void);
  NO_CONVERSIONS(XSelectInput, void);
  NO_CONVERSIONS(XSendEvent, void);
  NO_CONVERSIONS(XResizeWindow, void);
  NO_CONVERSIONS(XMoveWindow, void);
  NO_CONVERSIONS(XMoveResizeWindow, void);
  NO_CONVERSIONS(XChangeProperty, int);

  ::Window operator*();
  ::Drawable Drawable() override;

 private:
  ::Window window_;
  bool destroy_on_delete_;

  // Private constructor. Must come from the display!
  XWindow(std::shared_ptr<XDisplay> display,
          ::Window window,
          bool destroy_on_delete);

  // Allow XDisplay to create XWindow
  friend class Traits<XWindow>;
};

#undef NO_CONVERSIONS

}  // namespace xpp::xlib