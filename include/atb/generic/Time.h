#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h> /* uintmax_t */
#include <time.h>   /* clock_getime */

/**
 *  \return uintmax_t Corresponding to the timestamp in NS
 *
 *  \param[in] other Ptr to the begin of a raw c-string (null-terminated)
 */
inline uintmax_t atb_timespec_ToNs(struct timespec stamp) {
  if (clock_gettime(CLOCK_MONOTONIC_RAW, &stamp)) {
    stamp.tv_sec = 0;
    stamp.tv_nsec = 0;
  }

  return (uintmax_t)stamp.tv_sec * (uintmax_t)1e9 + (uintmax_t)stamp.tv_nsec;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
