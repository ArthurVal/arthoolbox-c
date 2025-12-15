#pragma once

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/// Declare ANY callable with a function pointer \a fn following the 'Callable
/// traits', i.e. the first argument will be \a data that is always forwarded;
///
/// Example:
/// ATB_CALLABLE_DECLARE_2(bool, Foo, int, float) give the following
/// signature: f(void*, int, float) -> bool
///
/// ATB_CALLABLE_DECLARE_0(void, Bar) give the following signature:
/// f(void*) -> void
///
/// ATB_CALLABLE_DECLARE_1(int, Baz, int) give the following signature:
/// f(void*, int) -> int
#define ATB_CALLABLE_DECLARE(R, NAME, ...) \
  struct NAME {                            \
    R (*fn)(void *data, ##__VA_ARGS__);    \
    void *data;                            \
  }

#define ATB_CALLABLE_DECLARE_0(R, NAME) \
  struct NAME {                         \
    R (*fn)(void *data);                \
    void *data;                         \
  }

#define ATB_CALLABLE_DECLARE_1(R, NAME, _1) \
  struct NAME {                             \
    R (*fn)(void *data, _1);                \
    void *data;                             \
  }

#define ATB_CALLABLE_DECLARE_2(R, NAME, _1, _2) \
  struct NAME {                                 \
    R (*fn)(void *data, _1, _2);                \
    void *data;                                 \
  }

#define ATB_CALLABLE_DECLARE_3(R, NAME, _1, _2, _3) \
  struct NAME {                                     \
    R (*fn)(void *data, _1, _2, _3);                \
    void *data;                                     \
  }

#define ATB_CALLABLE_DECLARE_4(R, NAME, _1, _2, _3, _4) \
  struct NAME {                                         \
    R (*fn)(void *data, _1, _2, _3, _4);                \
    void *data;                                         \
  }

#define ATB_CALLABLE_DECLARE_5(R, NAME, _1, _2, _3, _4, _5) \
  struct NAME {                                             \
    R (*fn)(void *data, _1, _2, _3, _4, _5);                \
    void *data;                                             \
  }

#define ATB_CALLABLE_DECLARE_6(R, NAME, _1, _2, _3, _4, _5, _6) \
  struct NAME {                                                 \
    R (*fn)(void *data, _1, _2, _3, _4, _5, _6);                \
    void *data;                                                 \
  }

#define ATB_CALLABLE_DECLARE_7(R, NAME, _1, _2, _3, _4, _5, _6, _7) \
  struct NAME {                                                     \
    R (*fn)(void *data, _1, _2, _3, _4, _5, _6, _7);                \
    void *data;                                                     \
  }

#define ATB_CALLABLE_DECLARE_8(R, NAME, _1, _2, _3, _4, _5, _6, _7, _8) \
  struct NAME {                                                         \
    R (*fn)(void *data, _1, _2, _3, _4, _5, _6, _7, _8);                \
    void *data;                                                         \
  }

/// Binds \a FUNCTION and \a DATA to ANY callable.
/// Can be used as Static Initializer
#define ATB_INIT_BIND(FUNCTION, DATA) \
  { .fn = FUNCTION, .data = (void *)DATA }

/// Binds \a F and \a D to \a TYPE as a Compound litteral
#define ATB_BIND_AS(TYPE, F, D) (TYPE) ATB_INIT_BIND((F), (D))

/// Binds to the NULL/NULL function/data pair.
#define K_ATB_INIT_BIND_NULL ATB_INIT_BIND(NULL, NULL)

/// Create a NULL erems_Callable
#define K_ATB_BIND_NULL_AS(TYPE) (TYPE) K_ATB_INIT_BIND_NULL

/// Returns TRUE whenever the given \a CALLABLE is valid, i.e. it's .fn member
/// is not NULL
#define ATB_CALLABLE_IS_VALID(CALLABLE) ((CALLABLE).fn != NULL)

/// Invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn')
/// WARNING: This DOESN'T check that \a CALLABLE 's fn is not NULL
#define ATB_INVOKE_UNSAFELY(CALLABLE, ...) \
  ((CALLABLE).fn((CALLABLE).data, ##__VA_ARGS__))

#define ATB_INVOKE_UNSAFELY_0(CALLABLE) ((CALLABLE).fn((CALLABLE).data))

#define ATB_INVOKE_UNSAFELY_1(CALLABLE, _1) ((CALLABLE).fn((CALLABLE).data, _1))

#define ATB_INVOKE_UNSAFELY_2(CALLABLE, _1, _2) \
  ((CALLABLE).fn((CALLABLE).data, _1, _2))

#define ATB_INVOKE_UNSAFELY_3(CALLABLE, _1, _2, _3) \
  ((CALLABLE).fn((CALLABLE).data, _1, _2, _3))

#define ATB_INVOKE_UNSAFELY_4(CALLABLE, _1, _2, _3, _4) \
  ((CALLABLE).fn((CALLABLE).data, _1, _2, _3, _4))

#define ATB_INVOKE_UNSAFELY_5(CALLABLE, _1, _2, _3, _4, _5) \
  ((CALLABLE).fn((CALLABLE).data, _1, _2, _3, _4, _5))

#define ATB_INVOKE_UNSAFELY_6(CALLABLE, _1, _2, _3, _4, _5, _6) \
  ((CALLABLE).fn((CALLABLE).data, _1, _2, _3, _4, _5, _6))

#define ATB_INVOKE_UNSAFELY_7(CALLABLE, _1, _2, _3, _4, _5, _6, _7) \
  ((CALLABLE).fn((CALLABLE).data, _1, _2, _3, _4, _5, _6, _7))

#define ATB_INVOKE_UNSAFELY_8(CALLABLE, _1, _2, _3, _4, _5, _6, _7, _8) \
  ((CALLABLE).fn((CALLABLE).data, _1, _2, _3, _4, _5, _6, _7, _8))

/// Safely invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn').
/// Return value is ignored.
#define ATB_INVOKE(CALLABLE, ...)                   \
  do {                                              \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {          \
      ATB_INVOKE_UNSAFELY(CALLABLE, ##__VA_ARGS__); \
    }                                               \
  } while (0)

#define ATB_INVOKE_0(CALLABLE)             \
  do {                                     \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) { \
      ATB_INVOKE_UNSAFELY_0(CALLABLE);     \
    }                                      \
  } while (0)

#define ATB_INVOKE_1(CALLABLE, _1)         \
  do {                                     \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) { \
      ATB_INVOKE_UNSAFELY_1(CALLABLE, _1); \
    }                                      \
  } while (0)

#define ATB_INVOKE_2(CALLABLE, _1, _2)         \
  do {                                         \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {     \
      ATB_INVOKE_UNSAFELY_2(CALLABLE, _1, _2); \
    }                                          \
  } while (0)

#define ATB_INVOKE_3(CALLABLE, _1, _2, _3)         \
  do {                                             \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {         \
      ATB_INVOKE_UNSAFELY_3(CALLABLE, _1, _2, _3); \
    }                                              \
  } while (0)

#define ATB_INVOKE_4(CALLABLE, _1, _2, _3, _4)         \
  do {                                                 \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {             \
      ATB_INVOKE_UNSAFELY_4(CALLABLE, _1, _2, _3, _4); \
    }                                                  \
  } while (0)

#define ATB_INVOKE_5(CALLABLE, _1, _2, _3, _4, _5)         \
  do {                                                     \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                 \
      ATB_INVOKE_UNSAFELY_5(CALLABLE, _1, _2, _3, _4, _5); \
    }                                                      \
  } while (0)

#define ATB_INVOKE_6(CALLABLE, _1, _2, _3, _4, _5, _6)         \
  do {                                                         \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                     \
      ATB_INVOKE_UNSAFELY_6(CALLABLE, _1, _2, _3, _4, _5, _6); \
    }                                                          \
  } while (0)

#define ATB_INVOKE_7(CALLABLE, _1, _2, _3, _4, _5, _6, _7)         \
  do {                                                             \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                         \
      ATB_INVOKE_UNSAFELY_7(CALLABLE, _1, _2, _3, _4, _5, _6, _7); \
    }                                                              \
  } while (0)

#define ATB_INVOKE_8(CALLABLE, _1, _2, _3, _4, _5, _6, _7, _8)         \
  do {                                                                 \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                             \
      ATB_INVOKE_UNSAFELY_8(CALLABLE, _1, _2, _3, _4, _5, _6, _7, _8); \
    }                                                                  \
  } while (0)

/// Safely invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn').
/// Return value is assigned to DEST.
#define ATB_INVOKE_R(DEST, CALLABLE, ...)                    \
  do {                                                       \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                   \
      (DEST) = ATB_INVOKE_UNSAFELY(CALLABLE, ##__VA_ARGS__); \
    }                                                        \
  } while (0)

#define ATB_INVOKE_R_0(DEST, CALLABLE)          \
  do {                                          \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {      \
      (DEST) = ATB_INVOKE_UNSAFELY_0(CALLABLE); \
    }                                           \
  } while (0)

#define ATB_INVOKE_R_1(DEST, CALLABLE, _1)          \
  do {                                              \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {          \
      (DEST) = ATB_INVOKE_UNSAFELY_1(CALLABLE, _1); \
    }                                               \
  } while (0)

#define ATB_INVOKE_R_2(DEST, CALLABLE, _1, _2)          \
  do {                                                  \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {              \
      (DEST) = ATB_INVOKE_UNSAFELY_2(CALLABLE, _1, _2); \
    }                                                   \
  } while (0)

#define ATB_INVOKE_R_3(DEST, CALLABLE, _1, _2, _3)          \
  do {                                                      \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                  \
      (DEST) = ATB_INVOKE_UNSAFELY_3(CALLABLE, _1, _2, _3); \
    }                                                       \
  } while (0)

#define ATB_INVOKE_R_4(DEST, CALLABLE, _1, _2, _3, _4)          \
  do {                                                          \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                      \
      (DEST) = ATB_INVOKE_UNSAFELY_4(CALLABLE, _1, _2, _3, _4); \
    }                                                           \
  } while (0)

#define ATB_INVOKE_R_5(DEST, CALLABLE, _1, _2, _3, _4, _5)          \
  do {                                                              \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                          \
      (DEST) = ATB_INVOKE_UNSAFELY_5(CALLABLE, _1, _2, _3, _4, _5); \
    }                                                               \
  } while (0)

#define ATB_INVOKE_R_6(DEST, CALLABLE, _1, _2, _3, _4, _5, _6)          \
  do {                                                                  \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                              \
      (DEST) = ATB_INVOKE_UNSAFELY_6(CALLABLE, _1, _2, _3, _4, _5, _6); \
    }                                                                   \
  } while (0)

#define ATB_INVOKE_R_7(DEST, CALLABLE, _1, _2, _3, _4, _5, _6, _7)          \
  do {                                                                      \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                                  \
      (DEST) = ATB_INVOKE_UNSAFELY_7(CALLABLE, _1, _2, _3, _4, _5, _6, _7); \
    }                                                                       \
  } while (0)

#define ATB_INVOKE_R_8(DEST, CALLABLE, _1, _2, _3, _4, _5, _6, _7, _8)     \
  do {                                                                     \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                                 \
      (DEST) =                                                             \
          ATB_INVOKE_UNSAFELY_8(CALLABLE, _1, _2, _3, _4, _5, _6, _7, _8); \
    }                                                                      \
  } while (0)

/// Safely invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn').
/// Return value set to \a DEFAULT_VALUE when \a CALLABLE is not valid.
#define ATB_INVOKE_DEFAULT(DEFAULT, CALLABLE, ...)    \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                    \
       ? ATB_INVOKE_UNSAFELY(CALLABLE, ##__VA_ARGS__) \
       : (DEFAULT))

#define ATB_INVOKE_DEFAULT_0(DEFAULT, CALLABLE)                      \
  (ATB_CALLABLE_IS_VALID(CALLABLE) ? ATB_INVOKE_UNSAFELY_0(CALLABLE) \
                                   : (DEFAULT))

#define ATB_INVOKE_DEFAULT_1(DEFAULT, CALLABLE, _1)                      \
  (ATB_CALLABLE_IS_VALID(CALLABLE) ? ATB_INVOKE_UNSAFELY_1(CALLABLE, _1) \
                                   : (DEFAULT))

#define ATB_INVOKE_DEFAULT_2(DEFAULT, CALLABLE, _1, _2)                      \
  (ATB_CALLABLE_IS_VALID(CALLABLE) ? ATB_INVOKE_UNSAFELY_2(CALLABLE, _1, _2) \
                                   : (DEFAULT))

#define ATB_INVOKE_DEFAULT_3(DEFAULT, CALLABLE, _1, _2, _3) \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                          \
       ? ATB_INVOKE_UNSAFELY_3(CALLABLE, _1, _2, _3)        \
       : (DEFAULT))

#define ATB_INVOKE_DEFAULT_4(DEFAULT, CALLABLE, _1, _2, _3, _4) \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                              \
       ? ATB_INVOKE_UNSAFELY_4(CALLABLE, _1, _2, _3, _4)        \
       : (DEFAULT))

#define ATB_INVOKE_DEFAULT_5(DEFAULT, CALLABLE, _1, _2, _3, _4, _5) \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                                  \
       ? ATB_INVOKE_UNSAFELY_5(CALLABLE, _1, _2, _3, _4, _5)        \
       : (DEFAULT))

#define ATB_INVOKE_DEFAULT_6(DEFAULT, CALLABLE, _1, _2, _3, _4, _5, _6) \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                                      \
       ? ATB_INVOKE_UNSAFELY_6(CALLABLE, _1, _2, _3, _4, _5, _6)        \
       : (DEFAULT))

#define ATB_INVOKE_DEFAULT_7(DEFAULT, CALLABLE, _1, _2, _3, _4, _5, _6, _7) \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                                          \
       ? ATB_INVOKE_UNSAFELY_7(CALLABLE, _1, _2, _3, _4, _5, _6, _7)        \
       : (DEFAULT))

#define ATB_INVOKE_DEFAULT_8(DEFAULT, CALLABLE, _1, _2, _3, _4, _5, _6, _7, \
                             _8)                                            \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                                          \
       ? ATB_INVOKE_UNSAFELY_8(CALLABLE, _1, _2, _3, _4, _5, _6, _7, _8)    \
       : (DEFAULT))

/// Functionnal helper use to evaluate \a DO in a boolean chain call (a && b &&
/// c && ...) ONLY WHEN \a PRED evaluates to TRUE (otherwise continue and skip

/// \a DO). The result of evaluating \a DO is then use to continue or not the
/// chain call.
///
/// Example:
/// bool succees = Foo(...)
///                && Bar(...)
///                && WHEN(i == 2, Baz(...)) // <= If Baz() returns false, stop
///                && Toto(...)
///                && ...;
///
/// WARNING: \a DO is expected to evaluate to a BOOLEAN.
#define ATB_WHEN(PRED, DO) (!(PRED) || (DO))

/// Functionnal helper use to ALWAYS evaluate \a DO in a boolean chain
/// call (a && b && c && ...) and then ALWAYS CONTINUE, ignoring the result of
/// \a DO.
///
/// This is usefull when \a DO doesn't evaluate to a boolean.
///
/// Example:
/// void Baz(...);
/// bool success = Foo(...)
///                && Bar(...)
///                && DO(Baz(...)) // <== Baz() result ignored in the chain
///                && Toto(...)
///                && ...;
#define ATB_DO(DO) (DO, true)

/// Functionnal helper use to ALWAYS evaluate \a DO in a boolean chain call (a
/// && b && c && ...) and then ALWAYS ENDS the chain, ignoring the result of \a
/// DO.
///
/// This is usefull when \a DO doesn't evaluate to a boolean.
///
/// Example:
/// void Baz(...);
/// bool success = Foo(...)
///                && Bar(...)
///                && FAILS(Baz(...)) // <== Stop here, sucess will be false
///                && Toto(...) // <== Never called
///                && ...;
#define ATB_FAILS(DO) (DO, false)

#if defined(__cplusplus)
} /* extern "C" */
#endif
