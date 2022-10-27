#pragma once

#include "component.h"

namespace xpp::ui {

class XButton : public XComponent {
 public:
  XButton(std::string text);
  void Paint(xpp::ui::Graphics* g) override;
  std::optional<gfx::Rect> GetPreferredSize() override;

  void Enter();
  void Exit();
  void Press();
  void Release();
 
 private:
  std::string content_;

  bool depressed_ = false;
  bool hovered_ = false;

  uint32_t hpadding_ = 40;
  uint32_t vshadow_ = 2;
};

}  // namespace xpp::ui