langs("C")

cpp_header (
  name = "include",
  srcs = [
    "xcolormap.h",
    "xdisplay.h",
    "xdrawable.h",
    "xgraphics.h",
    "xorg_typemap.h",
    "xpixmap.h",
    "xstatus.h",
    "xwindow.h",
  ],
  deps = [
    "//base/status:include",
    "//xpp_inherit/gfx:include",
  ]
)

cc_object (
  name = "xpp-xlib",
  srcs = [
    "xcolormap.cc",
    "xdisplay.cc",
    "xdrawable.cc",
    "xgraphics.cc",
    "xpixmap.cc",
    "xwindow.cc",
  ],
  includes = [
    ":include",
  ],
  deps = [
    "//base/status:status",
  ],
  flags = [
    "-lXft",
    "-lXext",
    "-lXrandr",
    "-lX11",
  ],
)