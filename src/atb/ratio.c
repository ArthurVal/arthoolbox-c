#include "atb/ratio.h"

#include <assert.h>
#include <stdlib.h>

#include "atb/ints.h"

static atb_Ratio_elem_t Ratio_FindGCD(atb_Ratio_elem_t a, atb_Ratio_elem_t b) {
  atb_Ratio_elem_t tmp = 0;

  /* Euclid 's algorithm to find the GCD between a and b */
  /* https://en.wikipedia.org/wiki/Euclidean_algorithm */

  /* Here 'a' will be used to holds the value of the computed GCD... */
  while (b != 0) {
    tmp = b;
    b = a % b;
    a = tmp;
  }

  return abs(a);
}

static bool Ratio_FixSign(struct atb_Ratio *const ratio) {
  assert(ratio != NULL);

  struct atb_Ratio tmp;
  bool success = true;

  if (ratio->den < 0) {
    success = atb_Mul_Safely_i32(ratio->num, -1, &(tmp.num)) &&
              atb_Mul_Safely_i32(ratio->den, -1, &(tmp.den));

    if (success) {
      *ratio = tmp;
    }
  }

  return success;
}

static bool Ratio_SameDenominator(struct atb_Ratio *const lhs,
                                  struct atb_Ratio *const rhs) {
  assert(lhs != NULL);
  assert(rhs != NULL);

  bool success = true;

  if (lhs->den != rhs->den) {
    atb_Ratio_elem_t lhs_num;
    atb_Ratio_elem_t rhs_num;
    atb_Ratio_elem_t den;

    success = atb_Mul_Safely_i32(lhs->den, rhs->den, &(den)) &&
              atb_Mul_Safely_i32(lhs->num, rhs->den, &(lhs_num)) &&
              atb_Mul_Safely_i32(rhs->num, lhs->den, &(rhs_num));

    if (success) {
      lhs->num = lhs_num;
      lhs->den = den;

      rhs->num = rhs_num;
      rhs->den = den;
    }
  }

  return success;
}

float atb_Ratio_Tof32(struct atb_Ratio ratio) {
  assert(ratio.den != 0);
  return (float)ratio.num / (float)ratio.den;
}

double atb_Ratio_Tof64(struct atb_Ratio ratio) {
  assert(ratio.den != 0);
  return (double)ratio.num / (double)ratio.den;
}

bool atb_Ratio_Inv(struct atb_Ratio ratio, struct atb_Ratio *const dest) {
  assert(dest != NULL);

  dest->num = ratio.den;
  dest->den = ratio.num;

  return true;
}

struct atb_Ratio atb_Ratio_Reduce(struct atb_Ratio ratio) {
  if (ratio.num != 0 && ratio.den != 0) {
    const atb_Ratio_elem_t gcd = Ratio_FindGCD(ratio.num, ratio.den);
    ratio.num /= gcd;
    ratio.den /= gcd;
  }

  return ratio;
}

bool atb_Ratio_Add(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  /* We use LHS to store the result and then assign to dest if it succeeded */
  bool success = Ratio_SameDenominator(&lhs, &rhs) &&
                 atb_Add_Safely_i32(lhs.num, rhs.num, &(lhs.num));

  /* Only assign if it succeeded */
  if (success && (dest != NULL)) {
    *dest = lhs;
  }

  return success;
}

bool atb_Ratio_Sub(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  /* We use LHS to store the result and then assign to dest if it succeeded */
  bool success = Ratio_SameDenominator(&lhs, &rhs) &&
                 atb_Sub_Safely_i32(lhs.num, rhs.num, &(lhs.num));

  /* Only assign if it succeeded */
  if (success && (dest != NULL)) {
    *dest = lhs;
  }

  return success;
}

bool atb_Ratio_Mul(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  bool success = atb_Mul_Safely_i32(lhs.num, rhs.num, &(lhs.num)) &&
                 atb_Mul_Safely_i32(lhs.den, rhs.den, &(lhs.den));

  if (success && (dest != NULL)) {
    *dest = lhs;
  }

  return success;
}

bool atb_Ratio_Div(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  return atb_Ratio_Inv(rhs, &rhs) && atb_Ratio_Mul(lhs, rhs, dest);
}

bool atb_Ratio_Compare(struct atb_Ratio lhs, struct atb_Ratio rhs,
                       atb_Ratio_Compare_Result *const dest) {
  bool success =
      Ratio_FixSign(&lhs) && Ratio_FixSign(&rhs) &&
      (atb_Ratio_Div(lhs, rhs, &lhs) ||
       atb_Ratio_Div(atb_Ratio_Reduce(lhs), atb_Ratio_Reduce(rhs), &lhs));

  if (success) {
    if (lhs.num == lhs.den) {
      *dest = atb_Ratio_Compare_EQUAL;
    } else if (lhs.num > lhs.den) {
      *dest = atb_Ratio_Compare_GREATER;
    } else {
      *dest = atb_Ratio_Compare_LESS;
    }
  }

  return success;
}

bool atb_Ratio_Eq(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  atb_Ratio_Compare_Result cmp_res;
  if (!atb_Ratio_Compare(lhs, rhs, &cmp_res)) {
    return false;
  }

  switch (cmp_res) {
    case atb_Ratio_Compare_EQUAL:
      return true;
    default:
      return false;
  }
}

bool atb_Ratio_Ne(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  atb_Ratio_Compare_Result cmp_res;
  if (!atb_Ratio_Compare(lhs, rhs, &cmp_res)) {
    return false;
  }

  switch (cmp_res) {
    case atb_Ratio_Compare_GREATER:
    case atb_Ratio_Compare_LESS:
      return true;
    default:
      return false;
  }
}

bool atb_Ratio_Gt(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  atb_Ratio_Compare_Result cmp_res;
  if (!atb_Ratio_Compare(lhs, rhs, &cmp_res)) {
    return false;
  }

  switch (cmp_res) {
    case atb_Ratio_Compare_GREATER:
      return true;
    default:
      return false;
  }
}

bool atb_Ratio_Lt(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  atb_Ratio_Compare_Result cmp_res;
  if (!atb_Ratio_Compare(lhs, rhs, &cmp_res)) {
    return false;
  }

  switch (cmp_res) {
    case atb_Ratio_Compare_LESS:
      return true;
    default:
      return false;
  }
}

bool atb_Ratio_Ge(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  atb_Ratio_Compare_Result cmp_res;
  if (!atb_Ratio_Compare(lhs, rhs, &cmp_res)) {
    return false;
  }

  switch (cmp_res) {
    case atb_Ratio_Compare_EQUAL:
    case atb_Ratio_Compare_GREATER:
      return true;
    default:
      return false;
  }
}

bool atb_Ratio_Le(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  atb_Ratio_Compare_Result cmp_res;
  if (!atb_Ratio_Compare(lhs, rhs, &cmp_res)) {
    return false;
  }

  switch (cmp_res) {
    case atb_Ratio_Compare_EQUAL:
    case atb_Ratio_Compare_LESS:
      return true;
    default:
      return false;
  }
}
