#include "../ui/panel.h"
#include "../ui/window.h"
#include "../ui/button.h"


class EventPrinter : public xpp::ui::MouseMotionListener,
                     public xpp::ui::MouseListener,
                     public xpp::ui::MouseWheelListener {
 public:
  void MouseEntered(xpp::ui::MouseMotionEvent*) {
    //puts("MouseEntered!");
  }
  void MouseExited(xpp::ui::MouseMotionEvent*) {
    //puts("MouseExited!");
  }
  void MouseMoved(xpp::ui::MouseMotionEvent*) {
    // puts("MouseMoved!");
  }
  void MouseDragged(xpp::ui::MouseMotionEvent*) {
    //puts("MouseDragged!");
  }
  void MouseClicked(xpp::ui::MouseEvent*) {
    //puts("MouseClick");
  }
  void MousePressed(xpp::ui::MouseEvent*) {
    //puts("MousePress");
  }
  void MouseReleased(xpp::ui::MouseEvent*) {
    //puts("MouseRelease");
  }
  void WheelScrolled(xpp::ui::MouseWheelEvent*) {
    //puts("WheelScroll");
  }
};

class PurplePanel : public xpp::ui::XPanel {
 public:
  PurplePanel() : xpp::ui::XPanel() {
    std::shared_ptr<EventPrinter> printer = std::make_shared<EventPrinter>();
    this->AddMouseMotionListener(printer);
    this->AddMouseListener(printer);
    this->AddMouseWheelListener(printer);
    this->AddComponent(std::make_unique<xpp::ui::XButton>("Click Me!"));
    this->AddComponent(std::make_unique<xpp::ui::XButton>("No, Me!"));
  }

  void Paint(xpp::ui::Graphics* g) override {
    xpp::ui::XPanel::Paint(g);
  }
};

int main() {
  auto window = xpp::ui::XWindow::Create();
  window->AddComponent(std::make_unique<PurplePanel>());
  window->SetVisible(true);
}