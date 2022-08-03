#pragma once

#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace xpp::gfx {

template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
struct RectImpl {
  T width;
  T height;

  RectImpl(T w, T h) : width(w), height(h) {}

  bool operator==(const RectImpl& other) const {
    return other.width == width && other.height == height;
  }

  bool operator!=(const RectImpl& other) const { return !(*this == other); }

  RectImpl Transpose() const { return RectImpl(height, width); }
};

using Rect = RectImpl<uint32_t>;

}  // namespace xpp::gfx