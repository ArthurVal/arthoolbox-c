#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> /* bool */
#include <stdint.h>  /* intmax_t */
#include <time.h>    /* timespec */

#include "atb/ratio.h"

/* Format string helper ****************************************************/

#define atb_timespec_FMT_RAW "{.tv_sec=%li, .tv_nsec=%li}"
#define atb_timespec_FMT "%li.%.0li seconds"

#define atb_timespec_FMT_VA_ARG(ts) (ts).tv_sec, (ts).tv_nsec

/* Pre-defined time related ratios *******************************************/

#define atb_NS atb_Ratio_NANO
#define atb_US atb_Ratio_MICRO
#define atb_MS atb_Ratio_MILLI
#define atb_SEC atb_Ratio_(1)
#define atb_MINUTES atb_Ratio_(60)
#define atb_HOURS atb_Ratio_(60 * 60)
#define atb_DAYS atb_Ratio_(60 * 60 * 24)
#define atb_WEEKS atb_Ratio_(60 * 60 * 24 * 7)
#define atb_MONTHS atb_Ratio_(2629746)
#define atb_YEARS atb_Ratio_(31556952)

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
 *
 *  \return struct timespec A timespec created from the given stamp/ratio
 */
#define atb_timespec_FROM(stamp, to_sec)                    \
  (struct timespec) {                                       \
    .tv_sec = atb_Ratio_APPLY((to_sec), (stamp)),           \
    .tv_nsec = ((((stamp) * (to_sec).num) % (to_sec).den) * \
                atb_Ratio_APPLY((to_sec), atb_NS.den)),     \
  }

/**
 *  \brief Create a timespec given a raw stamp and its ratio to seconds
 *
 *  \param[in] stamp Raw timestamps
 *  \param[in] to_sec Ratio to seconds of the given raw timestamp
 *
 *  \return struct timespec A timespec created from the given stamp/ratio
 */
struct timespec atb_timespec_From(intmax_t stamp, struct atb_Ratio to_sec);

/**
 *  \return struct timespec of the current time. On failure, returns -1
 *          timespec, with errno set according to clock_gettime() function.
 *
 *  \param[in] clk The clock ID used to get the time, one of : [CLOCK_REALTIME
 *             ,CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, ...] (cf time.h)
 */
struct timespec atb_timespec_Now(clockid_t clk);

/* Comparisons *************************************************************/

/**
 *  \brief Enum representing the result of the _Compare function
 */
typedef enum {
  atb_timespec_Compare_LESS,    /*!< LHS is LESS than RHS */
  atb_timespec_Compare_EQUAL,   /*!< LHS is EQUAL to RHS */
  atb_timespec_Compare_GREATER, /*!< LHS is GREATER than RHS */
} atb_timespec_Compare_Result;

/**
 *  \brief Compare 2 timespecs with each other
 */
atb_timespec_Compare_Result atb_timespec_Compare(struct timespec lhs,
                                                 struct timespec rhs);

bool atb_timespec_Eq(struct timespec lhs, struct timespec rhs);
bool atb_timespec_Ne(struct timespec lhs, struct timespec rhs);
bool atb_timespec_Gt(struct timespec lhs, struct timespec rhs);
bool atb_timespec_Lt(struct timespec lhs, struct timespec rhs);
bool atb_timespec_Ge(struct timespec lhs, struct timespec rhs);
bool atb_timespec_Le(struct timespec lhs, struct timespec rhs);

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
                        struct timespec delay);

#if defined(__cplusplus)
} /* extern "C" */
#endif
