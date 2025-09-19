#include "atb/time.h"

#include <assert.h>

static bool stamp_ToNs(int64_t stamp, struct atb_Ratio to_sec,
                       int64_t *const dest) {
  assert(to_sec.den != 0);
  assert(dest != NULL);

  if (!atb_Ratio_Eq(to_sec, K_ATB_NS)) {
    /* Convert stamp to NS */

    /* -> Find the ratio to transform stamp into NS */
    if (!(atb_Ratio_Div(to_sec, K_ATB_NS, &(to_sec)) &&
          atb_Ratio_Reduce(to_sec, &(to_sec)))) {
      return false;
    }

    /* -> Convert the stamp using the ratio */
    if (!atb_Ratio_Apply_i64(to_sec, stamp, &(stamp))) {
      return false;
    }
  }

  *dest = stamp;
  return true;
}

bool atb_timespec_From(int64_t stamp, struct atb_Ratio to_sec,
                       struct timespec *const dest) {
  assert(to_sec.den != 0);
  assert(dest != NULL);

  if (atb_Ratio_Ge(to_sec, K_ATB_RATIO_1)) {
    if (!atb_Ratio_Apply_i64(to_sec, stamp, &(dest->tv_sec))) {
      return false;
    }
    dest->tv_nsec = 0;
  } else if (!stamp_ToNs(stamp, to_sec, &(stamp))) {
    return false;
  } else {
    dest->tv_sec = stamp / K_ATB_NS.den;
    dest->tv_nsec = stamp % K_ATB_NS.den;
  }

  return true;
}

ATB_TIMESPEC_CMP atb_timespec_Compare(struct timespec lhs,
                                      struct timespec rhs) {
  if (lhs.tv_sec < rhs.tv_sec) {
    return K_ATB_TIMESPEC_CMP_LESS;
  } else if (lhs.tv_sec > rhs.tv_sec) {
    return K_ATB_TIMESPEC_CMP_GREATER;
  } else {
    if (lhs.tv_nsec == rhs.tv_nsec) {
      return K_ATB_TIMESPEC_CMP_EQUAL;
    } else if (lhs.tv_nsec > rhs.tv_nsec) {
      return K_ATB_TIMESPEC_CMP_GREATER;
    } else {
      return K_ATB_TIMESPEC_CMP_LESS;
    }
  }
}

bool atb_Time_RetryCall(struct atb_Time_RetryPredicate predicate, size_t count,
                        struct timespec delay) {
  assert(predicate.function != NULL);

  /* Any negative timespecs cancel the retries */
  if (atb_timespec_Le(delay, (struct timespec){0, 0})) {
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
