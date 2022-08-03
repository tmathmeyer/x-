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
    "//base/status:status_h",
    "//xpp_inherit/gfx:include",
  ],
  include_dirs = [
    "/usr/include/freetype2",
  ]
)

cpp_object (
  name = "xdrawable",
  srcs = [ "xdrawable.cc" ],
  deps = [
    ":include",
    ":xgraphics",
    "//base/status:status",
  ],
  include_dirs = [
    "/usr/include/freetype2",
  ],
  flags = [
    "-lX11",
  ],
)

cpp_object (
  name = "xpixmap",
  srcs = [ "xpixmap.cc" ],
  deps = [
    ":include",
    ":xdrawable",
    "//base/status:status",
  ],
)

cpp_object (
  name = "xdisplay",
  srcs = [ "xdisplay.cc" ],
  deps = [
    ":include",
    ":xwindow",
    "//base/status:status",
  ],
  flags = [
    "-lXft",
    "-lXext",
    "-lXrandr",
    "-lX11",
  ],
)

cpp_object (
  name = "xwindow",
  srcs = [ "xwindow.cc" ],
  deps = [
    ":include",
    ":xdrawable",
    ":xpixmap",
    ":xcolormap",
    "//base/status:status",
  ],
)

cpp_object (
  name = "xgraphics",
  srcs = [ "xgraphics.cc" ],
  deps = [
    ":include",
    "//base/status:status",
  ],
)

cpp_object (
  name = "xcolormap",
  srcs = [ "xcolormap.cc" ],
  deps = [
    ":include",
    "//base/status:status",
  ],
)