#include "atb/string.h"

#include "math.h"

static const struct atb_StrView m_digits =
    atb_StrView_From_StrLiteral_INIT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static bool INT_BASE_IsValid(ATB_INT_BASE base) {
  return (2 <= base) && (base <= m_digits.size);
}

static size_t Int_StrWidth_u(uintmax_t value, ATB_INT_BASE base) {
  size_t width = 1;

  if (value != 0) {
    switch (base) {
      case K_ATB_INT_BIN:
        width += (size_t)log2((double)value);
        break;
      case K_ATB_INT_DEC:
        width += (size_t)log10((double)value);
        break;
      default:
        /* Count by hands... */
        while ((value /= base) != 0) {
          width += 1;
        }
    }
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
