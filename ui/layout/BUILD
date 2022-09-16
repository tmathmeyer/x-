langs("C")

cpp_header (
  name = "include",
  srcs = [
    "fill_layout.h",
    "layout.h",
  ],
  deps = [
    "//xpp_inherit/gfx:include",
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