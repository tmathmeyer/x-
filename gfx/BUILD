langs("C")

cpp_header (
  name = "include",
  srcs = [
    "color.h",
    "coord.h",
    "font.h",
    "rect.h",
    "util.h",
  ],
  deps = [
    "//base:check",
  ],
  flags = [
    "-lfreetype",
  ],
  include_dirs = [
    "/usr/include/freetype2",
  ],
)

cpp_object (
  name = "util",
  srcs = [
    "util.cc",
  ],
  deps = [
    ":include",
  ],
)