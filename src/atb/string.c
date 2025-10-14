#include "atb/string.h"

#include <ctype.h> /* isdigit */

static const struct atb_StrView m_digits =
    atb_StrView_From_StrLiteral_INIT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static bool INT_BASE_IsValid(ATB_INT_BASE base) {
  return (2 <= base) && (base <= m_digits.size);
}

static size_t Int_StrWidth_u(uintmax_t value, ATB_INT_BASE base) {
  size_t width = 1;

  if (value != 0) {
    while ((value /= base) != 0) width++;
  }

  return width;
}

static size_t Int_StrWidth_i(intmax_t value, ATB_INT_BASE base) {
  size_t width = 0;

  if (value < 0) {
    width = 1 + Int_StrWidth_u((uintmax_t)(~value + 1), base);
  } else {
    width = Int_StrWidth_u((uintmax_t)value, base);
  }

  return width;
}

static void String_FromInt_u(uintmax_t value, ATB_INT_BASE base,
                             struct atb_StrSpan dest) {
  char *const d_last = atb_AnySpan_End(dest);
  char *d_byte = d_last;

  do {
    *(--d_byte) = m_digits.data[value % base];
    value /= base;
  } while (value != 0);
}

bool atb_String_FromInt_u(uintmax_t value, ATB_INT_BASE base,
                          struct atb_StrSpan dest,
                          struct atb_StrSpan *const remaining,
                          struct atb_Error *const err) {
  assert(INT_BASE_IsValid(base));

  bool success = true;
  size_t value_width = Int_StrWidth_u(value, base);

  if (!atb_StrSpan_IsValid(dest)) {
    if (remaining != NULL) remaining->size = value_width;
  } else if (value_width > dest.size) {
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);
    success = false;
  } else {
    String_FromInt_u(value, base, atb_StrSpan_First(dest, value_width));

    if (remaining != NULL) {
      *remaining =
          atb_StrSpan_Shrink(dest, value_width, K_ATB_SPAN_SHRINK_FRONT);
    }
  }

  return success;
}

bool atb_String_FromInt_i(intmax_t value, ATB_INT_BASE base,
                          struct atb_StrSpan dest,
                          struct atb_StrSpan *const remaining,
                          struct atb_Error *const err) {
  assert(INT_BASE_IsValid(base));

  bool success = true;
  size_t value_width = Int_StrWidth_i(value, base);
  uintmax_t unsigned_value;

  if (!atb_StrSpan_IsValid(dest)) {
    if (remaining != NULL) remaining->size = value_width;
  } else if (value_width > dest.size) {
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);
    success = false;
  } else {
    if (remaining != NULL) {
      *remaining =
          atb_StrSpan_Shrink(dest, value_width, K_ATB_SPAN_SHRINK_FRONT);
    }

    dest = atb_StrSpan_First(dest, value_width);
    if ((value < 0) && (base == K_ATB_INT_DEC)) {
      *dest.data++ = '-';
      dest.size -= 1;
      unsigned_value = (uintmax_t)(~value + 1);
    } else {
      unsigned_value = (uintmax_t)value;
    }

    String_FromInt_u(unsigned_value, base, dest);
  }

  return success;
}

bool atb_String_GetIntBase(struct atb_StrView str, ATB_INT_BASE *const base,
                           struct atb_StrView *const remaining,
                           struct atb_Error *const err) {
  assert(base != NULL);

  bool success = true;
  size_t base_offset = 0;

  if (str.size == 0) {
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_INVALID_ARGUMENT);
    success = false;
  } else if (!isdigit(str.data[0]) && (str.data[0] != '-')) {
    // Neither starts with a digit NOR '-'
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_ARGUMENT_OUT_OF_DOMAIN);
    success = false;
  } else if ((str.data[0] == '-') &&
             ((str.size == 1) || (!isdigit(str.data[1])))) {
    // Starts '-' but either:
    // - of size == 1
    // - followed by something that's not a digit
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_ARGUMENT_OUT_OF_DOMAIN);
    success = false;
  } else if ((str.data[0] == '0') && (str.size > 1)) {
    // Starts with digit '0' of size > 1
    if (isdigit(str.data[1])) {
      // Second char IS a digit
      *base = K_ATB_INT_OCT;
      base_offset = 1;
    } else {
      // Second char IS NOT a digit -> Base qualifier OR single 0
      switch (str.data[1]) {
        case 'x':
        case 'X':
          *base = K_ATB_INT_HEX;
          base_offset = 2;
          break;

        case 'b':
        case 'B':
          *base = K_ATB_INT_BIN;
          base_offset = 2;
          break;

        case 'o':
          *base = K_ATB_INT_OCT;
          base_offset = 2;
          break;

        default:
          // This means that str == "0<SOMETHING NOT DIGIT>", should be parsed
          // as 0
          *base = K_ATB_INT_DEC;
      }
    }
  } else {
    *base = K_ATB_INT_DEC;
  }

  if (success && (remaining != NULL)) {
    *remaining = atb_StrView_Shrink(str, base_offset, K_ATB_SPAN_SHRINK_FRONT);
  }

  return success;
}
