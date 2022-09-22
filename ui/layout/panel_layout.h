#pragma once

#include "layout.h"

namespace xpp::ui {

class PanelLayout : public Layout {
  virtual std::vector<xpp::ui::Layout::Position> DoLayout(
      std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
      gfx::Rect size) override;
};

}  // namespace xpp::ui