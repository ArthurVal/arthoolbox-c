#include "atb/time.h"

#include <assert.h>
#include <stdlib.h> /* labs */

static bool stamp_ToNs(uint64_t stamp, struct atb_Ratio to_sec,
                       uint64_t *const dest) {
  assert(to_sec.den != 0);
  assert(dest != NULL);

  if (atb_Ratio_Ne(to_sec, K_ATB_NS)) {
    /* Convert stamp to NS */

    /* -> Find the ratio to transform stamp into NS */
    if (!(atb_Ratio_Div(to_sec, K_ATB_NS, &(to_sec)) &&
          atb_Ratio_Reduce(to_sec, &(to_sec)))) {
      return false;
    }

    /* -> Convert the stamp using the ratio */
    if (!atb_Ratio_Apply_u64(to_sec, stamp, &(stamp))) {
      return false;
    }
  }

  *dest = stamp;
  return true;
}

bool atb_timespec_Set(struct timespec *const self, uint64_t stamp,
                      struct atb_Ratio to_sec) {
  assert(self != NULL);
  assert(to_sec.den != 0);

  bool success = true;

  if (atb_Ratio_Ge(to_sec, K_ATB_RATIO_1)) {
    if (atb_Ratio_Apply_u64(to_sec, stamp, &(stamp)) && (stamp <= INT64_MAX)) {
      self->tv_sec = (int64_t)stamp;
      self->tv_nsec = 0;
    } else {
      success = false;
    }
  } else if (stamp_ToNs(stamp, to_sec, &(stamp)) &&
             ((stamp / K_ATB_NS.den) <= INT64_MAX)) {
    self->tv_sec = (__typeof__(self->tv_sec))(stamp / K_ATB_NS.den);
    self->tv_nsec = (__typeof__(self->tv_nsec))(stamp % K_ATB_NS.den);
  } else {
    success = false;
  }

  return success;
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
