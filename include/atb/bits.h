#pragma once

#include <stdint.h>

#include "atb/macro.h"
#include "internal/bits.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  \return A bit mask with the given bits (indexes starting at 0) set to
 *          0b1 (up to 128 bits)
 */
#define ATB_BITMASK(...) \
  ATB_INTERNAL_BitMask(ATB_VA_ARGN(__VA_ARGS__), ##__VA_ARGS__)

/**
 *  \return True when value has ALL bits set to 0b1, false otherwise
 */
#define ATB_BITS_HasAllOf(value, i, ...)          \
  (((value) & (ATB_BITMASK(i, ##__VA_ARGS__))) == \
   (ATB_BITMASK(i, ##__VA_ARGS__)))

/**
 *  \return True when value has ANY of the bits set to 0b1, false otherwise
 */
#define ATB_BITS_HasAnyOf(value, i, ...) \
  (((value) & (ATB_BITMASK(i, ##__VA_ARGS__))) != 0)

/**
 *  \brief Set the bits of value to 1
 */
#define ATB_BITS_Set(value, i, ...)             \
  do {                                          \
    (value) |= (ATB_BITMASK(i, ##__VA_ARGS__)); \
  } while (0)

/**
 *  \brief Set the bits of value to 0
 */
#define ATB_BITS_Unset(value, i, ...)            \
  do {                                           \
    (value) &= ~(ATB_BITMASK(i, ##__VA_ARGS__)); \
  } while (0)

#if defined(__cplusplus)
} /* extern "C" */
#endif
