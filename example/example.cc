#include <sstream>
#include "../ui/button.h"
#include "../ui/panel.h"
#include "../ui/scroll_panel.h"
#include "../ui/accordion.h"
#include "../ui/window.h"

class EventPrinter : public xpp::ui::MouseMotionListener,
                     public xpp::ui::MouseListener,
                     public xpp::ui::MouseWheelListener {
 public:
  void MouseEntered(xpp::ui::MouseMotionEvent*) {
    // puts("MouseEntered!");
  }
  void MouseExited(xpp::ui::MouseMotionEvent*) {
    // puts("MouseExited!");
  }
  void MouseMoved(xpp::ui::MouseMotionEvent*) {
    // puts("MouseMoved!");
  }
  void MouseDragged(xpp::ui::MouseMotionEvent*) {
    // puts("MouseDragged!");
  }
  void MouseClicked(xpp::ui::MouseEvent*) {
    // puts("MouseClick");
  }
  void MousePressed(xpp::ui::MouseEvent*) {
    // puts("MousePress");
  }
  void MouseReleased(xpp::ui::MouseEvent*) {
    // puts("MouseRelease");
  }
  void WheelScrolled(xpp::ui::MouseWheelEvent*) {
    // puts("WheelScroll");
  }
};

class PurplePanel : public xpp::ui::XPanel {
 public:
  PurplePanel() : xpp::ui::XPanel() {
    std::shared_ptr<EventPrinter> printer = std::make_shared<EventPrinter>();
    this->AddMouseMotionListener(printer);
    this->AddMouseListener(printer);
    this->AddMouseWheelListener(printer);
    for (int i = 0; i < 20; i++) {
      std::stringstream ss;
      ss << "Very Long Text Button #" << i;
      this->AddComponent(std::make_unique<xpp::ui::XButton>(ss.str()));
    }
  }

  void Paint(xpp::ui::Graphics* g) override { xpp::ui::XPanel::Paint(g); }
};

int main() {
  auto window = xpp::ui::XWindow::Create();
  auto default_component = std::make_unique<xpp::ui::XAccordion>("Click Me");
  auto scrollpanel = std::make_unique<xpp::ui::XScrollPanel>();
  for (int i = 0; i < 20; i++) {
    std::stringstream ss;
    ss << "Very Long Text Button #" << i;
    scrollpanel->AddComponent(
        std::make_unique<xpp::ui::XButton>(ss.str()));
  }
  default_component->AddComponent(std::move(scrollpanel));
  window->AddComponent(std::move(default_component));
  window->SetVisible(true);
}