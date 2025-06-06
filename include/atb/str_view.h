#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h> /* NULL */

#include "atb/array.h"
#include "atb/export.h"

/**
 *  \brief Represents a non-owning view of a string (i.e. doesn't end with '\0')
 *  \note This view is not mutable
 */
struct atb_StrView {
  const char *data; /*!< Begin of the view */
  size_t size;      /*!< Size of the view */
};

typedef struct atb_StrView atb_StrView;

#define atb_StrView_Fmt "{.data=%p, .size=%zu}"
#define atb_StrView_FmtVaArg(str) ((void *)(str).data), ((str).size)

#define atb_Str_Fmt "%.*s"
#define atb_Str_FmtVaArg(str) ((int)(str).size), ((str).data)

/***************************************************************************/
/*                                Construct                                */
/***************************************************************************/

/**
 *  \brief Initialize the StrView data struct (NULL)
 */
static inline void atb_StrView_Init(struct atb_StrView *const self);

/**
 *  \return A string view from the given string literal
 *
 *  \param[in] str a string literal (e.g.: "foo")
 */
#define atb_StrView_MakeFromLiteral(str) \
  atb_StrView { atb_Array_Begin(str), (atb_Array_Size(str) - 1) }

/**
 *  \return An empty string view
 */
static inline struct atb_StrView atb_StrView_MakeEmpty(void);

/**
 *  \return A string view from the given NULL terminated string
 *
 *  \param[in] c_str A NULL terminated string
 *  \param[in] max Maximum size (in case c_str is not NULL terminated)
 */
ATB_PUBLIC struct atb_StrView atb_StrView_MakeFromCString(const char *c_str,
                                                          size_t max);

/***************************************************************************/
/*                                Operations                               */
/***************************************************************************/

/* Checks ******************************************************************/

/**
 *  \return True if the given StrView is corrupted (nullptr and size > 0)
 */
static inline bool atb_StrView_IsCorrupted(struct atb_StrView str);

/**
 *  \return True if the given strings memory are overlapping
 *
 *  \important Asserts when lhs or rhs is corrupted
 */
static inline bool atb_StrView_IsOverlapping(struct atb_StrView lhs,
                                             struct atb_StrView rhs);

/* Loops *******************************************************************/

/**
 *  \return const char* Begin of the string range
 *
 *  \important Asserts when str is corrupted
 */
static inline const char *atb_StrView_Begin(struct atb_StrView str);

/**
 *  \return const char* One past the end of the string range
 *
 *  \important Asserts when str is corrupted
 *  \important This iterator is not dereferenceable
 */
static inline const char *atb_StrView_End(struct atb_StrView str);

/**
 *  \brief Iterates over the character of a string view
 *
 *  \param[in] c A const char* used to iterator over the range
 *
 *  \important Asserts when str is corrupted
 */
#define atb_StrView_ForEachChar(c, str) \
  for ((c) = atb_StrView_Begin(str); c < atb_StrView_End(str); ++(c))

/**
 *  \return const char* Begin of the reverse string range
 *
 *  \important Asserts when str is corrupted
 */
static inline const char *atb_StrView_BeginR(struct atb_StrView str);

/**
 *  \return const char* One past the end of the reverse string range
 *
 *  \important Asserts when str is corrupted
 *  \important This iterator is not dereferenceable
 */
static inline const char *atb_StrView_EndR(struct atb_StrView str);

/**
 *  \brief Reverse iterates over the character of a string view
 *
 *  \param[in] c A const char* used to iterator over the range
 *
 *  \important Asserts when str is corrupted
 */
#define atb_StrView_ForEachCharR(c, str) \
  for ((c) = atb_StrView_BeginR(str); c > atb_StrView_EndR(str); --(c))

/* Slicing *****************************************************************/

/**
 *  \return A slice of str by removing 'offset' bytes from the back
 *
 *  \important Asserts when str is corrupted
 *  \note 'offset' is clamped to str.size
 *
 *  \param[in] str The string to slice
 *  \param[in] offset Number of bytes to remove from the back
 */
static inline struct atb_StrView atb_StrView_SliceBack(struct atb_StrView str,
                                                       size_t offset);

/**
 *  \return A slice of str by removing 'offset' bytes from the front
 *
 *  \important Asserts when str is corrupted
 *  \note 'offset' is clamped to str.size
 *
 *  \param[in] str The string to slice
 *  \param[in] offset Number of bytes to remove from the front
 */
static inline struct atb_StrView atb_StrView_SliceFront(struct atb_StrView str,
                                                        size_t offset);

/**
 *  \return A slice of 'size' bytes starting at 'begin' from 'str'
 *
 *  \important Asserts when str is corrupted
 *  \note 'begin' and 'size' are clamped in order to fit into str
 *
 *  \param[in] str The string to slice
 *  \param[in] begin, size The beginning (inside str) and the size of the slice
 */
static inline struct atb_StrView atb_StrView_Slice(struct atb_StrView str,
                                                   size_t begin, size_t size);

/* Comparison **************************************************************/

/**
 *  \brief Result of the lexicographical comparison of 2 strings
 */
typedef enum {
  atb_StrView_Compare_LESS,    /*!< (lhs.size < rhs.size) or lhs is less */
  atb_StrView_Compare_EQUAL,   /*!< (lhs.size == rhs.size) and lhs == rhs */
  atb_StrView_Compare_GREATER, /*!< (lhs.size > rhs.size) or lhs is greater */
} atb_StrView_Compare_Result;

/**
 *  \brief Lexicographically compare 2 strings
 *
 *  \important Asserts when any of lhs/rhs are corrupted
 *
 *  \param[in] lhs, rhs The strings to compare
 *
 *  \return One of [LESS, EQUAL, GREATER] based on the comparison result
 */
ATB_PUBLIC atb_StrView_Compare_Result
atb_StrView_Compare(struct atb_StrView lhs, struct atb_StrView rhs);

/**
 *  \return True when both lhs and rhs are equal
 *
 *  \important Asserts when any of lhs/rhs are corrupted
 */
static inline bool atb_StrView_Eq(struct atb_StrView lhs,
                                  struct atb_StrView rhs);
/**
 *  \return True when both lhs and rhs are not equal
 *
 *  \important Asserts when any of lhs/rhs are corrupted
 */
static inline bool atb_StrView_Ne(struct atb_StrView lhs,
                                  struct atb_StrView rhs);

/**
 *  \return True when lhs is striclty greater than rhs
 *
 *  \important Asserts when any of lhs/rhs are corrupted
 */
static inline bool atb_StrView_Gt(struct atb_StrView lhs,
                                  struct atb_StrView rhs);

/**
 *  \return True when lhs is striclty less than rhs
 *
 *  \important Asserts when any of lhs/rhs are corrupted
 */
static inline bool atb_StrView_Lt(struct atb_StrView lhs,
                                  struct atb_StrView rhs);

/**
 *  \return True when lhs is greater or equal than rhs
 *
 *  \important Asserts when any of lhs/rhs are corrupted
 */
static inline bool atb_StrView_Ge(struct atb_StrView lhs,
                                  struct atb_StrView rhs);
/**
 *  \return True when lhs is less or equal than rhs
 *
 *  \important Asserts when any of lhs/rhs are corrupted
 */
static inline bool atb_StrView_Le(struct atb_StrView lhs,
                                  struct atb_StrView rhs);

/* Lookup ******************************************************************/

/**
 *  \brief Try to find the FIRST appearence of substr inside str
 *
 *  \important Asserts when any of str/substr are corrupted
 *
 *  \param[in] str Main string to look into
 *  \param[in] substr Sub-string to look for
 *
 *  \return A view of str starting at the location where substr has been found
 *          or an empty view pointing at the end of str if lookup failed
 */
ATB_PUBLIC struct atb_StrView atb_StrView_Find(struct atb_StrView str,
                                               struct atb_StrView substr);

/**
 *  \brief Try to find the LAST appearence of substr inside str
 *
 *  \important Asserts when any of str/substr are corrupted
 *
 *  \param[in] str Main string to look into
 *  \param[in] substr Sub-string to look for
 *
 *  \return A view of str starting at the location where substr has been found
 *          or an empty view pointing at the end of str if lookup failed
 */
ATB_PUBLIC struct atb_StrView atb_StrView_FindR(struct atb_StrView str,
                                                struct atb_StrView substr);

/***************************************************************************/
/*                            Static definitions                           */
/***************************************************************************/
static inline void atb_StrView_Init(struct atb_StrView *const self) {
  self->data = NULL;
  self->size = 0;
}

static inline struct atb_StrView atb_StrView_MakeEmpty(void) {
  struct atb_StrView out;
  atb_StrView_Init(&out);
  return out;
}

static inline bool atb_StrView_IsCorrupted(struct atb_StrView str) {
  return (str.data == NULL) && (str.size > 0);
}

static inline bool atb_StrView_IsOverlapping(struct atb_StrView lhs,
                                             struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  if ((lhs.size == 0) || (rhs.size == 0)) {
    return false;
  }

  struct atb_StrView const *low_memory = NULL;
  struct atb_StrView const *high_memory = NULL;

  if (lhs.data <= rhs.data) {
    low_memory = &lhs;
    high_memory = &rhs;
  } else {
    low_memory = &rhs;
    high_memory = &lhs;
  }

  return (atb_StrView_End(*low_memory) > atb_StrView_Begin(*high_memory));
}

static inline const char *atb_StrView_Begin(struct atb_StrView str) {
  assert(!atb_StrView_IsCorrupted(str));

  return str.data;
}

static inline const char *atb_StrView_End(struct atb_StrView str) {
  assert(!atb_StrView_IsCorrupted(str));

  return str.data + str.size;
}

static inline const char *atb_StrView_BeginR(struct atb_StrView str) {
  assert(!atb_StrView_IsCorrupted(str));

  return str.data + str.size - 1;
}
static inline const char *atb_StrView_EndR(struct atb_StrView str) {
  assert(!atb_StrView_IsCorrupted(str));

  return str.data - 1;
}

static inline struct atb_StrView atb_StrView_SliceBack(struct atb_StrView str,
                                                       size_t offset) {
  assert(!atb_StrView_IsCorrupted(str));

  if (offset > str.size) {
    offset = str.size;
  }

  str.size -= offset;

  return str;
}

static inline struct atb_StrView atb_StrView_SliceFront(struct atb_StrView str,
                                                        size_t offset) {
  assert(!atb_StrView_IsCorrupted(str));

  if (offset > str.size) {
    offset = str.size;
  }

  str.data += offset;
  str.size -= offset;

  return str;
}

static inline struct atb_StrView atb_StrView_Slice(struct atb_StrView str,
                                                   size_t begin, size_t size) {
  assert(!atb_StrView_IsCorrupted(str));

  str = atb_StrView_SliceFront(str, begin);
  if (size < str.size) {
    str.size = size;
  }

  return str;
}

static inline bool atb_StrView_Eq(struct atb_StrView lhs,
                                  struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  switch (atb_StrView_Compare(lhs, rhs)) {
    case atb_StrView_Compare_EQUAL: return true;
    default: return false;
  }
}

static inline bool atb_StrView_Ne(struct atb_StrView lhs,
                                  struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  switch (atb_StrView_Compare(lhs, rhs)) {
    case atb_StrView_Compare_LESS:
    case atb_StrView_Compare_GREATER: return true;
    default: return false;
  }
}

static inline bool atb_StrView_Gt(struct atb_StrView lhs,
                                  struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  switch (atb_StrView_Compare(lhs, rhs)) {
    case atb_StrView_Compare_GREATER: return true;
    default: return false;
  }
}

static inline bool atb_StrView_Lt(struct atb_StrView lhs,
                                  struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  switch (atb_StrView_Compare(lhs, rhs)) {
    case atb_StrView_Compare_LESS: return true;
    default: return false;
  }
}

static inline bool atb_StrView_Ge(struct atb_StrView lhs,
                                  struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  switch (atb_StrView_Compare(lhs, rhs)) {
    case atb_StrView_Compare_EQUAL:
    case atb_StrView_Compare_GREATER: return true;
    default: return false;
  }
}

static inline bool atb_StrView_Le(struct atb_StrView lhs,
                                  struct atb_StrView rhs) {
  assert(!atb_StrView_IsCorrupted(lhs));
  assert(!atb_StrView_IsCorrupted(rhs));

  switch (atb_StrView_Compare(lhs, rhs)) {
    case atb_StrView_Compare_EQUAL:
    case atb_StrView_Compare_LESS: return true;
    default: return false;
  }
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
