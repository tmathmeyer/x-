langs("C")

cpp_header (
  name = "include",
  srcs = [
    "fill_layout.h",
    "layout.h",
  ],
  deps = [
    "//xpp/gfx:include",
  ],
)

cc_object (
  name = "layouts",
  srcs = [
    "fill_layout.cc",
  ],
  includes = [
    ":include",
  ],
)