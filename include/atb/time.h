#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> /* bool */
#include <stdint.h>  /* intmax_t */
#include <time.h>    /* clock_getime */

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
 *  \return struct timespec Of the duration provided in NANOSECONDS
 *  \param[in] stamp_ns The duration we wish to convert in NANOSECONDS
 */
static inline struct timespec atb_timespec_FromNs(intmax_t stamp_ns);

/**
 *  \return struct timespec Of the duration provided in MICROSECONDS
 *  \param[in] stamp_us The duration we wish to convert in MICROSECONDS
 */
static inline struct timespec atb_timespec_FromUs(intmax_t stamp_us);

/**
 *  \return struct timespec Of the duration provided in MILLISECONDS
 *  \param[in] stamp_ms The duration we wish to convert in MILLISECONDS
 */
static inline struct timespec atb_timespec_FromMs(intmax_t stamp_ms);

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
 *

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
static inline struct timespec atb_timespec_Now(clockid_t clk) {
  struct timespec now;

  if (clock_gettime(clk, &now)) {
    now.tv_sec  = -1;
    now.tv_nsec = -1;
  }

  return now;
}

static inline struct timespec atb_timespec_FromNs(intmax_t stamp_ns) {
  const intmax_t NSEC_PER_SECONDS = 1e9;
  return (struct timespec){
      .tv_sec = stamp_ns / NSEC_PER_SECONDS,
      .tv_nsec = stamp_ns % NSEC_PER_SECONDS,
  };
}

static inline struct timespec atb_timespec_FromUs(intmax_t stamp_us) {
  const intmax_t USEC_PER_SECONDS = 1e6;
  return (struct timespec){
      .tv_sec = stamp_us / USEC_PER_SECONDS,
      .tv_nsec = (stamp_us % USEC_PER_SECONDS) * 1e3,
  };
}

static inline struct timespec atb_timespec_FromMs(intmax_t stamp_ms) {
  const intmax_t MSEC_PER_SECONDS = 1e3;
  return (struct timespec){
      .tv_sec = stamp_ms / MSEC_PER_SECONDS,
      .tv_nsec = (stamp_ms % MSEC_PER_SECONDS) * 1e6,
  };
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
