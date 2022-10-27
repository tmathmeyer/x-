langs("C")

cpp_header (
  name = "include",
  srcs = [
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
    "fill_layout.cc",
    "panel_layout.cc",
  ],
  includes = [
    ":include",
    "//xpp/ui:include",
  ],
)