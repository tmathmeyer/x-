#pragma once

#include "container.h"

namespace xpp::ui {

class PanelLayout : public Layout {
  virtual std::vector<xpp::ui::Layout::Position> DoLayout(
      std::vector<std::unique_ptr<XComponent>>& entries,
      gfx::Rect size) override;
};

class XPanel : public XContainer {
 public:
  XPanel();
  void Paint(xpp::ui::Graphics* g) override;
};

}  // namespace xpp::ui