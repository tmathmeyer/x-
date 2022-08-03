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

cpp_object (
  name = "fill_layout",
  srcs = [
    "fill_layout.cc",
  ],
  deps = [
    ":include",
  ],
)