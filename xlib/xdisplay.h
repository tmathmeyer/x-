#pragma once

#include <cstdio>
#include <map>
#include <memory>

#include <X11/XKBlib.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>

#include "xpp_inherit/gfx/coord.h"
#include "xpp_inherit/gfx/rect.h"

#include "xorg_typemap.h"

namespace xpp::xlib {

#define NO_CONVERSIONS(fn, ret)                         \
  template <typename... Args>                           \
  ret fn(Args&&... args) {                              \
    return ::fn(display_, std::forward<Args>(args)...); \
  }

#define NO_RETURN(fn)                            \
  template <typename... Args>                    \
  void fn(Args&&... args) {                      \
    ::fn(display_, std::forward<Args>(args)...); \
  }

#define CONVERT_RETURN(fn, traits, ...)                                \
  template <typename... Args>                                          \
  traits::XppType fn(Args&&... args) {                                 \
    return traits::Import(::fn(display_, std::forward<Args>(args)...), \
                          shared_from_this(), ##__VA_ARGS__);          \
  }                                                                    \
  template <typename... Args>                                          \
  decltype(auto) fn##Raw(Args&&... args) {                             \
    return ::fn(display_, std::forward<Args>(args)...);                \
  }

// Forward declare.
class XColorMap;
class XDisplay;
class XWindow;
class XPixmap;

// Define converters
template <>
struct Traits<XWindow> {
  using XppType = std::shared_ptr<XWindow>;
  using XorgType = ::Window;
  static XppType Import(const XorgType& window,
                        std::shared_ptr<XDisplay> display,
                        bool destroy_on_delete = true);
};

template <>
struct Traits<XPixmap> {
  using XppType = std::shared_ptr<XPixmap>;
  using XorgType = ::Pixmap;
  static XppType Import(const XorgType& pixmap,
                        std::shared_ptr<XDisplay> display);
};

class XDisplay : public std::enable_shared_from_this<XDisplay> {
 public:
  static std::shared_ptr<XDisplay> Create(const char* id = nullptr);
  static std::map<std::string, gfx::Rect> GetMonitorSizes();
  static constexpr Atom kAtom = 4;
  ~XDisplay();

  using XWindowTraits = Traits<XWindow>;

  NO_CONVERSIONS(XDisplayWidth, int);
  NO_CONVERSIONS(XkbKeycodeToKeysym, KeySym);
  NO_CONVERSIONS(XDisplayHeight, int);
  NO_CONVERSIONS(XDefaultScreen, int);
  NO_CONVERSIONS(XCreateGC, GC);
  NO_CONVERSIONS(XCreateColormap, Colormap);
  NO_CONVERSIONS(XLoadQueryFont, XFontStruct*);
  NO_CONVERSIONS(XftFontOpenName, XftFont*);
  NO_CONVERSIONS(XftFontClose, void);
  NO_CONVERSIONS(XftDrawCreate, XftDraw*);
  NO_CONVERSIONS(XftColorAllocValue, bool);
  NO_CONVERSIONS(XftColorFree, void);
  NO_CONVERSIONS(XftTextExtentsUtf8, void);
  NO_CONVERSIONS(XRRGetScreenResourcesCurrent, XRRScreenResources*);
  NO_CONVERSIONS(XRRGetOutputInfo, XRROutputInfo*);
  NO_CONVERSIONS(XRRGetCrtcInfo, XRRCrtcInfo*);
  NO_CONVERSIONS(XInternAtom, Atom);
  NO_CONVERSIONS(XChangeProperty, int);

  CONVERT_RETURN(XRootWindow, XWindowTraits, false);
  CONVERT_RETURN(XCreateWindow, XWindowTraits);
  CONVERT_RETURN(XCreatePixmap, Traits<XPixmap>);

  NO_RETURN(XDestroyWindow);
  NO_RETURN(XFreePixmap);
  NO_RETURN(XNextEvent);
  NO_RETURN(XMapWindow);
  NO_RETURN(XMapRaised);
  NO_RETURN(XUnmapWindow);
  NO_RETURN(XMatchVisualInfo);
  NO_RETURN(XSelectInput);
  NO_RETURN(XFreeGC);
  NO_RETURN(XFillRectangle);
  NO_RETURN(XDrawRectangle);
  NO_RETURN(XDrawArc);
  NO_RETURN(XFillArc);
  NO_RETURN(XDrawLine);
  NO_RETURN(XSetLineAttributes);
  NO_RETURN(XDrawString);
  NO_RETURN(XSetForeground);
  NO_RETURN(XAllocColor);
  NO_RETURN(XSendEvent);
  NO_RETURN(XCopyArea);
  NO_RETURN(XFreeFont);
  NO_RETURN(XSetFont);
  NO_RETURN(XResizeWindow);
  NO_RETURN(XMoveWindow);
  NO_RETURN(XMoveResizeWindow);

 private:
  XDisplay(const char* id);
  ::Display* display_;
};

#undef NO_CONVERSIONS
#undef NO_RETURN
#undef CONVERT_RETURN

}  // namespace xpp::xlib