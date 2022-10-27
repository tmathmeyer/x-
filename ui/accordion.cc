

#include "accordion.h"

#include "button_listener.h"

namespace xpp::ui {

namespace {

class AccordionLayout : public Layout {
 public:
  AccordionLayout(XAccordion* parent) : parent_(parent) {}

  std::vector<xpp::ui::Layout::Position> DoLayout(
      std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
      gfx::Rect size) override {
    std::vector<xpp::ui::Layout::Position> positions;
    gfx::Coord tlc = {0, 0};
    XComponent* body = nullptr;
    for (const auto& tagged : entries) {
      const auto& comp = std::get<0>(tagged);
      switch (static_cast<XAccordion::ComponentUsage>(std::get<1>(tagged))) {
        case XAccordion::ComponentUsage::kTitle: {
          auto height = *comp->GetPreferredHeight();
          positions.push_back(
              {comp.get(), {0, 0}, {size.width, height}});
          tlc = {0, *comp->GetPreferredHeight()};
          break;
        }
        case XAccordion::ComponentUsage::kBody: {
          body = comp.get();
          break;
        }
      }
    }
    if (body && parent_->IsOpen()) {
      gfx::Rect leftover = {size.width, std::max(0l, size.height - tlc.y)};
      auto body_size = body->GetPreferredSize().value_or(leftover);
      positions.push_back({body, tlc, body_size});
    }
    return positions;
  }

 private:
  XAccordion* parent_;
};

class AccordionTitle : public XPanel {
 public:
  AccordionTitle(XAccordion* parent, std::string text, uint32_t size)
      : parent_(parent), text_(text), size_(size) {
    std::shared_ptr<ButtonListener<AccordionTitle>> button_action =
        std::make_shared<ButtonListener<AccordionTitle>>(this);

    this->AddMouseMotionListener(button_action);
    this->AddMouseListener(button_action);
  }

  void Enter() {}

  void Exit() {}

  void Press() {}

  void Release() { parent_->ToggleState(); }

  void Paint(xpp::ui::Graphics* g) override {
    if (parent_->IsOpen())
      g->SetColor("AccordionHeaderBackgroundColorOpen");
    else
      g->SetColor("AccordionHeaderBackgroundColorClosed");
    g->FillRect({0, 0}, g->GetDimensions());

    g->SetFontSize(size_);
    if (parent_->IsOpen())
      g->SetColor("AccordionHeaderTextColorOpen");
    else
      g->SetColor("AccordionHeaderTextColorClosed");
    g->DrawText({30, 20}, text_);
  }

  std::optional<gfx::Rect> GetPreferredSize() override {
    int32_t length = text_.length();
    return gfx::Rect{size_ * length * 2.42 + 30, size_ * 4 + 40};
  }

 private:
  XAccordion* parent_;
  std::string text_;
  uint32_t size_;
};

}  // namespace

XAccordion::XAccordion(std::string text) : XAccordion(text, 10) {}

XAccordion::XAccordion(std::string text, uint32_t fontsize) {
  auto title = std::make_unique<AccordionTitle>(this, text, fontsize);
  auto container = std::make_unique<XPanel>();

  title->SetParent(this);
  container->SetParent(this);
  container_ = container.get();
  title_ = title.get();

  components_.push_back(std::make_tuple(
      std::move(title), static_cast<int>(ComponentUsage::kTitle)));
  components_.push_back(std::make_tuple(
      std::move(container), static_cast<int>(ComponentUsage::kBody)));

  layout_ = std::make_unique<AccordionLayout>(this);
}

std::optional<uint32_t> XAccordion::GetPreferredHeight() {
  if (!open_)
    return title_->GetPreferredHeight();

  auto container_size = container_->CalculatePreferredSize();
  auto header_size = *(title_->GetPreferredHeight());
  return container_size.height + header_size;
}

void XAccordion::AddComponent(std::unique_ptr<XComponent> component,
                              int32_t key) {
  container_->AddComponent(std::move(component), key);
}

void XAccordion::AddComponent(std::unique_ptr<XComponent> component) {
  container_->AddComponent(std::move(component));
}

void XAccordion::RemoveComponent(XComponent* component) {
  container_->RemoveComponent(component);
}

void XAccordion::RemoveAll() {
  container_->RemoveAll();
}

void XAccordion::SetLayout(std::unique_ptr<Layout> layout) {
  container_->SetLayout(std::move(layout));
}

void XAccordion::AddComponentListener(
    std::shared_ptr<ContainerListener> listener) {
  container_->AddComponentListener(std::move(listener));
}

void XAccordion::ToggleState() {
  open_ = !open_;
  Repaint();
}

bool XAccordion::IsOpen() {
  return open_;
}

}  // namespace xpp::ui