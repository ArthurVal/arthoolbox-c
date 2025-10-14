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

bool atb_StrView_StartsWith(struct atb_StrView str, struct atb_StrView prefix) {
  assert(atb_StrView_IsValid(str));
  assert(atb_StrView_IsValid(prefix));

  bool success = false;
  if ((prefix.size != 0) && (prefix.size <= str.size)) {
    success = atb_StrView_Eq(prefix, atb_StrView_First(str, prefix.size));
  }

  return success;
}

bool atb_StrView_EndsWith(struct atb_StrView str, struct atb_StrView suffix) {
  assert(atb_StrView_IsValid(str));
  assert(atb_StrView_IsValid(suffix));

  bool success = false;
  if ((suffix.size != 0) && (suffix.size <= str.size)) {
    success = atb_StrView_Eq(suffix, atb_StrView_Last(str, suffix.size));
  }

  return success;
}

bool atb_StrView_Find(struct atb_StrView str, struct atb_StrView substr,
                      size_t *const where) {
  assert(atb_StrView_IsValid(str));
  assert(atb_StrView_IsValid(substr));

  size_t initial_size = str.size;
  bool found = atb_StrView_StartsWith(str, substr);

  while (!found && (str.size > substr.size)) {
    str = atb_StrView_Shrink(str, 1, K_ATB_SPAN_SHRINK_FRONT);
    found = atb_StrView_StartsWith(str, substr);
  }

  if ((where != NULL) && found) {
    *where = (initial_size - str.size);
  }

  return found;
}

bool atb_StrView_RFind(struct atb_StrView str, struct atb_StrView substr,
                       size_t *const where) {
  assert(atb_StrView_IsValid(str));
  assert(atb_StrView_IsValid(substr));

  bool found = atb_StrView_EndsWith(str, substr);

  while (!found && (str.size > substr.size)) {
    str = atb_StrView_Shrink(str, 1, K_ATB_SPAN_SHRINK_BACK);
    found = atb_StrView_EndsWith(str, substr);
  }

  if ((where != NULL) && found) {
    *where = (str.size - substr.size);
  }

  return found;
}

bool atb_StrView_Find_FirstOf(struct atb_StrView str,
                              struct atb_StrView pattern, size_t *const where) {
  assert(atb_StrView_IsValid(str));
  assert(atb_StrView_IsValid(pattern));

  const char *first_found = NULL;
  const char *ch_found = NULL;

  for (const char *ch = atb_AnySpan_Begin(pattern);
       ch != atb_AnySpan_End(pattern); ++ch) {
    ch_found = (const char *)memchr(str.data, *ch, str.size);

    if ((ch_found != NULL) &&
        ((first_found == NULL) || (first_found > ch_found))) {
      first_found = ch_found;
    }
  }

  if ((where != NULL) && (first_found != NULL)) {
    *where = (size_t)(first_found - str.data);
  }

  return (first_found != NULL);
}
