langs("C")

cpp_header (
  name = "include",
  srcs = [
    "accordion.h",
    "button.h",
    "button_listener.h",
    "canvas.h",
    "component.h",
    "container.h",
    "graphics.h",
    "look_and_feel.h",
    "panel.h",
    "scroll_panel.h",
    "window.h",
    "window_interface.h",
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
    "accordion.cc",
    "button.cc",
    "canvas.cc",
    "component.cc",
    "container.cc",
    "graphics.cc",
    "look_and_feel.cc",
    "panel.cc",
    "scroll_panel.cc",
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