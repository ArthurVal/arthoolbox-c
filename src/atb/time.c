#include "atb/time.h"

#include <assert.h>

struct timespec atb_timespec_Now(clockid_t clk) {
  struct timespec now;

  if (clock_gettime(clk, &now)) {
    now.tv_sec = -1;
    now.tv_nsec = -1;
  }

  return now;
}

struct timespec atb_timespec_From(intmax_t stamp, struct atb_Ratio to_sec) {
  assert(to_sec.den != 0);

  struct timespec out;

  stamp *= to_sec.num;
  out.tv_sec = (stamp / to_sec.den);
  out.tv_nsec = (stamp % to_sec.den);

  atb_Ratio_Div(to_sec, atb_NS, &to_sec);
  out.tv_nsec *= to_sec.num;
  out.tv_nsec /= to_sec.den;

  return out;
}

atb_timespec_Compare_Result atb_timespec_Compare(struct timespec lhs,
                                                 struct timespec rhs) {
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

bool atb_Time_RetryCall(struct atb_Time_RetryPredicate predicate, size_t count,
                        struct timespec delay) {
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
