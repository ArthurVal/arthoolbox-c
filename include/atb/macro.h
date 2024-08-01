#pragma once

#if defined(__cplusplus)
#include <type_traits>
#endif

/**
 *  \def atb_COMPOUND_LITERAL
 *
 *  \brief Use to declare a C compound literal without C++ warning
 *
 *  \param[in] type The struct type (do not include struct in C)
 */
#if defined(__cplusplus)
#define atb_COMPOUND_LITERAL(type) type
#else
#define atb_COMPOUND_LITERAL(type) (struct type)
#endif

/**
 *  \def atb_TYPEOF
 *
 *  \brief Use to get the type of an expression \a expr
 *
 *  \param[in] expr A valid expression (variable name, function call ...)
 */
#if defined(__cplusplus)
#define atb_TYPEOF(expr) decltype(expr)
#else
#define atb_TYPEOF(expr) __typeof(expr)
#endif

/**
 *  \def atb_IS_ARRAY
 *
 *  \brief Use to check, at compile time, if \a expr is an array
 *
 *  \param[in] expr A valid expression (variable name, ...)
 */
#if defined(__cplusplus)
#define atb_IS_ARRAY(expr) std::is_array<atb_TYPEOF(expr)>::value
#else
#define atb_IS_ARRAY(expr)                                                     \
  _Generic(&(expr), atb_TYPEOF(*expr)(*)[] : 1, default : 0)
#endif

/**
 *  \def atb_STATIC_ASSERT
 *
 *  \brief Perform a static assertion of expr
 *
 *  \param[in] pred A compile time boolean predicate (false triggers a compile
 *                  time error)
 *  \param[in] msg Message written at compile time
 */
#if defined(__cplusplus)
#define atb_STATIC_ASSERT(pred, msg) static_assert((pred), msg)
#else
#define atb_STATIC_ASSERT(pred, msg)                                           \
  (!!sizeof(struct {                                                           \
    int dummy;                                                                 \
    _Static_assert((pred), msg);                                               \
  }))
#endif
