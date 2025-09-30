#include "atb/string.h"

#include <assert.h>
#include <string.h>

#define MIN(a, b) (a) < (b) ? (a) : (b)

#define Str_Slice(data, size, begin, new_size) \
  do {                                         \
    begin = MIN(begin, size);                  \
    data += begin;                             \
    size = MIN(new_size, (size - begin));      \
  } while (0)

void atb_StrView_Set(struct atb_StrView *const view, const char *other,
                     size_t size) {
  assert(view != NULL);
  view->data = other;
  view->size = size;
}

void atb_StrView_Set_NullTerminated(struct atb_StrView *const view,
                                    const char *other) {
  atb_StrView_Set(view, other, strlen(other));
}

struct atb_StrView atb_StrView_Slice(struct atb_StrView view, size_t offset,
                                     size_t new_size) {
  assert(atb_StrView_IsValid(view));
  Str_Slice(view.data, view.size, offset, new_size);
  return view;
}

void atb_StrSpan_Set(struct atb_StrSpan *span, char *other, size_t size) {
  assert(span != NULL);
  span->data = other;
  span->size = size;
}

void atb_StrSpan_Set_NullTerminated(struct atb_StrSpan *view, char *other) {
  atb_StrSpan_Set(view, other, strlen(other));
}

struct atb_StrSpan atb_StrSpan_Slice(struct atb_StrSpan span, size_t offset,
                                     size_t new_size) {
  assert(atb_StrSpan_IsValid(span));
  Str_Slice(span.data, span.size, offset, new_size);
  return span;
}

bool atb_StrView_CopyInto(struct atb_StrView view, struct atb_StrSpan dest,
                          atb_StrView_CopyInto_Opt_t opt) {
  assert(atb_StrView_IsValid(view));
  assert(atb_StrSpan_IsValid(dest));

  bool success = true;

  if (opt.truncate) {
    view.size = MIN(view.size, dest.size);
  }

  if (view.size <= dest.size) {
    if (opt.overlap) {
      memmove(dest.data, view.data, view.size);
    } else {
      memcpy(dest.data, view.data, view.size);
    }
  } else {
    success = false;
  }

  return success;
}

extern int atb_StrView_Compare(struct atb_StrView lhs, struct atb_StrView rhs) {
  assert(atb_StrView_IsValid(lhs));
  assert(atb_StrView_IsValid(rhs));

  if (lhs.size > rhs.size) {
    return 1;
  } else if (lhs.size < rhs.size) {
    return -1;
  } else {
    return memcmp(lhs.data, rhs.data, lhs.size);
  }

  return 0;
}
