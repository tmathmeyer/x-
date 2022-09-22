#pragma once

#include "container.h"
#include "look_and_feel.h"

#include "../xlib/xdisplay.h"
#include "../xlib/xwindow.h"

namespace xpp::ui {

class XWindow : public XContainer {
 public:
  ~XWindow();

  enum class WindowType {
    kDesktopBackdrop,
    kDesktopDock,
    kApplicationToolbar,
    kApplicationMenu,
    kUtilityWindow,
    kSplashScreen,
    kDialogPopup,
    kNormal,
  };

  enum class PositionPin {
    kTopLeft,
    kTopCenter,
    kTopRight,
    kMiddleRight,
    kBottomRight,
    kBottomCenter,
    kBottomLeft,
    kMiddleLeft,
    kDoesNotMatter = kTopLeft,
  };

  LookAndFeel* GetLookAndFeel() const;
  void SetVisible(bool visibility);
  static std::unique_ptr<XWindow> Create();
  static std::unique_ptr<XWindow> Create(WindowType,
                                         PositionPin,
                                         gfx::Rect,
                                         gfx::Coord);


  void Repaint() override;

 private:
  XWindow();
  void RunEventLoop();
  bool Initialize(WindowType mode,
                  PositionPin positioning,
                  gfx::Rect size,
                  gfx::Coord loc);

  bool is_shown_ = false;
  bool request_hide_flag_ = false;
  PositionPin position_pin_ = PositionPin::kDoesNotMatter;
  WindowType type_ = WindowType::kNormal;
  gfx::Coord preferred_position_ = {0, 0};
  uint32_t depth_ = 32;
  gfx::Rect dimensions_ = {0, 0};
  gfx::Rect exposed_to_ = {0, 0};
  gfx::Coord previous_mouse_location_ = {0, 0};
  gfx::Coord press_location_ = {0, 0};
  uint8_t mouse_button_ = false;

  std::shared_ptr<LookAndFeel> laf_;
  std::shared_ptr<xlib::XWindow> root_;
  std::shared_ptr<xlib::XWindow> window_;
  std::shared_ptr<xlib::XDisplay> display_;
  std::shared_ptr<xlib::XColorMap> colormap_;
  std::shared_ptr<xlib::XGraphics> window_gc_;
};

}  // namespace xpp::ui