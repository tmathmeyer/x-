#pragma once

namespace xpp::xlib {

template <typename Xpp>
struct Traits {
  using XppType = Xpp;
  using XorgType = int;
  static XppType Export(const XorgType& input) {
    static_assert(false && sizeof(XppType), "No converted defined");
    return XppType();
  }
};

}  // namespace xpp::xlib