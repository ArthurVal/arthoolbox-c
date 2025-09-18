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

#define atb_timespec_FMT_RAW "{.tv_sec=%li, .tv_nsec=%li}"
#define atb_timespec_FMT "%li.%.9li"

#define atb_timespec_FMT_VA_ARG(ts) (ts).tv_sec, (ts).tv_nsec

/* Pre-defined time related ratios *******************************************/

/// Static initializer

#define atb_INIT_NS atb_Ratio_INIT_NANO
#define atb_INIT_US atb_Ratio_INIT_MICRO
#define atb_INIT_MS atb_Ratio_INIT_MILLI
#define atb_INIT_SEC atb_Ratio_INIT_1
#define atb_INIT_MINUTES atb_Ratio_INIT_(60)
#define atb_INIT_HOURS atb_Ratio_INIT_(60 * 60)
#define atb_INIT_DAYS atb_Ratio_INIT_(60 * 60 * 24)
#define atb_INIT_WEEKS atb_Ratio_INIT_(60 * 60 * 24 * 7)
#define atb_INIT_MONTHS atb_Ratio_INIT_(2629746)
#define atb_INIT_YEARS atb_Ratio_INIT_(31556952)

/// Compound literals

#define atb_NS (struct atb_Ratio) atb_INIT_NS
#define atb_US (struct atb_Ratio) atb_INIT_US
#define atb_MS (struct atb_Ratio) atb_INIT_MS
#define atb_SEC (struct atb_Ratio) atb_INIT_SEC
#define atb_MINUTES (struct atb_Ratio) atb_INIT_MINUTES
#define atb_HOURS (struct atb_Ratio) atb_INIT_HOURS
#define atb_DAYS (struct atb_Ratio) atb_INIT_DAYS
#define atb_WEEKS (struct atb_Ratio) atb_INIT_WEEKS
#define atb_MONTHS (struct atb_Ratio) atb_INIT_MONTHS
#define atb_YEARS (struct atb_Ratio) atb_INIT_YEARS

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
                atb_Ratio_APPLY((to_sec), atb_NS.den)),     \
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
  atb_timespec_Compare_LESS = -1,   /*!< LHS is LESS than RHS */
  atb_timespec_Compare_EQUAL = 0,   /*!< LHS is EQUAL to RHS */
  atb_timespec_Compare_GREATER = 1, /*!< LHS is GREATER than RHS */
} atb_timespec_Compare_Result;

/**
 * \brief Compare 2 timespecs with each other, following the 'three-way
 *        comparison' paradigm
 *
 * \param[in] lhs, rhs timespec we wish to compare
 *
 * \returns atb_timespec_Compare_Result And int value that follows the
 *          'three-way comparison' paradigm, i.e.:
 * \li res < 0 : lhs < rhs
 * \li res == 0: lhs == rhs
 * \li res > 0 : lhs > rhs
 */
atb_timespec_Compare_Result atb_timespec_Compare(
    struct timespec lhs, struct timespec rhs) ATB_PUBLIC;

/**@{*/
/**
 * \returns bool True when \a lhs is [Eq, Ne, Lt, Gt, Le, Ge] than \a rhs.
 *
 * \param[in] lhs, rhs timespec we wish to compare
 *
 * \note
 * \li Eq = Equals
 * \li Ne = Not Equals
 * \li Lt = Less than
 * \li Gt = Greater than
 * \li Le = Less or equals
 * \li Ge = Greater or equals
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
