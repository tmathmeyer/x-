langs("C")

cpp_header (
  name = "include",
  srcs = [
    "button.h",
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

cpp_object (
  name = "window",
  srcs = [
    "window.cc",
  ],
  deps = [
    ":container",
    ":include",
    ":look_and_feel",
    "//xpp_inherit/xlib:xdisplay",
  ],
)

cpp_object (
  name = "container",
  srcs = [
    "container.cc",
  ],
  deps = [
    ":component",
    ":graphics",
    ":include",
    "//xpp_inherit/gfx:util",
  ],
)

cpp_object (
  name = "component",
  srcs = [
    "component.cc",
  ],
  deps = [
    ":include",
    "//xpp_inherit/ui/layout:fill_layout",
  ],
)

cpp_object (
  name = "graphics",
  srcs = [
    "graphics.cc",
  ],
  deps = [
    ":include",
  ],
)

cpp_object (
  name = "look_and_feel",
  srcs = [
    "look_and_feel.cc",
  ],
  deps = [
    ":include",
  ],
)

cpp_object (
  name = "button",
  srcs = [
    "button.cc",
  ],
  deps = [
    ":include",
  ],
)

cpp_object (
  name = "panel",
  srcs = [
    "panel.cc",
  ],
  deps = [
    ":include",
  ],
)