#include "button.h"

namespace xpp::ui {

class ButtonEventListener : public MouseMotionListener, public MouseListener {
 public:
  ButtonEventListener(XButton* button) { button_ = button; }

  void MouseEntered(MouseMotionEvent* event) {
    event->active = false;
    button_->Enter();
    button_->Repaint();
  }

  void MouseExited(MouseMotionEvent* event) {
    event->active = false;
    button_->Exit();
    button_->Repaint();
  }

  void MousePressed(MouseEvent* event) {
    event->active = false;
    button_->Press();
    button_->Repaint();
  }

  void MouseReleased(MouseEvent* event) {
    event->active = false;
    button_->Release();
    button_->Repaint();
  }

  void MouseMoved(MouseMotionEvent*) {}
  void MouseDragged(MouseMotionEvent*) {}
  void MouseClicked(MouseEvent*) {}

 private:
  XButton* button_;
};

XButton::XButton(std::string content) : content_(content) {
  std::shared_ptr<ButtonEventListener> button_action =
      std::make_shared<ButtonEventListener>(this);

  this->AddMouseMotionListener(button_action);
  this->AddMouseListener(button_action);
}

void XButton::Paint(xpp::ui::Graphics* g) {
  XComponent::Paint(g);
  g->SetFontSize(8);

  const char* background_color = "ButtonBackground";
  const char* text_color = "ButtonTextColor";
  const char* border_color = "ButtonBorder";
  const char* shadow_color = "ButtonShadow";

  if (depressed_) {
    background_color = "ButtonPressedBackground";
    text_color = "ButtonPressedTextColor";
    border_color = "ButtonPressedBorder";
    shadow_color = "ButtonPressedShadow";
  } else if (hovered_) {
    background_color = "ButtonHoveredBackground";
    text_color = "ButtonHoveredTextColor";
    border_color = "ButtonHoveredBorder";
    shadow_color = "ButtonHoveredShadow";
  }

  g->SetColor(shadow_color);
  gfx::Rect shadow = {g->GetDimensions().width - vshadow_ * 2,
                      g->GetDimensions().height};
  g->FillRoundedRect({vshadow_, vshadow_}, shadow, 10);

  gfx::Rect insets = {g->GetDimensions().width, g->GetDimensions().height - vshadow_};
  g->SetColor(background_color);
  g->FillRoundedRect({0, 0}, insets, 10);

  g->SetColor(border_color);
  g->DrawRoundedRect({0, 0}, insets, 10);

  g->SetColor(text_color);
  auto height = g->GetDimensions().height;
  g->DrawText({hpadding_, (height - vshadow_ - g->GetFontHeight()) / 2}, content_);
}

std::optional<gfx::Rect> XButton::GetPreferredSize() const {
  return gfx::Rect(content_.length() * 22 + hpadding_ * 2, 70);
}

void XButton::Enter() {
  hovered_ = true;
  depressed_ = false;
}
void XButton::Exit() {
  hovered_ = false;
  depressed_ = false;
}
void XButton::Press() {
  hovered_ = true;
  depressed_ = true;
}
void XButton::Release() {
  hovered_ = true;
  depressed_ = false;
}

}  // namespace xpp::ui