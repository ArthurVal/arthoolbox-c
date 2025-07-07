#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> /* bool */
#include <stdint.h>  /* intmax_t */
#include <time.h>    /* clock_getime */

#include "atb/ratio.h"

/* Pre-defined time related ratios *******************************************/
static inline struct atb_Ratio atb_ns(void);
static inline struct atb_Ratio atb_us(void);
static inline struct atb_Ratio atb_ms(void);
static inline struct atb_Ratio atb_sec(void);
static inline struct atb_Ratio atb_minutes(void);
static inline struct atb_Ratio atb_hours(void);
static inline struct atb_Ratio atb_days(void);
static inline struct atb_Ratio atb_weeks(void);
static inline struct atb_Ratio atb_months(void);
static inline struct atb_Ratio atb_years(void);

/* Constructors **************************************************************/
/**
 *  \return struct timespec of the current time. On failure, returns -1
 *          timespec, with errno set according to clock_gettime() function.
 *
 *  \param[in] clk The clock ID used to get the time, one of : [CLOCK_REALTIME
 *             ,CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, ...] (cf time.h)
 */
static inline struct timespec atb_timespec_Now(clockid_t clk);

/**
 *  \brief Create a timespec given a raw stamp and its ratio to seconds
 *
 *  \param[in] stamp Raw timestamps
 *  \param[in] to_sec Ratio to seconds of the given raw timestamp
 *
 *  \return struct timespec A timespec created from the given stamp/ratio
 */
static inline struct timespec atb_timespec_From(intmax_t stamp,
                                                struct atb_Ratio to_sec);

/* Operators *****************************************************************/
/// Returns true when lhs == rhs
static inline bool atb_timespec_Eq(struct timespec lhs, struct timespec rhs);

/// Returns true when lhs != rhs
static inline bool atb_timespec_Ne(struct timespec lhs, struct timespec rhs);

/// Returns true when lhs > rhs
static inline bool atb_timespec_Gt(struct timespec lhs, struct timespec rhs);

/// Returns true when lhs < rhs
static inline bool atb_timespec_Lt(struct timespec lhs, struct timespec rhs);

/// Returns true when lhs >= rhs
static inline bool atb_timespec_Ge(struct timespec lhs, struct timespec rhs);

/// Returns true when lhs <= rhs
static inline bool atb_timespec_Le(struct timespec lhs, struct timespec rhs);

/* Utils functions ***********************************************************/
/**
 *  \brief Argument used to describe the predicate (function ptr + args) from
 *         _RetryCall()
 */
struct atb_Time_RetryPredicate {
  bool (*function)(void *); /*!< Predicate function use inside RetryCall */
  void *args;               /*!< Argument forwarded to the function above */
};

typedef struct atb_Time_RetryPredicate atb_Time_RetryPredicate;

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
static inline bool atb_Time_RetryCall(struct atb_Time_RetryPredicate predicate,
                                      size_t count, struct timespec delay);

/***************************************************************************/
/*                           Inline definitions                            */
/***************************************************************************/

static inline struct atb_Ratio atb_ns(void) { return atb_nano(); }

static inline struct atb_Ratio atb_us(void) { return atb_micro(); }

static inline struct atb_Ratio atb_ms(void) { return atb_milli(); }

static inline struct atb_Ratio atb_sec(void) { return atb_one(); }

static inline struct atb_Ratio atb_minutes(void) {
  struct atb_Ratio r = atb_sec();
  r.num *= 60;
  return r;
}

static inline struct atb_Ratio atb_hours(void) {
  struct atb_Ratio r = atb_minutes();
  r.num *= 60;
  return r;
}

static inline struct atb_Ratio atb_days(void) {
  struct atb_Ratio r = atb_hours();
  r.num *= 24;
  return r;
}

static inline struct atb_Ratio atb_weeks(void) {
  struct atb_Ratio r = atb_days();
  r.num *= 7;
  return r;
}

static inline struct atb_Ratio atb_months(void) {
  struct atb_Ratio r;
  r.num = 2629746;
  r.den = 1;
  return r;
}

static inline struct atb_Ratio atb_years(void) {
  struct atb_Ratio r;
  r.num = 31556952;
  r.den = 1;
  return r;
}

static inline struct timespec atb_timespec_Now(clockid_t clk) {
  struct timespec now;

  if (clock_gettime(clk, &now)) {
    now.tv_sec = -1;
    now.tv_nsec = -1;
  }

  return now;
}

static inline struct timespec atb_timespec_From(intmax_t stamp,
                                                struct atb_Ratio to_sec) {
  assert(to_sec.den != 0);

  struct timespec out;

  stamp *= to_sec.num;
  out.tv_sec = (stamp / to_sec.den);
  out.tv_nsec = (stamp % to_sec.den);

  to_sec = atb_Ratio_Div(to_sec, atb_ns());
  out.tv_nsec *= to_sec.num;
  out.tv_nsec /= to_sec.den;

  return out;
}

static inline bool atb_timespec_Eq(struct timespec lhs, struct timespec rhs) {
  return (lhs.tv_sec == rhs.tv_sec) && (lhs.tv_nsec == rhs.tv_nsec);
}

static inline bool atb_timespec_Ne(struct timespec lhs, struct timespec rhs) {
  return !atb_timespec_Eq(lhs, rhs);
}

static inline bool atb_timespec_Gt(struct timespec lhs, struct timespec rhs) {
  if (lhs.tv_sec == rhs.tv_sec) {
    return lhs.tv_nsec > rhs.tv_nsec;
  } else {
    return lhs.tv_sec > rhs.tv_sec;
  }
}

static inline bool atb_timespec_Lt(struct timespec lhs, struct timespec rhs) {
  return atb_timespec_Gt(rhs, lhs);
}

static inline bool atb_timespec_Ge(struct timespec lhs, struct timespec rhs) {
  return !atb_timespec_Lt(lhs, rhs);
}

static inline bool atb_timespec_Le(struct timespec lhs, struct timespec rhs) {
  return !atb_timespec_Gt(lhs, rhs);
}

static inline bool atb_Time_RetryCall(struct atb_Time_RetryPredicate predicate,
                                      size_t count, struct timespec delay) {
  assert(predicate.function != NULL);

  /* Any negative timespecs cancel the retries */
  if (atb_timespec_Le(delay, atb_timespec_From(0, atb_sec()))) {
    count = 0;
  }

  bool succeeded = predicate.function(predicate.args);
  while ((!succeeded) && (count-- != 0)) {
    /* TODO: Handle EINTR when receiving signals ? */
    if (nanosleep(&delay, NULL) == -1) break;

    succeeded = predicate.function(predicate.args);
  }

  return succeeded;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
