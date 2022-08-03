#pragma once

#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace xpp::gfx {

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
struct CoordImpl {
  T x;
  T y;

  CoordImpl(T x, T y) : x(x), y(y) {}

  bool operator==(const CoordImpl& other) const {
    return other.x == x && other.y == y;
  }

  bool operator!=(const CoordImpl& other) const { return !(*this == other); }

  CoordImpl Transpose() const { return CoordImpl(y, x); }

  CoordImpl operator+(const CoordImpl& other) const {
    return {x + other.x, y + other.y};
  }

  CoordImpl operator-(const CoordImpl& other) const {
    return {x - other.x, y - other.y};
  }
};

using Coord = CoordImpl<int64_t>;

}  // namespace xpp::gfx