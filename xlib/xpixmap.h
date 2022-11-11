#pragma once

#include <memory>

#include "xdisplay.h"
#include "xdrawable.h"
#include "xorg_typemap.h"

namespace xpp::xlib {

class XPixmap : public XDrawable {
 public:
  ~XPixmap() override;
  ::Pixmap operator*();
  ::Drawable Drawable() override;

 private:
  friend struct Traits<XPixmap>;
  XPixmap(std::shared_ptr<XDisplay>, ::Pixmap);

  ::Pixmap pixmap_;
};

}  // namespace xpp::xlib