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
    "//xpp/xlib:include",
    "//xpp/ui/event:include",
    "//xpp/ui/layout:include",
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
    "//xpp/xlib:xpp-xlib",
    "//xpp/gfx:util",
    "//xpp/ui/layout:layouts",
  ],
)