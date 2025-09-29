#include "atb/error.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#include "atb/array.h"

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

typedef enum {
  K_INT_DECIMAL = 10,
  K_INT_HEXADECIMAL = 16,
} INT_BASE;

static size_t IntToString_Width_10_u(uintmax_t value) {
  size_t width = 1;

  if (value != 0) {
    width += (size_t)log10((double)value);
  }

  return width;
}

static size_t IntToString_Width_10_i(intmax_t value) {
  size_t width = 0;

  if (value < 0) {
    width = 1 + IntToString_Width_10_u((uintmax_t)(~value + 1));
  } else {
    width = IntToString_Width_10_u((uintmax_t)value);
  }

  return width;
}

static char *IntToString_u(uintmax_t value, INT_BASE base, char *d_first,
                           size_t d_size) {
  const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  assert((1 < base) && (base < atb_Array_Size(digits) - 1));

  char *const d_last = d_first + d_size;
  char *d_byte = d_last;

  do {
    *(--d_byte) = digits[value % base];
    value /= base;
  } while ((value != 0) && (d_byte >= d_first));

  /* if value != 0, it means that we reached the end of the buffer before ending
   * the convertion... */
  assert(value == 0);

  return d_byte;
}

static char *IntToString_i(intmax_t value, INT_BASE base, char *d_first,
                           size_t d_size) {
  if (value < 0) {
    *d_first++ = '-';
    d_size -= 1;

    return IntToString_u((uintmax_t)(~value + 1), base, d_first, d_size);
  } else {
    return IntToString_u((uintmax_t)value, base, d_first, d_size);
  }
}

struct String {
  size_t size;
  char data[1 << 6];
};

struct StringView {
  char const *data;
  size_t size;
};

#define StringView_FromLiteral(str) \
  { .data = str, .size = (atb_Array_Size(str) - 1) }

static bool StringView_Set(struct StringView *const view,
                           char const *const other, size_t size) {
  assert(view != NULL);
  assert(other != NULL);

  view->data = other;
  view->size = size;

  return true;
}

static bool StringView_Set_FromString(struct StringView *const view,
                                      struct String const *const other) {
  assert(other != NULL);
  return StringView_Set(view, other->data, other->size);
}

static bool StringView_Set_FromNullTerminated(struct StringView *const view,
                                              char const *const other) {
  assert(other != NULL);
  return StringView_Set(view, other, strlen(other));
}

static bool StringView_ShrinkFront(struct StringView *const view,
                                   size_t offset) {
  assert(view != NULL);

  offset = MIN(view->size, offset);
  view->data += offset;
  view->size -= offset;

  return true;
}

static bool StringView_ShrinkBack(struct StringView *const view,
                                  size_t offset) {
  assert(view != NULL);

  view->size -= MIN(view->size, offset);
  return true;
}

static bool StringView_Slice(struct StringView *const view, size_t offset,
                             size_t new_size) {
  return StringView_ShrinkFront(view, offset) &&
         StringView_ShrinkBack(view, view->size - new_size);
}

static bool StringView_CopyInto(struct StringView const *const view,
                                bool truncate_view, char *d_first,
                                size_t d_size, size_t *const written) {
  assert(view != NULL);
  assert(d_first != NULL);
  assert(written != NULL);

  bool success = true;

  if (view->size <= d_size) {
    memcpy(d_first, view->data, view->size);
    *written = view->size;
  } else if (truncate_view) {
    memcpy(d_first, view->data, d_size);
    *written = d_size;
  } else {
    success = false;
  }

  return success;
}

static size_t String_Capacity(struct String const *const str) {
  assert(str != NULL);
  return atb_Array_Size(str->data);
}

#define String_FromLiteral(str) \
  { .size = (atb_Array_Size(str) - 1), .data = str, }

static bool String_Set_From(struct String *const str, char const *const other,
                            size_t other_size, bool truncate_other) {
  assert(str != NULL);
  assert(other != NULL);

  bool success = false;

  if (truncate_other) {
    other_size = MIN(other_size, String_Capacity(str));
  }

  if (other_size <= String_Capacity(str)) {
    memcpy(str->data, other, other_size);
    str->size = other_size;
    success = true;
  }

  return success;
}

static bool String_Set_FromNullTerminated(struct String *const str,
                                          char const *const other,
                                          bool truncate_other) {
  assert(str != NULL);
  assert(other != NULL);
  return String_Set_From(str, other, strlen(other), truncate_other);
}

static void String_Clear(struct String *const str) {
  assert(str != NULL);

  str->size = 0;
  memset(str->data, '\0', String_Capacity(str));
}

static bool String_CopyInto(struct String const *const str, bool truncate_str,
                            char *d_first, size_t d_size,
                            size_t *const written) {
  struct StringView view;
  return StringView_Set_FromString(&view, str) &&
         StringView_CopyInto(&view, truncate_str, d_first, d_size, written);
}

static bool ErrorFormatter_IsValid(
    struct atb_ErrorFormatter const *const self) {
  return (self != NULL) && (self->Describe != NULL);
}

static bool ErrorFormatter_Describe(struct atb_ErrorFormatter *const self,
                                    atb_ErrorCode_t code, char *d_first,
                                    size_t d_size, size_t *written) {
  assert(ErrorFormatter_IsValid(self));
  return self->Describe(self->data, code, d_first, d_size, written);
}

static bool UnknownError_Describe(struct atb_Error const *const err,
                                  char *d_first, size_t d_size,
                                  size_t *written) {
  assert(written != NULL);

  bool success = true;

  const struct StringView category_header = StringView_FromLiteral("0x");
  const size_t category_value_size = sizeof(atb_ErrorCategory_t) * 2;
  const struct StringView category_footer = StringView_FromLiteral(": ");

  const size_t expected_category_size =
      category_header.size + category_value_size + category_footer.size;

  const size_t expected_size =
      expected_category_size + IntToString_Width_10_i(err->code);

  if (d_first == NULL) {
    *written = expected_size;
  } else if (d_size < expected_size) {
    success = false;
  } else {
    /* NOTE:
     * No checks on return values NOR we update d_size since we know for sure
     * that the size is big enough
     */

    StringView_CopyInto(&category_header, false, d_first, category_header.size,
                        written);
    d_first += *written;

    /* IntToString always fills the buffer BACKWARD AND returns the last byte
     * written, in case of we didn't fill the buffer... */
    char *end = IntToString_u(err->category, K_INT_HEXADECIMAL, d_first,
                              category_value_size);
    /* ... It can then be used to fill the remaining bytes with '0' */
    while (end-- != d_first) *end = '0';

    /* Advance at the end of the category section */
    d_first += category_value_size;

    StringView_CopyInto(&category_footer, false, d_first, category_footer.size,
                        written);
    d_first += *written;

    IntToString_i(err->code, K_INT_DECIMAL, d_first,
                  expected_size - expected_category_size);

    *written = expected_size;
  }

  return success;
}

static bool RawError_Describe(void *data, atb_ErrorCode_t code, char *d_first,
                              size_t d_size, size_t *written) {
  (void)data;

  assert(written != NULL);

  bool success = true;
  const size_t expected_size = IntToString_Width_10_i(code);

  if (d_first == NULL) {
    *written = expected_size;
  } else if (d_size < expected_size) {
    success = false;
  } else {
    IntToString_i(code, K_INT_DECIMAL, d_first, expected_size);
    *written = expected_size;
  }

  return success;
}

static bool GenericError_Describe(void *self, atb_ErrorCode_t code,
                                  char *d_first, size_t d_size,
                                  size_t *written) {
  (void)self;

  assert(written != NULL);

  bool success = true;
  char const *const generic_descr = strerror(code);
  const size_t generic_descr_size = strlen(generic_descr);

  if (d_first == NULL) {
    *written = generic_descr_size;
  } else if (d_size < generic_descr_size) {
    success = false;
  } else {
    memcpy(d_first, generic_descr, generic_descr_size);
    *written = generic_descr_size;
  }

  return success;
}

struct NamedErrorFormatter {
  struct String name;
  struct atb_ErrorFormatter fmt;
};

static struct NamedErrorFormatter *NamedErrorFormatter_Get(
    atb_ErrorCategory_t cat) {
  static struct NamedErrorFormatter
      m_formatters[1 << ((sizeof(atb_ErrorCategory_t)) * 8)] = {
          [K_ATB_ERROR_RAW] =
              {
                  .name = String_FromLiteral("raw"),
                  .fmt =
                      {
                          .data = NULL,
                          .Describe = RawError_Describe,
                      },
              },
          [K_ATB_ERROR_GENERIC] =
              {
                  .name = String_FromLiteral("generic"),
                  .fmt =
                      {
                          .data = NULL,
                          .Describe = GenericError_Describe,
                      },
              },
      };

  return &(m_formatters[cat]);
}

static void NamedErrorFormatter_Clear(struct NamedErrorFormatter *const self) {
  assert(self != NULL);

  String_Clear(&(self->name));
  self->fmt = (struct atb_ErrorFormatter){
      NULL,
      NULL,
  };
}

static bool NamedErrorFormatter_IsValid(
    struct NamedErrorFormatter const *const self) {
  return (self != NULL) && (ErrorFormatter_IsValid(&(self->fmt)));
}

static bool NamedErrorFormatter_Describe(struct NamedErrorFormatter *const self,
                                         atb_ErrorCode_t code, char *d_first,
                                         size_t d_size, size_t *written) {
  assert(NamedErrorFormatter_IsValid(self));
  assert(written != NULL);

  const struct StringView separator = StringView_FromLiteral(": ");
  bool success = true;

  size_t expected_size = 0;
  ErrorFormatter_Describe(&(self->fmt), code, NULL, 0, &expected_size);
  expected_size += self->name.size + separator.size;

  if (d_first == NULL) {
    *written = expected_size;
  } else if (d_size < expected_size) {
    success = false;
  } else {
    /* NOTE:
     * No checks on return values NOR we update d_size since we know for sure
     * that the size is big enough
     */

    String_CopyInto(&(self->name), false, d_first, self->name.size, written);
    d_first += *written;
    d_size -= *written;

    StringView_CopyInto(&separator, false, d_first, separator.size, written);
    d_first += *written;
    d_size -= *written;

    ErrorFormatter_Describe(&(self->fmt), code, d_first, d_size, written);

    *written = expected_size;
  }

  return success;
}

bool atb_ErrorCategory_HasFormatter(atb_ErrorCategory_t category) {
  return NamedErrorFormatter_IsValid(NamedErrorFormatter_Get(category));
}

bool atb_ErrorCategory_AddFormatter(atb_ErrorCategory_t category,
                                    const char *name,
                                    struct atb_ErrorFormatter fmt,
                                    struct atb_Error *const err) {
  assert(name != NULL);
  assert(fmt.Describe != NULL);

  bool success = false;

  struct NamedErrorFormatter *named_fmt = NamedErrorFormatter_Get(category);

  if (NamedErrorFormatter_IsValid(named_fmt)) {
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_INVALID_ARGUMENT);
  } else if (!String_Set_FromNullTerminated(&(named_fmt->name), name, false)) {
    atb_GenericError_Set(err, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);
  } else {
    named_fmt->fmt = fmt;
    success = true;
  }

  return success;
}

bool atb_ErrorCategory_RemoveFormatter(atb_ErrorCategory_t category) {
  bool success = false;

  struct NamedErrorFormatter *fmt = NamedErrorFormatter_Get(category);
  if (NamedErrorFormatter_IsValid(fmt)) {
    NamedErrorFormatter_Clear(fmt);
    success = true;
  }

  return success;
}

bool atb_Error_Describe(struct atb_Error const *const self, char *d_first,
                        size_t d_size, size_t *written) {
  assert(self != NULL);
  assert(written != NULL);

  bool success = false;

  struct NamedErrorFormatter *fmt = NamedErrorFormatter_Get(self->category);
  if (atb_ErrorCategory_HasFormatter(self->category)) {
    success =
        NamedErrorFormatter_Describe(fmt, self->code, d_first, d_size, written);
  } else {
    success = UnknownError_Describe(self, d_first, d_size, written);
  }
  return success;
}
