#include "look_and_feel.h"

#include <sstream>

namespace xpp::ui {

LookAndFeel::LookAndFeel() {
  SetColor("PanelBackground", gfx::Color::GRAY8);
  SetColor("PanelBorder", gfx::Color::GRAY6);
  SetColor("TextColor", gfx::Color::BLACK);

  SetColor("ButtonTextColor", gfx::Color::WHITE);
  SetColor("ButtonBackground", gfx::Color::BLUE);
  SetColor("ButtonShadow", gfx::Color::GRAY2);
  SetColor("ButtonBorder", gfx::Color::GRAY5);

  SetColor("ButtonHoveredTextColor", gfx::Color::WHITE);
  SetColor("ButtonHoveredBackground", gfx::Color::BLUE.Burn());
  SetColor("ButtonHoveredShadow", gfx::Color::GRAY2);
  SetColor("ButtonHoveredBorder", gfx::Color::GRAY5);

  SetColor("ButtonPressedTextColor", gfx::Color::WHITE);
  SetColor("ButtonPressedBackground", gfx::Color::BLUE);
  SetColor("ButtonPressedShadow", gfx::Color::GRAY2);
  SetColor("ButtonPressedBorder", gfx::Color::GRAY5);
}

XColor LookAndFeel::GetXColor(std::shared_ptr<xlib::XColorMap> colormap,
                              gfx::Color color) {
  auto itr = xcolors_.find(color);
  if (itr != xcolors_.end())
    return itr->second;

  XColor xcolor;
  xcolor.red = color.red();
  xcolor.blue = color.blue();
  xcolor.green = color.green();
  colormap->XAllocColor(&xcolor);
  xcolors_[color] = xcolor;
  return xcolor;
}

XftColor LookAndFeel::GetXFTColor(std::shared_ptr<xlib::XGraphics> gc,
                                  gfx::Color color) {
  auto itr = xft_colors_.find(color);
  if (itr != xft_colors_.end())
    return itr->second;

  XftColor xft_color;
  XRenderColor xrc = {.red = color.red(),
                      .green = color.green(),
                      .blue = color.blue(),
                      .alpha = 0xFFFF};
  gc->XftColorAllocValue(&xrc, &xft_color);
  xft_colors_[color] = xft_color;
  return xft_color;
}

gfx::Font LookAndFeel::AllocateFont(std::shared_ptr<xlib::XGraphics> gc,
                                    std::string key,
                                    std::string name,
                                    uint16_t size) {
  auto itr = fonts_.find(key);
  if (itr != fonts_.end())
    return itr->second;

  gfx::Font font;

  // try to allocate an XFont first, since it is simpler
  font.xfont_ = gc->XLoadQueryFont(name.c_str());
  if (font.xfont_) {
    font.mode_ = gfx::Font::TextRenderingMode::kXorg;
    fonts_[key] = font;
    return font;
  }

  // If it failed, use an XFT font
  std::stringstream fontstr;
  fontstr << name << ":size=" << size;
  font.xft_font_ = gc->XftFontOpenName(fontstr.str().c_str());
  if (font.xft_font_) {
    if (!xft_ctx_)
      xft_ctx_ = gc->XftDrawCreate();
    if (xft_ctx_) {
      font.mode_ = gfx::Font::TextRenderingMode::kXFT;
      fonts_[key] = font;
    }
    return font;
  }

  font.mode_ = gfx::Font::TextRenderingMode::kFailed;
  return font;
}

gfx::Font LookAndFeel::GetFont(std::shared_ptr<xlib::XGraphics> gc,
                               std::string name,
                               gfx::Font existing) {
  std::stringstream stream;
  stream << name << "::" << existing.size_;
  return AllocateFont(gc, stream.str(), name, existing.size_);
}

gfx::Font LookAndFeel::GetFont(std::shared_ptr<xlib::XGraphics> gc,
                               uint16_t size,
                               gfx::Font existing) {
  std::stringstream stream;
  stream << existing.font_name_ << "::" << size;
  return AllocateFont(gc, stream.str(), existing.font_name_, size);
}

gfx::Color LookAndFeel::GetColorByName(std::string color) {
  auto itr = colors_.find(color);
  if (itr != colors_.end())
    return itr->second;
  return gfx::Color::BLACK;
}

void LookAndFeel::SetColor(std::string name, gfx::Color color) {
  colors_[name] = color;
}

}  // namespace xpp::ui