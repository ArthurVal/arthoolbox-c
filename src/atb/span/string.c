#include "atb/span/string.h"

ATB_SPAN_VIEW_DEFINE(, atb_StrSpan, atb_StrView, char);

struct atb_StrSpan atb_StrSpan_From_NullTerminated(char *other) {
  assert(other != NULL);
  return atb_StrSpan_From(other, strlen(other));
}

struct atb_StrView atb_StrView_From_NullTerminated(const char *other) {
  assert(other != NULL);
  return atb_StrView_From(other, strlen(other));
}

extern bool atb_StrView_StartsWith(struct atb_StrView str,
                                   struct atb_StrView prefix) {
  assert(atb_StrView_IsValid(str));
  assert(atb_StrView_IsValid(prefix));

  bool success = false;
  if ((prefix.size != 0) && (prefix.size <= str.size)) {
    success = atb_StrView_Eq(prefix, atb_StrView_First(str, prefix.size));
  }

  return success;
}
