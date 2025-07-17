#include "atb/ratio.h"

#include <assert.h>

float atb_Ratio_Tof32(struct atb_Ratio ratio) {
  assert(ratio.den != 0);
  return (float)ratio.num / (float)ratio.den;
}

double atb_Ratio_Tof64(struct atb_Ratio ratio) {
  assert(ratio.den != 0);
  return (double)ratio.num / (double)ratio.den;
}

struct atb_Ratio atb_Ratio_Inv(struct atb_Ratio ratio) {
  /* Just swap num and den ... */
  const atb_Ratio_elem_t tmp = ratio.num;
  ratio.num = ratio.den;
  ratio.den = tmp;

  return ratio;
}

struct atb_Ratio atb_Ratio_Add(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  if (lhs.den != rhs.den) {
    lhs.num = (lhs.num * rhs.den) + (rhs.num * lhs.den);
    lhs.den *= rhs.den;
  } else {
    lhs.num += rhs.num;
  }

  return lhs;
}

struct atb_Ratio atb_Ratio_Sub(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  if (lhs.den != rhs.den) {
    lhs.num = (lhs.num * rhs.den) - (rhs.num * lhs.den);
    lhs.den *= rhs.den;
  } else {
    lhs.num -= rhs.num;
  }

  return lhs;
}

struct atb_Ratio atb_Ratio_Mul(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  lhs.num *= rhs.num;
  lhs.den *= rhs.den;
  return lhs;
}

struct atb_Ratio atb_Ratio_Div(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  return atb_Ratio_Mul(lhs, atb_Ratio_Inv(rhs));
}

struct atb_Ratio atb_Ratio_Reduce(struct atb_Ratio ratio) {
  if (ratio.num == 0 || ratio.den == 0) return ratio;

  struct atb_Ratio reduced_ratio = ratio;

  /* Euclid 's algorithm to find the GCD */
  /* Here 'ratio.num' will be used to holds the value of the computed GCD... */
  atb_Ratio_elem_t tmp = 0;
  while (ratio.den != 0) {
    tmp = ratio.den;
    ratio.den = ratio.num % ratio.den;
    ratio.num = tmp;
  }

  /* ratio.num == GCD */
  reduced_ratio.num /= ratio.num;
  reduced_ratio.den /= ratio.num;

  return reduced_ratio;
}

struct atb_Ratio atb_Ratio_FixSign(struct atb_Ratio ratio) {
  if (ratio.den < 0) {
    ratio.num *= -1;
    ratio.den *= -1;
  }

  return ratio;
}

atb_Ratio_Compare_Result atb_Ratio_Compare(struct atb_Ratio lhs,
                                           struct atb_Ratio rhs) {
  const struct atb_Ratio div =
      atb_Ratio_Div(atb_Ratio_FixSign(lhs), atb_Ratio_FixSign(rhs));

  if (div.num == div.den) {
    return atb_Ratio_Compare_EQUAL;
  } else if (div.num > div.den) {
    return atb_Ratio_Compare_GREATER;
  } else {
    return atb_Ratio_Compare_LESS;
  }
}

bool atb_Ratio_Eq(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  return atb_Ratio_Compare(lhs, rhs) == atb_Ratio_Compare_EQUAL;
}

bool atb_Ratio_Ne(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  return !atb_Ratio_Eq(lhs, rhs);
}

bool atb_Ratio_Gt(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_GREATER: return true;
    default: return false;
  }
}

bool atb_Ratio_Lt(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_LESS: return true;
    default: return false;
  }
}

bool atb_Ratio_Ge(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_EQUAL:
    case atb_Ratio_Compare_GREATER: return true;
    default: return false;
  }
}

bool atb_Ratio_Le(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_EQUAL:
    case atb_Ratio_Compare_LESS: return true;
    default: return false;
  }
}
