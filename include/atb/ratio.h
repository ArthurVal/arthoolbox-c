#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "atb/compare.h"
#include "atb/export.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Underlying integer type used within a atb_Ratio
typedef uint32_t atb_Ratio_elem_t;

/**
 *  \brief Contains a ratio reprensted by 2 UNSIGNED integers
 *  \note Ratio's can't be NEGATIVE
 */
struct atb_Ratio {
  atb_Ratio_elem_t num; /*!< The numerator of the ratio */
  atb_Ratio_elem_t den; /*!< The denumerator of the ratio */
};

/* Format string helper ****************************************************/

#define K_ATB_RATIO_FMT "{.num=%ju, .den=%ju}"
#define atb_Ratio_FMT_VA_ARG(ratio) \
  (uintmax_t)(ratio).num, (uintmax_t)(ratio).den

/* Constant ****************************************************************/

/// Initialize an atb_Ratio as n:1 (static initializer)
#define K_ATB_RATIO_INIT_(n) \
  { .num = (n), .den = 1, }

/// Initialize an atb_Ratio as 1:n (static initializer)
#define K_ATB_RATIO_INIT_1_(n) \
  { .num = 1, .den = (n), }

/* Requires 'atb_Ratio_elem_t' to be at least 8 bits */
#define K_ATB_RATIO_INIT_0 K_ATB_RATIO_INIT_(0)
#define K_ATB_RATIO_INIT_1 K_ATB_RATIO_INIT_(1)

#define K_ATB_RATIO_INIT_DECA K_ATB_RATIO_INIT_(10)
#define K_ATB_RATIO_INIT_DECI K_ATB_RATIO_INIT_1_(10)

#define K_ATB_RATIO_INIT_HECTO K_ATB_RATIO_INIT_(100)
#define K_ATB_RATIO_INIT_CENTI K_ATB_RATIO_INIT_1_(100)

/* Requires 'atb_Ratio_elem_t' to be at least 16 bits */
#define K_ATB_RATIO_INIT_KILO K_ATB_RATIO_INIT_(1000)
#define K_ATB_RATIO_INIT_MILLI K_ATB_RATIO_INIT_1_(1000)

/* Requires 'atb_Ratio_elem_t' to be at least 32 bits */
#define K_ATB_RATIO_INIT_MEGA K_ATB_RATIO_INIT_(1000000)
#define K_ATB_RATIO_INIT_MICRO K_ATB_RATIO_INIT_1_(1000000)

#define K_ATB_RATIO_INIT_GIGA K_ATB_RATIO_INIT_(1000000000)
#define K_ATB_RATIO_INIT_NANO K_ATB_RATIO_INIT_1_(1000000000)

/* Requires 'atb_Ratio_elem_t' to be at least 64 bits */
#define K_ATB_RATIO_INIT_TERA K_ATB_RATIO_INIT_(1000000000000)
#define K_ATB_RATIO_INIT_PICO K_ATB_RATIO_INIT_1_(1000000000000)

#define K_ATB_RATIO_INIT_PERA K_ATB_RATIO_INIT_(1000000000000000)
#define K_ATB_RATIO_INIT_FEMPTO K_ATB_RATIO_INIT_1_(1000000000000000)

/// Compound literal of a ratio corresponding to n:1
#define K_ATB_RATIO_(n) (struct atb_Ratio) K_ATB_RATIO_INIT_(n)

/// Compound literal of a ratio corresponding to 1:n
#define K_ATB_RATIO_1_(n) (struct atb_Ratio) K_ATB_RATIO_INIT_1_(n)

/* Requires 'atb_Ratio_elem_t' to be at least 8 bits */
#define K_ATB_RATIO_0 (struct atb_Ratio) K_ATB_RATIO_INIT_0
#define K_ATB_RATIO_1 (struct atb_Ratio) K_ATB_RATIO_INIT_1

#define K_ATB_RATIO_DECA (struct atb_Ratio) K_ATB_RATIO_INIT_DECA
#define K_ATB_RATIO_DECI (struct atb_Ratio) K_ATB_RATIO_INIT_DECI

#define K_ATB_RATIO_HECTO (struct atb_Ratio) K_ATB_RATIO_INIT_HECTO
#define K_ATB_RATIO_CENTI (struct atb_Ratio) K_ATB_RATIO_INIT_CENTI

/* Requires 'atb_Ratio_elem_t' to be at least 16 bits */
#define K_ATB_RATIO_KILO (struct atb_Ratio) K_ATB_RATIO_INIT_KILO
#define K_ATB_RATIO_MILLI (struct atb_Ratio) K_ATB_RATIO_INIT_MILLI

/* Requires 'atb_Ratio_elem_t' to be at least 32 bits */
#define K_ATB_RATIO_MEGA (struct atb_Ratio) K_ATB_RATIO_INIT_MEGA
#define K_ATB_RATIO_MICRO (struct atb_Ratio) K_ATB_RATIO_INIT_MICRO

#define K_ATB_RATIO_GIGA (struct atb_Ratio) K_ATB_RATIO_INIT_GIGA
#define K_ATB_RATIO_NANO (struct atb_Ratio) K_ATB_RATIO_INIT_NANO

/* Requires 'atb_Ratio_elem_t' to be at least 64 bits */
/* #define K_ATB_RATIO_TERA (struct atb_Ratio) K_ATB_RATIO_INIT_TERA */
/* #define K_ATB_RATIO_PICO (struct atb_Ratio) K_ATB_RATIO_INIT_PICO */

/* #define K_ATB_RATIO_PERA (struct atb_Ratio) K_ATB_RATIO_INIT_PERA */
/* #define K_ATB_RATIO_FEMPTO (struct atb_Ratio) K_ATB_RATIO_INIT_FEMPTO */

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
bool atb_Ratio_Apply_u64(struct atb_Ratio ratio, uint64_t value,
                         uint64_t *const dest) ATB_PUBLIC;
bool atb_Ratio_Apply_u32(struct atb_Ratio ratio, uint32_t value,
                         uint32_t *const dest) ATB_PUBLIC;
bool atb_Ratio_Apply_u16(struct atb_Ratio ratio, uint16_t value,
                         uint16_t *const dest) ATB_PUBLIC;
bool atb_Ratio_Apply_u8(struct atb_Ratio ratio, uint8_t value,
                        uint8_t *const dest) ATB_PUBLIC;
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
 *  \param[in] lhs, rhs Both ratio operands
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
  K_ATB_RATIO_CMP_LESS = -1,   /*!< LHS is LESS than RHS */
  K_ATB_RATIO_CMP_EQUAL = 0,   /*!< LHS is EQUAL to RHS */
  K_ATB_RATIO_CMP_GREATER = 1, /*!< LHS is GREATER than RHS */
} ATB_RATIO_CMP;

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
