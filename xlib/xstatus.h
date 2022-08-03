#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "base/status/status.h"

namespace xpp::xlib {

struct XStatusImpl {
  enum class Codes {
    kSuccess = Success,
    kBadRequest = BadRequest,
    kBadValue = BadValue,
    kBadWindow = BadWindow,
    kBadPixmap = BadPixmap,
    kBadAtom = BadAtom,
    kBadCursor = BadCursor,
    kBadFont = BadFont,
    kBadMatch = BadMatch,
    kBadDrawable = BadDrawable,
    kBadAccess = BadAccess,
    kBadAlloc = BadAlloc,
    kBadColor = BadColor,
    kBadGC = BadGC,
    kBadIDChoice = BadIDChoice,
    kBadName = BadName,
    kBadLength = BadLength,
    kBadImplementation = BadImplementation,
    kExtension = FirstExtensionError,
  };
  static constexpr base::StatusGroupType Group() { return "XorgError"; }
  static constexpr Codes DefaultEnumValue() { return Codes::kSuccess; }
};

using XStatus = base::TypedStatus<XStatusImpl>;

}  // namespace xpp::xlib