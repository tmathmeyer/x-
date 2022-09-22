langs("C")

cpp_header (
  name = "include",
  srcs = [
    "accordion_layout.h",
    "fill_layout.h",
    "layout.h",
    "panel_layout.h",
  ],
  deps = [
    "//xpp/gfx:include",
  ],
)

cc_object (
  name = "layouts",
  srcs = [
    "accordion_layout.cc",
    "fill_layout.cc",
    "panel_layout.cc",
  ],
  includes = [
    ":include",
    "//xpp/ui:include",
  ],
)