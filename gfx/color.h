#pragma once

#include <cstdint>

#include "base/check.h"

namespace xpp::gfx {

template <typename Depth>
class ColorImpl {
 public:
  using Int = typename Depth::Int;
  using Float = typename Depth::Float;

  static ColorImpl BLACK;
  static ColorImpl GRAY1;
  static ColorImpl GRAY2;
  static ColorImpl GRAY3;
  static ColorImpl GRAY4;
  static ColorImpl GRAY5;
  static ColorImpl GRAY6;
  static ColorImpl GRAY7;
  static ColorImpl GRAY8;
  static ColorImpl GRAY9;
  static ColorImpl WHITE;
  static ColorImpl RED;
  static ColorImpl ORANGE;
  static ColorImpl YELLOW;
  static ColorImpl GREEN;
  static ColorImpl MINT;
  static ColorImpl TEAL;
  static ColorImpl CYAN;
  static ColorImpl BLUE;
  static ColorImpl INDIGO;
  static ColorImpl PURPLE;
  static ColorImpl PINK;
  static ColorImpl BROWN;

  bool operator<(const ColorImpl& other) const {
    return Index() < other.Index();
  }

  Int Index() const {
    return R + (G * Depth::Max) + (B * Depth::Max * Depth::Max);
  }

  ~ColorImpl() = default;

  static ColorImpl RGB(Int R, Int G, Int B) { return ColorImpl(R, G, B); }

  static ColorImpl CMYK(Int C, Int M, Int Y, Int K) {
    CHECK((C >= 0 && M >= 0 && Y >= 0 && K >= 0));
    CHECK((C <= 100 && M <= 100 && Y <= 100 && K <= 100));

    int R = ((100 - C) * (100 - K) * Depth::Max) / 10000;
    int G = ((100 - M) * (100 - K) * Depth::Max) / 10000;
    int B = ((100 - Y) * (100 - K) * Depth::Max) / 10000;

    return ColorImpl(R, G, B);
  }

  static ColorImpl HSV(Float H, Float S, Float V) { return Black(); }

  static ColorImpl HSL(Float H, Float S, Float L) { return Black(); }

  static ColorImpl Black() { return ColorImpl(0, 0, 0); }

  static ColorImpl White() {
    return ColorImpl(Depth::Max, Depth::Max, Depth::Max);
  }

  uint16_t red() { return static_cast<uint16_t>(R) * Depth::Scale; }

  uint16_t green() { return static_cast<uint16_t>(G) * Depth::Scale; }

  uint16_t blue() { return static_cast<uint16_t>(B) * Depth::Scale; }

  bool operator==(const ColorImpl& other) {
    return other.R == R && other.G == G && other.B == B;
  }

  ColorImpl() : ColorImpl(0, 0, 0) {}

 private:
  Int R;
  Int G;
  Int B;

  ColorImpl(Int R, Int G, Int B) : R(R), G(G), B(B) {
    CHECK((R >= 0 && G >= 0 && B >= 0));
    CHECK((R <= 255 && G <= 255 && B <= 255));
  }
};

struct Depth8 {
  using Int = uint8_t;
  using Float = float;
  static constexpr Int Max = 255;
  static constexpr uint16_t Scale = 257;
};

struct Depth16 {
  using Int = uint16_t;
  using Float = float;
  static constexpr Int Max = 65535;
  static constexpr uint16_t Scale = 1;
};

using Color = ColorImpl<Depth8>;
using HDRColor = ColorImpl<Depth16>;

template <typename D>
ColorImpl<D> ColorImpl<D>::BLACK = ColorImpl<D>::RGB(28, 28, 30);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY1 = ColorImpl<D>::RGB(44, 44, 46);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY2 = ColorImpl<D>::RGB(58, 58, 60);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY3 = ColorImpl<D>::RGB(72, 72, 74);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY4 = ColorImpl<D>::RGB(99, 99, 102);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY5 = ColorImpl<D>::RGB(142, 142, 147);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY6 = ColorImpl<D>::RGB(174, 174, 178);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY7 = ColorImpl<D>::RGB(199, 199, 204);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY8 = ColorImpl<D>::RGB(209, 209, 214);

template <typename D>
ColorImpl<D> ColorImpl<D>::GRAY9 = ColorImpl<D>::RGB(229, 229, 234);

template <typename D>
ColorImpl<D> ColorImpl<D>::WHITE = ColorImpl<D>::RGB(242, 242, 247);

template <typename D>
ColorImpl<D> ColorImpl<D>::RED = ColorImpl<D>::RGB(255, 59, 48);

template <typename D>
ColorImpl<D> ColorImpl<D>::ORANGE = ColorImpl<D>::RGB(255, 149, 0);

template <typename D>
ColorImpl<D> ColorImpl<D>::YELLOW = ColorImpl<D>::RGB(255, 204, 0);

template <typename D>
ColorImpl<D> ColorImpl<D>::GREEN = ColorImpl<D>::RGB(52, 199, 89);

template <typename D>
ColorImpl<D> ColorImpl<D>::MINT = ColorImpl<D>::RGB(0, 199, 190);

template <typename D>
ColorImpl<D> ColorImpl<D>::TEAL = ColorImpl<D>::RGB(48, 176, 199);

template <typename D>
ColorImpl<D> ColorImpl<D>::CYAN = ColorImpl<D>::RGB(50, 173, 230);

template <typename D>
ColorImpl<D> ColorImpl<D>::BLUE = ColorImpl<D>::RGB(0, 122, 255);

template <typename D>
ColorImpl<D> ColorImpl<D>::INDIGO = ColorImpl<D>::RGB(88, 86, 214);

template <typename D>
ColorImpl<D> ColorImpl<D>::PURPLE = ColorImpl<D>::RGB(175, 82, 222);

template <typename D>
ColorImpl<D> ColorImpl<D>::PINK = ColorImpl<D>::RGB(255, 45, 85);

template <typename D>
ColorImpl<D> ColorImpl<D>::BROWN = ColorImpl<D>::RGB(162, 132, 94);

}  // namespace xpp::gfx
