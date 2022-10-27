langs("C")

cpp_header (
  name = "include",
  srcs = [
    "color.h",
    "coord.h",
    "rect.h",
    "util.h",
  ],
  deps = [
    "//base:include",
  ],
  flags = [
    "-lfreetype",
  ],
  include_dirs = [
    "/usr/include/freetype2",
  ],
)

cc_object (
  name = "util",
  srcs = [
    "util.cc",
  ],
  include = [
    ":include",
  ],
)