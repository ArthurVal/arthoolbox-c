#pragma once

#include <stdbool.h> /* bool */
#include <stdint.h>  /* int64_t */
#include <time.h>    /* timespec */

#include "atb/compare.h"
#include "atb/export.h"
#include "atb/ratio.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Format string helper ****************************************************/

#define K_ATB_TIMESPEC_FMT_RAW "{.tv_sec=%li, .tv_nsec=%li}"
#define K_ATB_TIMESPEC_FMT "%li.%.9li"

#define atb_timespec_FMT_VA_ARG(ts) (ts).tv_sec, (ts).tv_nsec

/* Pre-defined time related ratios *******************************************/

/// Static initializer

#define K_ATB_INIT_NS K_ATB_RATIO_INIT_NANO
#define K_ATB_INIT_US K_ATB_RATIO_INIT_MICRO
#define K_ATB_INIT_MS K_ATB_RATIO_INIT_MILLI
#define K_ATB_INIT_SEC K_ATB_RATIO_INIT_1
#define K_ATB_INIT_MINUTES K_ATB_RATIO_INIT_(60)
#define K_ATB_INIT_HOURS K_ATB_RATIO_INIT_(60 * 60)
#define K_ATB_INIT_DAYS K_ATB_RATIO_INIT_(60 * 60 * 24)
#define K_ATB_INIT_WEEKS K_ATB_RATIO_INIT_(60 * 60 * 24 * 7)
#define K_ATB_INIT_MONTHS K_ATB_RATIO_INIT_(2629746)
#define K_ATB_INIT_YEARS K_ATB_RATIO_INIT_(31556952)

/// Compound literals

#define K_ATB_NS (struct atb_Ratio) K_ATB_INIT_NS
#define K_ATB_US (struct atb_Ratio) K_ATB_INIT_US
#define K_ATB_MS (struct atb_Ratio) K_ATB_INIT_MS
#define K_ATB_SEC (struct atb_Ratio) K_ATB_INIT_SEC
#define K_ATB_MINUTES (struct atb_Ratio) K_ATB_INIT_MINUTES
#define K_ATB_HOURS (struct atb_Ratio) K_ATB_INIT_HOURS
#define K_ATB_DAYS (struct atb_Ratio) K_ATB_INIT_DAYS
#define K_ATB_WEEKS (struct atb_Ratio) K_ATB_INIT_WEEKS
#define K_ATB_MONTHS (struct atb_Ratio) K_ATB_INIT_MONTHS
#define K_ATB_YEARS (struct atb_Ratio) K_ATB_INIT_YEARS

/* Constructors **************************************************************/

/**
 *  \brief Create a timespec given a raw stamp and its ratio to seconds
 *
 *  \note Usable as static initializer
 *
 *  \param[in] stamp An integer corresponding to the raw timestamp
 *  \param[in] to_sec An atb_Ratio to seconds of the given timestamp
 *
 *  \warning Arguments SHOULDN'T have any side effect
 *  \warning This is UNSAFE, i.e. no overflows/underflows checks are performed
 *
 *  \return struct timespec A timespec created from the given stamp/ratio
 */
#define atb_timespec_INIT_FROM(stamp, to_sec)               \
  {                                                         \
    .tv_sec = atb_Ratio_APPLY((to_sec), (stamp)),           \
    .tv_nsec = ((((stamp) * (to_sec).num) % (to_sec).den) * \
                atb_Ratio_APPLY((to_sec), K_ATB_NS.den)),   \
  }

/**
 *  \brief Same as atb_timespec_INIT_FROM but as a compound literal
 */
#define atb_timespec_FROM(stamp, to_sec) \
  (struct timespec) atb_timespec_INIT_FROM((stamp), (to_sec))

/**
 *  \brief Set a timespec given a raw stamp and its ratio to seconds
 *
 *  \param[in] stamp Raw timestamps
 *  \param[in] to_sec Ratio to seconds of the given raw timestamp
 *  \param[out] dest Ptr to the timespec we wish to update
 *
 *  \pre to_sec.den != 0
 *  \pre dest != NULL
 *
 *  \return bool True when the operation succeed. False when the operation
 *               induce an overflows/underflows.
 */
bool atb_timespec_From(int64_t stamp, struct atb_Ratio to_sec,
                       struct timespec *const dest) ATB_PUBLIC;

/* Comparisons *************************************************************/

/**
 *  \brief Enum representing the result of the _Compare function
 */
typedef enum {
  K_ATB_TIMESPEC_CMP_LESS = -1,   /*!< LHS is LESS than RHS */
  K_ATB_TIMESPEC_CMP_EQUAL = 0,   /*!< LHS is EQUAL to RHS */
  K_ATB_TIMESPEC_CMP_GREATER = 1, /*!< LHS is GREATER than RHS */
} ATB_TIMESPEC_CMP;

/**
 * \brief Compare 2 timespecs with each other, following the 'three-way
 *        comparison' paradigm
 *
 * \param[in] lhs, rhs timespec we wish to compare
 *
 * \returns atb_timespec_Compare_Result An int value that follows the
 *          'three-way comparison' paradigm, i.e.:
 * \li res < 0 : lhs < rhs
 * \li res == 0: lhs == rhs
 * \li res > 0 : lhs > rhs
 */
ATB_TIMESPEC_CMP atb_timespec_Compare(struct timespec lhs,
                                      struct timespec rhs) ATB_PUBLIC;

/**@{*/
/**
 * \returns bool True when \a lhs is [Eq, Ne, Lt, Gt, Le, Ge] than \a rhs.
 *
 * \param[in] lhs, rhs timespec we wish to compare
 *
 * \note
 * \li Eq = Equals (lhs == rhs)
 * \li Ne = Not Equals (lhs != rhs)
 * \li Lt = Less than (lhs < rhs)
 * \li Gt = Greater than (lhs > rhs)
 * \li Le = Less or equals (lhs <= rhs)
 * \li Ge = Greater or equals (lhs >= rhs)
 */
ATB_CMP_DEFINE_ALL_FROM_UNSAFE_COMPARE(static inline, atb_timespec_,
                                       struct timespec);
/**@}*/

/* Utils functions ***********************************************************/

/**
 *  \brief Argument used to describe the predicate (function ptr + args) from
 *         _RetryCall()
 */
struct atb_Time_RetryPredicate {
  bool (*function)(void *); /*!< Predicate function use inside RetryCall */
  void *args;               /*!< Argument forwarded to the function above */
};

/**
 *  \brief Call 'predicate'. If it fails, retry to call it until succeed or
 *         up to 'count' times separated by 'delay' in between each retries
 *
 *  \param[in] predicate Function that will be call every time until it returns
 *             TRUE or the retry count is reached.
 *  \param[in] count Number of retry to do when predicate fails
 *  \param[in] delay Delay in between each failed call
 *
 *  \return True when the predicate succeed BEFORE we exceeded count,
 *          otherwise, false if the retry count has been reached and predicate
 *          failed everytime.
 *
 *  \pre predicate.function != NULL
 *
 *  \note count will be zeroed when delay is <= 0
 */
bool atb_Time_RetryCall(struct atb_Time_RetryPredicate predicate, size_t count,
                        struct timespec delay) ATB_PUBLIC;

#if defined(__cplusplus)
} /* extern "C" */
#endif
