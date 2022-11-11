#pragma once

#include <memory>

#include "xcolormap.h"
#include "xorg_typemap.h"
#include "xwindow.h"

namespace xpp::xlib {

#define DRAWABLE_METHOD(fn, ret)                                  \
  template <typename... Args>                                     \
  ret fn(Args&&... args) {                                        \
    return drawable_->fn(graphics_, std::forward<Args>(args)...); \
  }

#define DISPLAY_METHOD(fn, ret)                                  \
  template <typename... Args>                                    \
  ret fn(Args&&... args) {                                       \
    return display_->fn(graphics_, std::forward<Args>(args)...); \
  }

#define DISPLAY_METHOD_PASSTHROUGH(fn, ret)           \
  template <typename... Args>                         \
  ret fn(Args&&... args) {                            \
    return display_->fn(std::forward<Args>(args)...); \
  }

#define DISPLAY_METHOD_SCREEN(fn, ret)                \
  template <typename... Args>                         \
  ret fn(Args&&... args) {                            \
    return display_->fn(display_->XDefaultScreen(),   \
                        std::forward<Args>(args)...); \
  }

class XGraphics {
 public:
  ~XGraphics();
  DRAWABLE_METHOD(XFillRectangle, void);
  DRAWABLE_METHOD(XDrawRectangle, void);
  DRAWABLE_METHOD(XDrawArc, void);
  DRAWABLE_METHOD(XFillArc, void);
  DRAWABLE_METHOD(XDrawString, void);
  DRAWABLE_METHOD(XDrawLine, void);

  DISPLAY_METHOD(XSetForeground, void);
  DISPLAY_METHOD(XSetFont, void);
  DISPLAY_METHOD(XSetLineAttributes, void);

  DISPLAY_METHOD_PASSTHROUGH(XLoadQueryFont, XFontStruct*);
  DISPLAY_METHOD_PASSTHROUGH(XFreeFont, void);
  DISPLAY_METHOD_PASSTHROUGH(XftFontClose, void);
  DISPLAY_METHOD_PASSTHROUGH(XftTextExtentsUtf8, void);

  DISPLAY_METHOD_SCREEN(XftFontOpenName, XftFont*);

  void XCopyArea(Drawable src, int x, int y, uint w, uint h, int dx, int dy) {
    display_->XCopyArea(src, drawable_->Drawable(), graphics_, x, y, w, h, dx,
                        dy);
  }

  XftDraw* XftDrawCreate() {
    XVisualInfo vinfo;
    display_->XMatchVisualInfo(display_->XDefaultScreen(), 32, TrueColor,
                               &vinfo);
    return drawable_->XftDrawCreate(vinfo.visual, colormap_->colormap());
  }

  bool XftColorAllocValue(XRenderColor* color, XftColor* result) {
    XVisualInfo vinfo;
    display_->XMatchVisualInfo(display_->XDefaultScreen(), 32, TrueColor,
                               &vinfo);
    return display_->XftColorAllocValue(vinfo.visual, colormap_->colormap(),
                                        color, result);
  }

  void XftColorFree(XftColor* color) {
    XVisualInfo vinfo;
    display_->XMatchVisualInfo(display_->XDefaultScreen(), 32, TrueColor,
                               &vinfo);
    return display_->XftColorFree(vinfo.visual, colormap_->colormap(), color);
  }

  ::GC operator*();

 private:
  std::shared_ptr<XDrawable> drawable_;
  std::shared_ptr<XDisplay> display_;
  std::shared_ptr<XColorMap> colormap_;
  ::GC graphics_;

  // Private constructor. Must come from the display!
  XGraphics(::GC graphics,
            std::shared_ptr<XDrawable> drawable,
            std::shared_ptr<XDisplay> display,
            std::shared_ptr<XColorMap> colormap);

  // Allow XDrawable to create XGraphics
  friend struct Traits<XGraphics>;
};

#undef DRAWABLE_METHOD
#undef DISPLAY_METHOD
#undef DISPLAY_METHOD_PASSTHROUGH
#undef DISPLAY_METHOD_SCREEN

}  // namespace xpp::xlib
