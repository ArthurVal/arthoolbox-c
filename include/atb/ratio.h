#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "atb/export.h"

/// Underlying integer type used within a atb_Ratio
typedef int32_t atb_Ratio_elem_t;

/**
 *  \brief Contains a ratio reprensted by 2 integers
 */
struct atb_Ratio {
  atb_Ratio_elem_t num; /*!< The numerator of the ratio */
  atb_Ratio_elem_t den; /*!< The denumerator of the ratio */
};

/* Format string helper ****************************************************/

#define atb_Ratio_FMT "{.num=%jd, .den=%jd}"
#define atb_Ratio_FMT_VA_ARG(ratio) (intmax_t)(ratio).num, (intmax_t)(ratio).den

/* Constant ****************************************************************/

/**
 * \return A ratio corresponding to n:1
 */
#define atb_Ratio_(n) \
  (struct atb_Ratio) { .num = (n), .den = 1, }

/**
 * \return A ratio corresponding to 1:n
 */
#define atb_Ratio_1_(n) \
  (struct atb_Ratio) { .num = 1, .den = (n), }

/* Requires 'atb_Ratio_elem_t' to be at least 8 bits */
#define atb_Ratio_0 atb_Ratio_(0)
#define atb_Ratio_1 atb_Ratio_(1)

#define atb_Ratio_DECA atb_Ratio_(10)
#define atb_Ratio_DECI atb_Ratio_1_(10)

#define atb_Ratio_HECTO atb_Ratio_(100)
#define atb_Ratio_CENTI atb_Ratio_1_(100)

/* Requires 'atb_Ratio_elem_t' to be at least 16 bits */
#define atb_Ratio_KILO atb_Ratio_(1000)
#define atb_Ratio_MILLI atb_Ratio_1_(1000)

/* Requires 'atb_Ratio_elem_t' to be at least 32 bits */
#define atb_Ratio_MEGA atb_Ratio_(1000000)
#define atb_Ratio_MICRO atb_Ratio_1_(1000000)

#define atb_Ratio_GIGA atb_Ratio_(1000000000)
#define atb_Ratio_NANO atb_Ratio_1_(1000000000)

/* Requires 'atb_Ratio_elem_t' to be at least 64 bits */
/* #define atb_Ratio_TERA atb_Ratio_(1000000000000) */
/* #define atb_Ratio_PICO atb_Ratio_1_(1000000000000) */

/* #define atb_Ratio_PERA atb_Ratio_(1000000000000000) */
/* #define atb_Ratio_FEMPTO atb_Ratio_1_(1000000000000000) */

/* Conversion **************************************************************/

/**
 *  \return The closest floating point value (32 bits) represented by this ratio
 *  \pre ratio.den != 0
 */
ATB_PUBLIC float atb_Ratio_Tof32(struct atb_Ratio ratio);

/**
 *  \return The closest floating point value (64 bits) represented by this ratio
 *  \pre ratio.den != 0
 */
ATB_PUBLIC double atb_Ratio_Tof64(struct atb_Ratio ratio);

/* Operations **************************************************************/

/**
 *  \brief Apply (multiply) the ratio to the given value
 *
 *  \param[in] value The value that will be multiply by the ratio
 *  \param[in] ratio The ratio we wish to apply
 *
 *  \warning 'ratio' SHOULDN'T have any side effects
 *
 *  \return The result of ((value) * (ratio).num / (ratio).den)
 */
#define atb_Ratio_APPLY(ratio, value) (((value) * (ratio).num) / (ratio).den)

/**
 *  \return The ratio correspondint to the inverse
 */
ATB_PUBLIC struct atb_Ratio atb_Ratio_Inv(struct atb_Ratio ratio);

/**
 *  \return The addition of the 2 lhs/rhs ratios
 */
ATB_PUBLIC struct atb_Ratio atb_Ratio_Add(struct atb_Ratio lhs,
                                          struct atb_Ratio rhs);

/**
 *  \return The substraction of the 2 lhs/rhs ratios
 */
ATB_PUBLIC struct atb_Ratio atb_Ratio_Sub(struct atb_Ratio lhs,
                                          struct atb_Ratio rhs);

/**
 *  \return The multiplication of the 2 lhs/rhs ratios
 */
ATB_PUBLIC struct atb_Ratio atb_Ratio_Mul(struct atb_Ratio lhs,
                                          struct atb_Ratio rhs);

/**
 *  \return The division of the 2 lhs/rhs ratios
 */
ATB_PUBLIC struct atb_Ratio atb_Ratio_Div(struct atb_Ratio lhs,
                                          struct atb_Ratio rhs);

/**
 *  \return The reduced fraction using Euclid's algorithm to find the GCD
 *  \note When either num or den are equal to 0, return the same ratio
 */
ATB_PUBLIC struct atb_Ratio atb_Ratio_Reduce(struct atb_Ratio ratio);

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
ATB_PUBLIC atb_Ratio_Compare_Result atb_Ratio_Compare(struct atb_Ratio lhs,
                                                      struct atb_Ratio rhs);

ATB_PUBLIC bool atb_Ratio_Eq(struct atb_Ratio lhs, struct atb_Ratio rhs);
ATB_PUBLIC bool atb_Ratio_Ne(struct atb_Ratio lhs, struct atb_Ratio rhs);
ATB_PUBLIC bool atb_Ratio_Gt(struct atb_Ratio lhs, struct atb_Ratio rhs);
ATB_PUBLIC bool atb_Ratio_Lt(struct atb_Ratio lhs, struct atb_Ratio rhs);
ATB_PUBLIC bool atb_Ratio_Ge(struct atb_Ratio lhs, struct atb_Ratio rhs);
ATB_PUBLIC bool atb_Ratio_Le(struct atb_Ratio lhs, struct atb_Ratio rhs);

#if defined(__cplusplus)
} /* extern "C" */
#endif
