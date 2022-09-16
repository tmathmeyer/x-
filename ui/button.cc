#include "button.h"

namespace xpp::ui {

class ButtonEventListener : public xpp::ui::MouseMotionListener,
                            public xpp::ui::MouseListener {
 public:
  ButtonEventListener(XButton* button) { button_ = button; }

  void MouseEntered(xpp::ui::MouseMotionEvent event) {
    event.active = false;
    button_->SetHovered(true);
    button_->Repaint();
  }

  void MouseExited(xpp::ui::MouseMotionEvent event) {
    event.active = false;
    button_->SetHovered(false);
    button_->Repaint();
  }

  void MouseMoved(xpp::ui::MouseMotionEvent) { puts("MM"); }

  void MouseDragged(xpp::ui::MouseMotionEvent) { puts("MouseDragged!"); }
  void MouseClicked(xpp::ui::MouseEvent) { puts("MouseClick"); }
  void MousePressed(xpp::ui::MouseEvent) { puts("MousePress"); }
  void MouseReleased(xpp::ui::MouseEvent) { puts("MouseRelease"); }

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
  if (hovered_)
    g->SetColor("ButtonActiveBackground");
  else
    g->SetColor("ButtonBackground");

  g->FillRoundedRect({0, 0}, g->GetDimensions(), 4);
  g->SetColor("ButtonBorder");
  g->DrawRoundedRect({0, 0}, g->GetDimensions(), 4);
  g->SetColor("TextColor");
  g->DrawText({0, (100 - 70) / 2}, content_);
}

std::optional<gfx::Rect> XButton::GetPreferredSize() const {
  return gfx::Rect(content_.length() * 24, 100);
}

void XButton::SetHovered(bool hovered) {
  hovered_ = hovered;
}

}  // namespace xpp::ui