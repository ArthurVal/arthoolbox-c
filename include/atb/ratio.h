#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "atb/compare.h"
#include "atb/export.h"

#if defined(__cplusplus)
extern "C" {
#endif

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

/// Initialize an atb_Ratio as n:1 (static initializer)
#define atb_Ratio_INIT_(n) \
  { .num = (n), .den = 1, }

/// Initialize an atb_Ratio as 1:n (static initializer)
#define atb_Ratio_INIT_1_(n) \
  { .num = 1, .den = (n), }

/* Requires 'atb_Ratio_elem_t' to be at least 8 bits */
#define atb_Ratio_INIT_0 atb_Ratio_INIT_(0)
#define atb_Ratio_INIT_1 atb_Ratio_INIT_(1)

#define atb_Ratio_INIT_DECA atb_Ratio_INIT_(10)
#define atb_Ratio_INIT_DECI atb_Ratio_INIT_1_(10)

#define atb_Ratio_INIT_HECTO atb_Ratio_INIT_(100)
#define atb_Ratio_INIT_CENTI atb_Ratio_INIT_1_(100)

/* Requires 'atb_Ratio_elem_t' to be at least 16 bits */
#define atb_Ratio_INIT_KILO atb_Ratio_INIT_(1000)
#define atb_Ratio_INIT_MILLI atb_Ratio_INIT_1_(1000)

/* Requires 'atb_Ratio_elem_t' to be at least 32 bits */
#define atb_Ratio_INIT_MEGA atb_Ratio_INIT_(1000000)
#define atb_Ratio_INIT_MICRO atb_Ratio_INIT_1_(1000000)

#define atb_Ratio_INIT_GIGA atb_Ratio_INIT_(1000000000)
#define atb_Ratio_INIT_NANO atb_Ratio_INIT_1_(1000000000)

/* Requires 'atb_Ratio_elem_t' to be at least 64 bits */
/* #define atb_Ratio_INIT_TERA atb_Ratio_INIT_(1000000000000) */
/* #define atb_Ratio_INIT_PICO atb_Ratio_INIT_1_(1000000000000) */

/* #define atb_Ratio_INIT_PERA atb_Ratio_INIT_(1000000000000000) */
/* #define atb_Ratio_INIT_FEMPTO atb_Ratio_INIT_1_(1000000000000000) */

/// Compound literal of a ratio corresponding to n:1
#define atb_Ratio_(n) (struct atb_Ratio) atb_Ratio_INIT_(n)

/// Compound literal of a ratio corresponding to 1:n
#define atb_Ratio_1_(n) (struct atb_Ratio) atb_Ratio_INIT_1_(n)

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

/**@{*/
/**
 *  \return The floating point value (32/64 bits) represented by this ratio
 *  \pre ratio.den != 0
 */
float atb_Ratio_Tof32(struct atb_Ratio ratio) ATB_PUBLIC;
double atb_Ratio_Tof64(struct atb_Ratio ratio) ATB_PUBLIC;
/**@}*/

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
 *  \brief Same as _APPLY but divide first, then multiply
 *
 *  \param[in] value The value that will be multiply by the ratio
 *  \param[in] ratio The ratio we wish to apply
 *
 *  \warning 'ratio' SHOULDN'T have any side effects
 *
 *  \return The result of ((value) / (ratio).den * (ratio).num)
 */
#define atb_Ratio_APPLY_R(ratio, value) (((value) / (ratio).den) * (ratio).num)

/**@{*/
/**
 *  \brief Safely apply the given \a ratio to \a value
 *
 *  \note Prevent overflows/underflows
 *
 *  \param[in] ratio Ratio we wish to aplly
 *  \param[in] value Value we wish to transform using ratio
 *  \param[out] dest Ptr to the value where the result is stored
 *
 *  \pre ratio.den != 0
 *  \pre dest != NULL
 *
 *  \return bool True applying the ratio succeed. False when an
 *          overflows/underflows occured, with dest left unchanged.
 */
bool atb_Ratio_Apply_i64(struct atb_Ratio ratio, int64_t value,
                         int64_t *const dest) ATB_PUBLIC;
bool atb_Ratio_Apply_i32(struct atb_Ratio ratio, int32_t value,
                         int32_t *const dest) ATB_PUBLIC;
bool atb_Ratio_Apply_i16(struct atb_Ratio ratio, int16_t value,
                         int16_t *const dest) ATB_PUBLIC;
bool atb_Ratio_Apply_i8(struct atb_Ratio ratio, int8_t value,
                        int8_t *const dest) ATB_PUBLIC;
/**@}*/

/**
 *  \brief Inverse the ratio
 *
 *  \param[in] ratio The ratio we wish to inverse
 *  \param[out] dest The inverse computed
 *
 *  \pre dest != NULL
 *
 *  \return Always true.
 */
bool atb_Ratio_Inv(struct atb_Ratio ratio,
                   struct atb_Ratio *const dest) ATB_PUBLIC;

/**
 *  \brief Reduce a ratio (e.g. 2/4 becomes 1/2, ...)
 *
 *  \param[in] ratio The ratio we wish to reduce
 *  \param[out] dest The result of the reduction
 *
 *  \pre dest != NULL
 *
 *  \return Always true
 *
 *  \note When either num or den are equal to 0, dest is set to ratio
 */
bool atb_Ratio_Reduce(struct atb_Ratio ratio,
                      struct atb_Ratio *const dest) ATB_PUBLIC;
/**@{*/
/**
 *  \brief Add/Sub/Mul/Div 2 ratios and store the result in dest, if valid
 *
 *  \param[in] lhs, rhs Both ratios we wish to add
 *  \param[out] dest If not null, the result of the computation
 *
 *  \pre dest != NULL
 *
 *  \warning Having one of the inputs with a `.den = 0` is not checked and
 *           doesn't trigger any error
 *
 *  \returns True when the computation succeed. False when computing this new
 *           ratio would OVERFLOWS/UNDERFLOWS the underlying integer type.
 */
bool atb_Ratio_Add(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) ATB_PUBLIC;
bool atb_Ratio_Sub(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) ATB_PUBLIC;
bool atb_Ratio_Mul(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) ATB_PUBLIC;
bool atb_Ratio_Div(struct atb_Ratio lhs, struct atb_Ratio rhs,
                   struct atb_Ratio *const dest) ATB_PUBLIC;
/**@}*/

/* Comparison **************************************************************/

/**
 *  \brief Enum representing the result of the _Compare function
 */
typedef enum {
  atb_Ratio_Compare_LESS = -1,   /*!< LHS is LESS than RHS */
  atb_Ratio_Compare_EQUAL = 0,   /*!< LHS is EQUAL to RHS */
  atb_Ratio_Compare_GREATER = 1, /*!< LHS is GREATER than RHS */
} atb_Ratio_Compare_Result;

/**
 *  \brief Compare 2 ratios with each other and set dest accordingly
 *
 *  \param[in] lhs, rhs Both ratios we wish to compare
 *  \param[out] dest The result of the comparison, if succeeded
 *
 *  \return True when the comparison succeeded (no overflows/underflows...).
 *          False otherwise, and dest is left unchanged.
 *
 *  \pre dest != NULL
 */
bool atb_Ratio_Compare(struct atb_Ratio lhs, struct atb_Ratio rhs,
                       int *const dest) ATB_PUBLIC;

/**@{*/
/**
 * \returns bool True when \a lhs is [Eq, Ne, Lt, Gt, Le, Ge] than \a rhs.
 *
 * \param[in] lhs, rhs timespec we wish to compare
 *
 * \warning Also returns FALSE when the comparison canno't be performed due to
 *          overflows/undeflows
 *
 * \note
 * \li Eq = Equals (lhs == rhs)
 * \li Ne = Not Equals (lhs != rhs)
 * \li Lt = Less than (lhs < rhs)
 * \li Gt = Greater than (lhs > rhs)
 * \li Le = Less or equals (lhs <= rhs)
 * \li Ge = Greater or equals (lhs >= rhs)
 */
ATB_CMP_DEFINE_ALL_FROM_SAFE_COMPARE(static inline, atb_Ratio_,
                                     struct atb_Ratio);
/**@}*/

#if defined(__cplusplus)
} /* extern "C" */
#endif
