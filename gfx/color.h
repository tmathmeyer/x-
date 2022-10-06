#pragma once

#include <cstdint>
#include <iostream>

#include "base/check.h"

namespace xpp::gfx {

namespace {

template <typename Numeric>
Numeric hue2rgb(Numeric p, Numeric q, Numeric t) {
  if (t < 0)
    t += 1;
  if (t > 1)
    t -= 1;
  if (t < 1. / 6)
    return p + (q - p) * 6 * t;
  if (t < 1. / 2)
    return q;
  if (t < 2. / 3)
    return p + (q - p) * (2. / 3 - t) * 6;

  return p;
}

}  // namespace

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

    Int R = ((100 - C) * (100 - K) * Depth::Max) / 10000;
    Int G = ((100 - M) * (100 - K) * Depth::Max) / 10000;
    Int B = ((100 - Y) * (100 - K) * Depth::Max) / 10000;

    return ColorImpl(R, G, B);
  }

  static ColorImpl HSV(Float H, Float S, Float V) {
    CHECK(H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0);

    Float s = S / 100;
    Float v = V / 100;
    Float C = s * v;
    Float X = C * (1 - std::abs(fmod(H / 60.0, 2) - 1));
    Float m = v - C;
    Float r, g, b;
    if (H >= 0 && H < 60) {
      r = C, g = X, b = 0;
    } else if (H >= 60 && H < 120) {
      r = X, g = C, b = 0;
    } else if (H >= 120 && H < 180) {
      r = 0, g = C, b = X;
    } else if (H >= 180 && H < 240) {
      r = 0, g = X, b = C;
    } else if (H >= 240 && H < 300) {
      r = X, g = 0, b = C;
    } else {
      r = C, g = 0, b = X;
    }
    Int R = (r + m) * Depth::Max;
    Int G = (g + m) * Depth::Max;
    Int B = (b + m) * Depth::Max;
    return ColorImpl(R, G, B);
  }

  static ColorImpl HSL(Float H, Float S, Float L) {
    Float R = 0;
    Float G = 0;
    Float B = 0;
    if (S == 0) {
      R = G = B = L * Depth::Max;
    } else {
      Float q = L < 0.5 ? L * (1 + S) : L + S - L * S;
      Float p = 2 * L - q;
      R = hue2rgb<Float>(p, q, H + 1.0 / 3) * Depth::Max;
      G = hue2rgb<Float>(p, q, H) * Depth::Max;
      B = hue2rgb<Float>(p, q, H - 1.0 / 3) * Depth::Max;
    }
    return ColorImpl(R, G, B);
  }

  static ColorImpl Black() { return ColorImpl(0, 0, 0); }

  static ColorImpl White() {
    return ColorImpl(Depth::Max, Depth::Max, Depth::Max);
  }

  uint16_t red() const { return static_cast<uint16_t>(R) * Depth::Scale; }

  uint16_t green() const { return static_cast<uint16_t>(G) * Depth::Scale; }

  uint16_t blue() const { return static_cast<uint16_t>(B) * Depth::Scale; }

  std::tuple<Float, Float, Float> GetHSL() const {
    Float r = (Float)R / Depth::Max;
    Float g = (Float)G / Depth::Max;
    Float b = (Float)B / Depth::Max;

    Float max = std::max(std::max(r, g), b);
    Float min = std::min(std::min(r, g), b);

    Float H, S, L;
    H = S = L = (max + min) / 2;

    if (max == min) {
      H = S = 0;
    } else {
      Float d = max - min;
      S = (L > 0.5) ? d / (2 - max - min) : d / (max + min);
      if (max == r) {
        H = (g - b) / d + (g < b ? 6 : 0);
      } else if (max == g) {
        H = (b - r) / d + 2;
      } else if (max == b) {
        H = (r - g) / d + 4;
      }

      H /= 6;
    }

    return std::tie(H, S, L);
  }

  ColorImpl Darker(Float value = 0.9) const {
    auto hsl = GetHSL();
    return ColorImpl::HSL(std::get<0>(hsl), std::get<1>(hsl),
                          std::get<2>(hsl) * value);
  }

  ColorImpl Lighter(Float value = 0.9) const {
    auto hsl = GetHSL();
    Float lum = 1.0 - std::get<2>(hsl);
    return ColorImpl::HSL(std::get<0>(hsl), std::get<1>(hsl),
                          1.0 - (lum * value));
  }

  bool operator==(const ColorImpl& other) const {
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
