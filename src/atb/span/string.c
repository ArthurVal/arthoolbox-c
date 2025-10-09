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
