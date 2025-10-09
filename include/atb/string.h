
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "atb/error.h"
#include "atb/export.h"
#include "atb/span/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Use to qualify the integer BASE for string representation
typedef enum {
  K_ATB_INT_BIN = 2,  /*!< Binary */
  K_ATB_INT_OCT = 8,  /*!< Octal */
  K_ATB_INT_DEC = 10, /*!< Decimal */
  K_ATB_INT_HEX = 16, /*!< Hexadecimal */
} ATB_INT_BASE;

/**@{*/
/**
 * \brief Convert the given \a value to its string repr into \a dest.
 *
 * \param[in] value Int value we wish to convert to string
 * \param[in] base Base use to represent the int
 * \param[in] dest Destination str range (may be invalid)
 * \param[out] remaining Optional.
 *                       When \a dest is a VALID StrSpan, set to the remaining
 *                       space in \a dest after writing in it.
 *                       Otherwise (\a dest is INVALID) set remaining->size to
 *                       the minimum size required to represent the given
 *                       integer.
 * \param[out] err Optional. Error set whenever the operation failed.
 *                 Possible values are:
 *                 - K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE: When the provided \a
 *                   dest is too small to fit the int string repr
 *
 * \returns bool True on success. False otherwise, \a err is set accordingly and
 *               both dest and remaining remains untouched.
 *
 * \important Signess ONLY appears when selecting K_ATB_INT_DEC base
 *
 * \pre base is in [2;37) interval
 */
ATB_PUBLIC bool atb_String_FromInt_u(uintmax_t value, ATB_INT_BASE base,
                                     struct atb_StrSpan dest,
                                     struct atb_StrSpan *const remaining,
                                     struct atb_Error *const err);

ATB_PUBLIC bool atb_String_FromInt_i(intmax_t value, ATB_INT_BASE base,
                                     struct atb_StrSpan dest,
                                     struct atb_StrSpan *const remaining,
                                     struct atb_Error *const err);
/**@}*/

#if defined(__cplusplus)
}
#endif
