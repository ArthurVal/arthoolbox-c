#include "atb/str_view.h"

#include <assert.h>
#include <string.h>

struct atb_StrView atb_StrView_MakeFromCString(const char *c_str, size_t max) {
  return (struct atb_StrView){
      .data = c_str,
      .size = (c_str != NULL ? strnlen(c_str, max) : 0),
  };
}

atb_StrView_Compare_Result atb_StrView_Compare(struct atb_StrView lhs,
                                               struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  if (lhs.size == rhs.size) {
    int cmp_res = memcmp(lhs.data, rhs.data, rhs.size);
    if (cmp_res < 0) {
      return atb_StrView_Compare_LESS;
    } else if (cmp_res > 0) {
      return atb_StrView_Compare_GREATER;
    } else {
      return atb_StrView_Compare_EQUAL;
    }
  } else {
    if (lhs.size < rhs.size) {
      return atb_StrView_Compare_LESS;
    } else {
      return atb_StrView_Compare_GREATER;
    }
  }
}

struct atb_StrView atb_StrView_Find(struct atb_StrView str,
                                    struct atb_StrView substr) {
  assert(!atb_StrView_IsCorrupted(str));
  assert(!atb_StrView_IsCorrupted(substr));

  /* substr is within the (0, str.size] interval */
  if ((0 < substr.size) && (substr.size <= str.size)) {
    const char *where = NULL;
    while ((where = memchr(str.data, substr.data[0], str.size)) != NULL) {
      str = atb_StrView_SliceFront(str, (size_t)(where - str.data));
      if (atb_StrView_Eq(substr, atb_StrView_Slice(str, 0, substr.size))) {
        return str;
      } else {
        str = atb_StrView_SliceFront(str, 1);
      }
    }
  }

  /* Here we didn't found the substr */
  return atb_StrView_SliceFront(str, str.size);
}

struct atb_StrView atb_StrView_FindR(struct atb_StrView str,
                                     struct atb_StrView substr) {
  assert(!atb_StrView_IsCorrupted(str));
  assert(!atb_StrView_IsCorrupted(substr));

  /* substr is within the (0, str.size] interval */
  if ((0 < substr.size) && (substr.size <= str.size)) {

    /*
     * We don't start @ BeginR, and offset the begin of the reverse iteration by
     * the size of the substr, since trying the match a str that's smaller to
     * the substr will always fail
     */
    for (const char *c = (atb_StrView_BeginR(str) - (substr.size - 1));
         c != atb_StrView_EndR(str); --c) {

      if (*c == substr.data[0]) {
        struct atb_StrView sliced_str =
          atb_StrView_SliceFront(str, (size_t)(c - str.data));

        if (atb_StrView_Eq(substr,
                           atb_StrView_Slice(sliced_str, 0, substr.size))) {
          return sliced_str;
        }
      }
    }
  }

  return atb_StrView_SliceFront(str, str.size);
}
