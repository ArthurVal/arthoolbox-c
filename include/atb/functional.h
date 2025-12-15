#pragma once

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************************/
/*                                 CALLABLE                                */
/***************************************************************************/

/* DECLARE *****************************************************************/

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

/* CTOR - BIND *************************************************************/

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

/* IS_VALID ****************************************************************/

/// Returns TRUE whenever the given \a CALLABLE is valid, i.e. it's .fn member
/// is not NULL
#define ATB_CALLABLE_IS_VALID(CALLABLE) ((CALLABLE).fn != NULL)

/* INVOKE ******************************************************************/

/// Invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn')
/// WARNING: This DOESN'T check that \a CALLABLE 's fn is not NULL
#define ATB_INVOKE_UNSAFELY(CALLABLE, ...) \
  ((CALLABLE).fn((CALLABLE).data, ##__VA_ARGS__))

/// Safely invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn').
/// Return value is ignored.
#define ATB_INVOKE(CALLABLE, ...)                   \
  do {                                              \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {          \
      ATB_INVOKE_UNSAFELY(CALLABLE, ##__VA_ARGS__); \
    }                                               \
  } while (0)

/// Safely invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn').
/// Return value is assigned to DEST.
#define ATB_INVOKE_R(DEST, CALLABLE, ...)                    \
  do {                                                       \
    if (ATB_CALLABLE_IS_VALID(CALLABLE)) {                   \
      (DEST) = ATB_INVOKE_UNSAFELY(CALLABLE, ##__VA_ARGS__); \
    }                                                        \
  } while (0)

/// Safely invoke a \a CALLABLE (i.e. struct with member '.data' and '.fn').
/// Return value set to \a DEFAULT_VALUE when \a CALLABLE is not valid.
#define ATB_INVOKE_DEFAULT(DEFAULT, CALLABLE, ...)    \
  (ATB_CALLABLE_IS_VALID(CALLABLE)                    \
       ? ATB_INVOKE_UNSAFELY(CALLABLE, ##__VA_ARGS__) \
       : (DEFAULT))

/***************************************************************************/
/*                                  UTILS                                  */
/***************************************************************************/

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
