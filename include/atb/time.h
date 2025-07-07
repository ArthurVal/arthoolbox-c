#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> /* bool */
#include <stdint.h>  /* intmax_t */
#include <time.h>    /* clock_getime */

#include "atb/ratio.h"

/* Pre-defined ratios ********************************************************/
static inline struct atb_Ratio atb_ns(void);
static inline struct atb_Ratio atb_us(void);
static inline struct atb_Ratio atb_ms(void);
static inline struct atb_Ratio atb_sec(void);
static inline struct atb_Ratio atb_min(void);
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
 *  \brief Call 'predicate' expression until it evaluates to true or up to
 *         'count' times, separated by 'delay' in between each retry calls
 *
 *  \param[in] predicate Expression expected to evaluate to a boolean expression
 *             , that will be call every time until it returns TRUE or the retry
 *             count is reached.
 *  \param[in] count (size_t) Number of retry to do when predicate fails
 *  \param[in] delay (struct timespec) Delay in between each failed call
 *
 *  \return True when the predicate succeed BEFORE we exceeded count,
 *          otherwise, false if the retry count has been reached and predicate
 *          failed everytime.
 *
 *  \note Use the STATEMENT EXPRESSION (using braces {} inside parenthesis) GNU
 *        extension, only available with GCC and Clang. It allows you to use
 *        loops/switches/local variables inside a macro expression, serving as
 *        'safe macro' (can test input params types and only evaluate them once)
 *        and have a return value. See
 *        https://gcc.gnu.org/onlinedocs/gcc-3.1.1/gcc/Statement-Exprs.html
 *
 *  \code
 *  int main() {
 *    int i = 0;
 *
 *    if(atb_Time_retryCallExpr(
 *        (printf("%d\n", i), (i++ == 5)),
 *        20,
 *        atb_timespec_fromMs(5)
 *      )) {
 *      puts("Expression succeed");
 *    } else {
 *      puts("Max count reached");
 *    }
 *    // This expression above ouputs:
 *    // 0
 *    // 1
 *    // 2
 *    // 3
 *    // 4
 *    // 5
 *    // Expression succeed
 *
 *    i = 0;
 *    if(atb_Time_retryCallExpr(
 *        (printf("%d\n", i), (i++ == 5)),
 *        4,
 *        atb_timespec_fromMs(5)
 *      )) {
 *      puts("Expression succeed");
 *    } else {
 *      puts("Max count reached");
 *    }
 *    // This expression above ouputs:
 *    // 0
 *    // 1
 *    // 2
 *    // 3
 *    // Max count reached
 *
 *    return 0;
 *  }
 *  \endcode
 */
#define atb_Time_RetryCallExpr(predicate, count, delay)                        \
  ({                                                                           \
    /* This is used as 'safe macro' in order to check input types */           \
    /* Also all variables declared inside the macro expression are */          \
    /* surounded by '_' in order to avoid shadowing with user code. */         \
    struct timespec _delay_ = (delay);                                         \
    bool _succeed_ = false;                                                    \
    /* Comparison here is used to secure negative timespecs ... */             \
    size_t _count_ =                                                           \
        (atb_timespec_Le(_delay_, atb_timespec_FromNs(0)) ? 0 : (count));      \
                                                                               \
    while (!(_succeed_ = (predicate)) && (_count_ != 0)) {                     \
      --_count_;                                                               \
      if (nanosleep(&_delay_, NULL) == -1) {                                   \
        /* TODO: Handle EINTR when receiving signals ? */                      \
      }                                                                        \
    }                                                                          \
    _succeed_; /* return value */                                              \
  })

/**
 *  \brief Argument used to describe the predicate (function ptr + args) from
 *         _RetryCall()
 */
struct atb_Time_RetryPredicate {
  bool (*function)(void *); /*!< Predicate function use inside RetryCall */
  void *args;               /*!< Argument forwarded to the function above */
};

/**
 *  \brief Call 'predicate' until it returns true or up to 'count' times,
 *         separated by 'delay' in between each retry calls
 *
 *  \param[in] predicate Function that will be call every time until it returns
 *             TRUE or the retry count is reached.
 *  \param[in] count Number of retry to do when predicate fails
 *  \param[in] delay Delay in between each failed call
 *
 *  \return True when the predicate succeed BEFORE we exceeded count,
 *          otherwise, false if the retry count has been reached and predicate
 *          failed everytime.
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
  struct atb_Ratio r = atb_min();
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
  out.tv_nsec /= to_sec.den;
  out.tv_nsec *= to_sec.num;

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
  if(predicate.function == NULL)
    return false;

  /* We use retryCallExpr here in order to avoid code duplication...
   * TODO: Find a way to re-use the GNU style stuff without triggering
   *       warnings ?
   */
  return atb_Time_RetryCallExpr(predicate.function(predicate.args), count,
                                delay);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
