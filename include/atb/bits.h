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
#define atb_BitMask(...) \
  atb_internal_BitMask(atb_VA_ARGN(__VA_ARGS__), ##__VA_ARGS__)

/**
 *  \return True when value has ALL bits set to 0b1, false otherwise
 */
#define atb_Bits_HasAllOf(value, i, ...)          \
  (((value) & (atb_BitMask(i, ##__VA_ARGS__))) == \
   (atb_BitMask(i, ##__VA_ARGS__)))

/**
 *  \return True when value has ANY of the bits set to 0b1, false otherwise
 */
#define atb_Bits_HasAnyOf(value, i, ...) \
  (((value) & (atb_BitMask(i, ##__VA_ARGS__))) != 0)

/**
 *  \brief Set the bits of value to 1
 */
#define atb_Bits_Set(value, i, ...)             \
  do {                                          \
    (value) |= (atb_BitMask(i, ##__VA_ARGS__)); \
  } while (0)

/**
 *  \brief Set the bits of value to 0
 */
#define atb_Bits_Unset(value, i, ...)            \
  do {                                           \
    (value) &= ~(atb_BitMask(i, ##__VA_ARGS__)); \
  } while (0)

#if defined(__cplusplus)
} /* extern "C" */
#endif
