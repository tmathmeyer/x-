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
  SetColor("ButtonHoveredBackground", gfx::Color::BLUE.Darker());
  SetColor("ButtonHoveredShadow", gfx::Color::GRAY2);
  SetColor("ButtonHoveredBorder", gfx::Color::GRAY5);

  SetColor("ButtonPressedTextColor", gfx::Color::WHITE);
  SetColor("ButtonPressedBackground", gfx::Color::BLUE);
  SetColor("ButtonPressedShadow", gfx::Color::GRAY2);
  SetColor("ButtonPressedBorder", gfx::Color::GRAY5);

  SetColor("ScrollbarTrackColor", gfx::Color::GRAY6);
  SetColor("ScrollbarTrackBorderColor", gfx::Color::GRAY6);
  SetColor("ScrollbarColor", gfx::Color::GRAY2);
  SetColor("ScrollbarBorderColor", gfx::Color::GRAY4);
  SetColor("ScrollbarHoveredColor", gfx::Color::BLUE);

  SetColor("AccordionHeaderBackgroundColorOpen", gfx::Color::GRAY7);
  SetColor("AccordionHeaderBackgroundColorClosed", gfx::Color::GRAY7);
  SetColor("AccordionHeaderTextColorOpen", gfx::Color::GRAY1);
  SetColor("AccordionHeaderTextColorClosed", gfx::Color::GRAY1);
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
                                    std::string name,
                                    uint16_t size,
                                    FontCache* fonts) {
  std::stringstream stream;
  stream << name << "::" << size;
  std::string key = stream.str();

  auto itr = fonts->fonts.find(key);
  if (itr != fonts->fonts.end())
    return itr->second;

  gfx::Font font;

  // try to allocate an XFont first, since it is simpler
  font.xfont_ = gc->XLoadQueryFont(name.c_str());
  if (font.xfont_) {
    font.mode_ = gfx::Font::TextRenderingMode::kXorg;
    fonts->fonts[key] = font;
    return font;
  }

  // If it failed, use an XFT font
  std::stringstream fontstr;
  fontstr << name << ":size=" << size;
  font.xft_font_ = gc->XftFontOpenName(fontstr.str().c_str());
  if (font.xft_font_) {
    if (!fonts->xft_ctx)
      fonts->xft_ctx = gc->XftDrawCreate();
    if (fonts->xft_ctx) {
      font.mode_ = gfx::Font::TextRenderingMode::kXFT;
      font.size_ = size;
      font.font_name_ = name;
      fonts->fonts[key] = font;
    }
    return font;
  }

  font.mode_ = gfx::Font::TextRenderingMode::kFailed;
  return font;
}

gfx::Font LookAndFeel::GetFont(std::shared_ptr<xlib::XGraphics> gc,
                               std::string name,
                               gfx::Font existing,
                               FontCache* fonts) {
  return AllocateFont(gc, name, existing.size_, fonts);
}

gfx::Font LookAndFeel::GetFont(std::shared_ptr<xlib::XGraphics> gc,
                               uint16_t size,
                               gfx::Font existing,
                               FontCache* fonts) {
  return AllocateFont(gc, existing.font_name_, size, fonts);
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