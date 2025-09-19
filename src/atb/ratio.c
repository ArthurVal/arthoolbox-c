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

  return a;
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

    success = atb_Mul_Safely_u32(lhs->den, rhs->den, &(den)) &&
              atb_Mul_Safely_u32(lhs->num, rhs->den, &(lhs_num)) &&
              atb_Mul_Safely_u32(rhs->num, lhs->den, &(rhs_num));

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

bool atb_Ratio_Apply_u64(struct atb_Ratio ratio, uint64_t value,
                         uint64_t *const dest) {
  assert(ratio.den != 0);
  assert(dest != NULL);

  bool success = true;

  if (atb_Mul_Safely_u64(value, ratio.num, &(value))) {
    value /= ratio.den;
  } else {
    success = atb_Mul_Safely_u64(value / ratio.den, ratio.num, &(value));
  }

  if (success) {
    *dest = value;
  }

  return success;
}

bool atb_Ratio_Apply_u32(struct atb_Ratio ratio, uint32_t value,
                         uint32_t *const dest) {
  assert(ratio.den != 0);
  assert(dest != NULL);

  bool success = true;

  if (atb_Mul_Safely_u32(value, ratio.num, &(value))) {
    value /= ratio.den;
  } else {
    success = atb_Mul_Safely_u32(value / ratio.den, ratio.num, &(value));
  }

  if (success) {
    *dest = value;
  }

  return success;
}

bool atb_Ratio_Apply_u16(struct atb_Ratio ratio, uint16_t value,
                         uint16_t *const dest) {
  bool success = atb_Ratio_Apply_u32(ratio, value, &(ratio.num)) &&
                 (ratio.num <= UINT16_MAX);

  if (success) {
    *dest = (uint16_t)ratio.num;
  }

  return success;
}

bool atb_Ratio_Apply_u8(struct atb_Ratio ratio, uint8_t value,
                        uint8_t *const dest) {
  bool success = atb_Ratio_Apply_u32(ratio, value, &(ratio.num)) &&
                 (ratio.num <= UINT8_MAX);

  if (success) {
    *dest = (uint8_t)ratio.num;
  }

  return success;
}

bool atb_Ratio_Inv(struct atb_Ratio ratio, struct atb_Ratio *const dest) {
  assert(dest != NULL);

  dest->num = ratio.den;
  dest->den = ratio.num;

  return true;
}

bool atb_Ratio_Reduce(struct atb_Ratio ratio, struct atb_Ratio *const dest) {
  assert(dest != NULL);

  if (ratio.num != 0 && ratio.den != 0) {
    const atb_Ratio_elem_t gcd = Ratio_FindGCD(ratio.num, ratio.den);
    ratio.num /= gcd;
    ratio.den /= gcd;
  }

  *dest = ratio;
  return true;
}

bool atb_Ratio_Add(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  assert(dest != NULL);

  /* We use LHS to store the result and then assign to dest if it succeeded */
  bool success = Ratio_SameDenominator(&lhs, &rhs) &&
                 atb_Add_Safely_u32(lhs.num, rhs.num, &(lhs.num));

  /* Only assign if it succeeded */
  if (success) {
    *dest = lhs;
  }

  return success;
}

bool atb_Ratio_Sub(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  assert(dest != NULL);

  /* We use LHS to store the result and then assign to dest if it succeeded */
  bool success = Ratio_SameDenominator(&lhs, &rhs) &&
                 atb_Sub_Safely_u32(lhs.num, rhs.num, &(lhs.num));

  /* Only assign if it succeeded */
  if (success) {
    *dest = lhs;
  }

  return success;
}

bool atb_Ratio_Mul(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  assert(dest != NULL);

  bool success = atb_Mul_Safely_u32(lhs.num, rhs.num, &(lhs.num)) &&
                 atb_Mul_Safely_u32(lhs.den, rhs.den, &(lhs.den));

  if (success) {
    *dest = lhs;
  }

  return success;
}

bool atb_Ratio_Div(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) {
  return atb_Ratio_Inv(rhs, &rhs) && atb_Ratio_Mul(lhs, rhs, dest);
}

bool atb_Ratio_Compare(struct atb_Ratio lhs, struct atb_Ratio rhs,
                       int *const dest) {
  assert(dest != NULL);

  bool success = (atb_Ratio_Div(lhs, rhs, &lhs) ||
                  (atb_Ratio_Reduce(lhs, &lhs) && atb_Ratio_Reduce(rhs, &rhs) &&
                   atb_Ratio_Div(lhs, rhs, &lhs)));

  if (success) {
    if (lhs.num == lhs.den) {
      *dest = K_ATB_RATIO_CMP_EQUAL;
    } else if (lhs.num > lhs.den) {
      *dest = K_ATB_RATIO_CMP_GREATER;
    } else {
      *dest = K_ATB_RATIO_CMP_LESS;
    }
  }

  return success;
}
