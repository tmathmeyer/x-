#pragma once

#include "layout.h"

namespace xpp::ui {

class FillLayout : public Layout {
 public:
  virtual std::vector<xpp::ui::Layout::Position> DoLayout(
      std::vector<std::unique_ptr<XComponent>>& entries,
      gfx::Rect size) override;
};

}  // namespace xpp::ui