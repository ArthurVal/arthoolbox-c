#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> /* bool */
#include <stdint.h>  /* intmax_t */
#include <time.h>    /* clock_getime */

#include "atb/ratio.h"

/* Pre-defined time related ratios *******************************************/
#define atb_NS atb_Ratio_NANO
#define atb_US atb_Ratio_MICRO
#define atb_MS atb_Ratio_MILLI
#define atb_SEC atb_Ratio_1
#define atb_MINUTES atb_Ratio_K(60)
#define atb_HOURS atb_Ratio_K(60 * 60)
#define atb_DAYS atb_Ratio_K(60 * 60 * 24)
#define atb_WEEKS atb_Ratio_K(60 * 60 * 24 * 7)
#define atb_MONTHS atb_Ratio_K(2629746)
#define atb_YEARS atb_Ratio_K(31556952)

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
static inline atb_timespec_Compare_Result atb_timespec_Compare(
    struct timespec lhs, struct timespec rhs);

static inline bool atb_timespec_Eq(struct timespec lhs, struct timespec rhs);
static inline bool atb_timespec_Ne(struct timespec lhs, struct timespec rhs);
static inline bool atb_timespec_Gt(struct timespec lhs, struct timespec rhs);
static inline bool atb_timespec_Lt(struct timespec lhs, struct timespec rhs);
static inline bool atb_timespec_Ge(struct timespec lhs, struct timespec rhs);
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

  to_sec = atb_Ratio_Div(to_sec, atb_NS);
  out.tv_nsec *= to_sec.num;
  out.tv_nsec /= to_sec.den;

  return out;
}

static inline atb_timespec_Compare_Result atb_timespec_Compare(
    struct timespec lhs, struct timespec rhs) {
  if (lhs.tv_sec < rhs.tv_sec) {
    return atb_timespec_Compare_LESS;
  } else if (lhs.tv_sec > rhs.tv_sec) {
    return atb_timespec_Compare_GREATER;
  } else {
    if (lhs.tv_nsec == rhs.tv_nsec) {
      return atb_timespec_Compare_EQUAL;
    } else if (lhs.tv_nsec > rhs.tv_nsec) {
      return atb_timespec_Compare_GREATER;
    } else {
      return atb_timespec_Compare_LESS;
    }
  }
}

static inline bool atb_timespec_Eq(struct timespec lhs, struct timespec rhs) {
  return atb_timespec_Compare(lhs, rhs) == atb_timespec_Compare_EQUAL;
}

static inline bool atb_timespec_Ne(struct timespec lhs, struct timespec rhs) {
  return !atb_timespec_Eq(lhs, rhs);
}

static inline bool atb_timespec_Gt(struct timespec lhs, struct timespec rhs) {
  switch (atb_timespec_Compare(lhs, rhs)) {
    case atb_timespec_Compare_GREATER: return true;
    default: return false;
  }
}

static inline bool atb_timespec_Lt(struct timespec lhs, struct timespec rhs) {
  switch (atb_timespec_Compare(lhs, rhs)) {
    case atb_timespec_Compare_LESS: return true;
    default: return false;
  }
}

static inline bool atb_timespec_Ge(struct timespec lhs, struct timespec rhs) {
  switch (atb_timespec_Compare(lhs, rhs)) {
    case atb_timespec_Compare_EQUAL:
    case atb_timespec_Compare_GREATER: return true;
    default: return false;
  }
}

static inline bool atb_timespec_Le(struct timespec lhs, struct timespec rhs) {
  switch (atb_timespec_Compare(lhs, rhs)) {
    case atb_timespec_Compare_EQUAL:
    case atb_timespec_Compare_LESS: return true;
    default: return false;
  }
}

static inline bool atb_Time_RetryCall(struct atb_Time_RetryPredicate predicate,
                                      size_t count, struct timespec delay) {
  assert(predicate.function != NULL);

  /* Any negative timespecs cancel the retries */
  if (atb_timespec_Le(delay, atb_timespec_From(0, atb_SEC))) {
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
