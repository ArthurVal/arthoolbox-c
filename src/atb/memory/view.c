#include "atb/memory/view.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

bool atb_MemView_IsOverlapping(struct atb_MemView lhs, struct atb_MemView rhs) {
  assert(!atb_MemView_IsInvalid(lhs));
  assert(!atb_MemView_IsInvalid(rhs));

  /* View of size 0 never overlap anything */
  if ((lhs.size == 0) || (rhs.size == 0)) {
    return false;
  }

  /* .first == lo, .second == hi */
  const struct atb_MemViewPair sorted =
      atb_MemView_Sort(atb_MemView_IsLowerOrEqual, lhs, rhs);

  /*
   * We have the following diagram:
   *
   *                                end of lo
   *                                   |
   *                                   v
   * lo:      [------------------------[
   * hi:                          [-----------[
   *                              ^
   *                              |
   *                        begin of hi
   *
   * Since End is not part of the memory view (one past the valid range) we dont
   * check for equality (use > vs >=)
   */
  return atb_MemView_EndAs(char, sorted.first) >
         atb_MemView_BeginAs(char, sorted.second);
}

struct atb_MemView atb_MemView_Merge(struct atb_MemView lhs,
                                     struct atb_MemView rhs) {
  assert(!atb_MemView_IsInvalid(lhs));
  assert(!atb_MemView_IsInvalid(rhs));
  assert(atb_MemView_IsOverlapping(lhs, rhs));

  char const *const begin =
      MIN(atb_MemView_BeginAs(char, lhs), atb_MemView_BeginAs(char, rhs));

  char const *const end =
      MAX(atb_MemView_EndAs(char, lhs), atb_MemView_EndAs(char, rhs));

  return (struct atb_MemView){
      .data = (const void *)begin,
      .size = (size_t)(end - begin),
  };
}
