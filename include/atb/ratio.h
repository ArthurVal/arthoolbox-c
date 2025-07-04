#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <assert.h>   /* static_assert */
#include <inttypes.h> /* imaxabs */
#include <stdbool.h>
#include <stdint.h>

/// Underlying integer type used within a atb_Ratio
typedef int32_t atb_Ratio_elem_t;

/**
 *  \brief Contains a ratio reprensted by 2 integers
 */
struct atb_Ratio {
  atb_Ratio_elem_t num; /*!< The numerator of the ratio */
  atb_Ratio_elem_t den; /*!< The denumerator of the ratio */
};

typedef struct atb_Ratio atb_Ratio;

/* Format string helper ****************************************************/
#define atb_Ratio_FMT "{.num=%jd, .den=%jd}"
#define atb_Ratio_FMT_VA_ARG(ratio) (intmax_t)(ratio).num, (intmax_t)(ratio).den

/* Conversion **************************************************************/

/**
 *  \return The closest floating point value (32 bits) represented by this ratio
 *  \pre ratio.den != 0
 */
static inline float atb_Ratio_Tof32(struct atb_Ratio ratio);

/**
 *  \return The closest floating point value (64 bits) represented by this ratio
 *  \pre ratio.den != 0
 */
static inline double atb_Ratio_Tof64(struct atb_Ratio ratio);

/* Operations **************************************************************/

/**
 *  \return The ratio correspondint to the inverse
 */
static inline struct atb_Ratio atb_Ratio_Inv(struct atb_Ratio ratio);

/**
 *  \return The addition of the 2 lhs/rhs ratios
 */
static inline struct atb_Ratio atb_Ratio_Add(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs);

/**
 *  \return The substraction of the 2 lhs/rhs ratios
 */
static inline struct atb_Ratio atb_Ratio_Sub(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs);

/**
 *  \return The multiplication of the 2 lhs/rhs ratios
 */
static inline struct atb_Ratio atb_Ratio_Mul(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs);

/**
 *  \return The division of the 2 lhs/rhs ratios
 */
static inline struct atb_Ratio atb_Ratio_Div(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs);

/**
 *  \return The reduced fraction using Euclid's algorithm to find the GCD
 *  \note When either num or den are equal to 0, return the same ratio
 */
static inline struct atb_Ratio atb_Ratio_Reduce(struct atb_Ratio ratio);

/**
 *  \brief Normalize the sign of the ratio by moving it from the denominator to
 *         the numerator when relevant
 *
 *  \return A ratio with it's denominator always be POSITIVE
 */
static inline struct atb_Ratio atb_Ratio_FixSign(struct atb_Ratio ratio);

/* Comparison **************************************************************/

/**
 *  \brief Enum representing the result of the _Compare function
 */
typedef enum {
  atb_Ratio_Compare_LESS,    /*!< LHS is LESS than RHS */
  atb_Ratio_Compare_EQUAL,   /*!< LHS is EQUAL to RHS */
  atb_Ratio_Compare_GREATER, /*!< LHS is GREATER than RHS */
} atb_Ratio_Compare_Result;

/**
 *  \brief Compare 2 ratios with each other
 */
static inline atb_Ratio_Compare_Result atb_Ratio_Compare(struct atb_Ratio lhs,
                                                         struct atb_Ratio rhs);

static inline bool atb_Ratio_Eq(struct atb_Ratio lhs, struct atb_Ratio rhs);
static inline bool atb_Ratio_Ne(struct atb_Ratio lhs, struct atb_Ratio rhs);
static inline bool atb_Ratio_Gt(struct atb_Ratio lhs, struct atb_Ratio rhs);
static inline bool atb_Ratio_Lt(struct atb_Ratio lhs, struct atb_Ratio rhs);
static inline bool atb_Ratio_Ge(struct atb_Ratio lhs, struct atb_Ratio rhs);
static inline bool atb_Ratio_Le(struct atb_Ratio lhs, struct atb_Ratio rhs);

/* Constant ****************************************************************/
#define atb_internal_DECLARE_RATIO(value, name, inv_name) \
  static inline struct atb_Ratio atb_##name(void) {       \
    struct atb_Ratio res;                                 \
    res.num = (value);                                    \
    res.den = 1;                                          \
    return res;                                           \
  }                                                       \
                                                          \
  static inline struct atb_Ratio atb_##inv_name(void) {   \
    return atb_Ratio_Inv(atb_##name());                   \
  }                                                       \
                                                          \
  static_assert(true, "")

/* Require 64 bits */
/* atb_internal_DECLARE_RATIO(1e15, peta, fempto); */
/* atb_internal_DECLARE_RATIO(1e12, tera, pico); */

atb_internal_DECLARE_RATIO(1e9, giga, nano);
atb_internal_DECLARE_RATIO(1e6, mega, micro);
atb_internal_DECLARE_RATIO(1e3, kilo, milli);
atb_internal_DECLARE_RATIO(1e2, hecto, centi);
atb_internal_DECLARE_RATIO(1e1, deca, deci);

#undef atb_internal_DECLARE_RATIO

static inline struct atb_Ratio atb_one(void) {
  struct atb_Ratio res;
  res.num = res.den = 1;
  return res;
}

/***************************************************************************/
/*                           Inline definitions                            */
/***************************************************************************/
static inline float atb_Ratio_Tof32(struct atb_Ratio ratio) {
  assert(ratio.den != 0);
  return (float)ratio.num / (float)ratio.den;
}

static inline double atb_Ratio_Tof64(struct atb_Ratio ratio) {
  assert(ratio.den != 0);
  return (double)ratio.num / (double)ratio.den;
}

static inline struct atb_Ratio atb_Ratio_Inv(struct atb_Ratio ratio) {
  /* Just swap num and den ... */
  const atb_Ratio_elem_t tmp = ratio.num;
  ratio.num = ratio.den;
  ratio.den = tmp;

  return ratio;
}

static inline struct atb_Ratio atb_Ratio_Add(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs) {
  if (lhs.den != rhs.den) {
    lhs.num = (lhs.num * rhs.den) + (rhs.num * lhs.den);
    lhs.den *= rhs.den;
  } else {
    lhs.num += rhs.num;
  }

  return lhs;
}

static inline struct atb_Ratio atb_Ratio_Sub(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs) {
  if (lhs.den != rhs.den) {
    lhs.num = (lhs.num * rhs.den) - (rhs.num * lhs.den);
    lhs.den *= rhs.den;
  } else {
    lhs.num -= rhs.num;
  }

  return lhs;
}

static inline struct atb_Ratio atb_Ratio_Mul(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs) {
  lhs.num *= rhs.num;
  lhs.den *= rhs.den;
  return lhs;
}

static inline struct atb_Ratio atb_Ratio_Div(struct atb_Ratio lhs,
                                             struct atb_Ratio rhs) {
  return atb_Ratio_Mul(lhs, atb_Ratio_Inv(rhs));
}

static inline struct atb_Ratio atb_Ratio_Reduce(struct atb_Ratio ratio) {
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

static inline struct atb_Ratio atb_Ratio_FixSign(struct atb_Ratio ratio) {
  if (ratio.den < 0) {
    ratio.num *= -1;
    ratio.den *= -1;
  }

  return ratio;
}

static inline atb_Ratio_Compare_Result atb_Ratio_Compare(struct atb_Ratio lhs,
                                                         struct atb_Ratio rhs) {
  const atb_Ratio div =
      atb_Ratio_Div(atb_Ratio_FixSign(lhs), atb_Ratio_FixSign(rhs));

  if (div.num == div.den) {
    return atb_Ratio_Compare_EQUAL;
  } else if (div.num > div.den) {
    return atb_Ratio_Compare_GREATER;
  } else {
    return atb_Ratio_Compare_LESS;
  }
}

static inline bool atb_Ratio_Eq(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  return atb_Ratio_Compare(lhs, rhs) == atb_Ratio_Compare_EQUAL;
}

static inline bool atb_Ratio_Ne(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  return !atb_Ratio_Eq(lhs, rhs);
}

static inline bool atb_Ratio_Gt(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_GREATER: return true;
    default: return false;
  }
}

static inline bool atb_Ratio_Lt(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_LESS: return true;
    default: return false;
  }
}

static inline bool atb_Ratio_Ge(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_EQUAL:
    case atb_Ratio_Compare_GREATER: return true;
    default: return false;
  }
}

static inline bool atb_Ratio_Le(struct atb_Ratio lhs, struct atb_Ratio rhs) {
  switch (atb_Ratio_Compare(lhs, rhs)) {
    case atb_Ratio_Compare_EQUAL:
    case atb_Ratio_Compare_LESS: return true;
    default: return false;
  }
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
