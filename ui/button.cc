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
    puts("MOUSE EXIT");
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
  if (depressed_)
    g->SetColor("ButtonPressedBackground");
  else if (hovered_)
    g->SetColor("ButtonHoveredBackground");
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