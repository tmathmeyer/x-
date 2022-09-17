langs("C")

cpp_header (
  name = "include",
  srcs = [
    "button.h",
    "button_listener.h",
    "component.h",
    "container.h",
    "graphics.h",
    "look_and_feel.h",
    "panel.h",
    "window.h",
  ],
  deps = [
    "//xpp_inherit/xlib:include",
    "//xpp_inherit/ui/event:include",
    "//xpp_inherit/ui/layout:include",
  ],
)

cc_object (
  name = "xpp-uilib",
  srcs = [
    "button.cc",
    "component.cc",
    "container.cc",
    "graphics.cc",
    "look_and_feel.cc",
    "panel.cc",
    "window.cc",
  ],
  includes = [
    ":include",
  ],
  deps = [
    "//xpp_inherit/xlib:xpp-xlib",
    "//xpp_inherit/gfx:util",
    "//xpp_inherit/ui/layout:layouts",
  ],
)