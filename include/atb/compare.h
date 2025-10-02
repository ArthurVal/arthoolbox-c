#ifndef _ATB_CMP_H_
#define _ATB_CMP_H_

#include <assert.h> /* static_assert() */
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

/// Generic Three-way compare result enumeration
typedef enum {
  K_ATB_CMP_LESS = -1,
  K_ATB_CMP_EQUAL = 0,
  K_ATB_CMP_GREATER = 1,
} atb_Cmp_t;

/**
 *  \brief Define 'Not Equal' function `FooNe(lhs, rhs) -> bool` using the
 *         already defined `FooEq(...) -> bool`, corresponding to 'Is Equal'.
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX The prefix used when defining the function (e.g.
 *                    prefix=Foo will create FooNe() and assume that FooEq()
 *                    exists)
 *  \param[in] T The argument's type of the functions created
 *
 *  \pre The function `<PREFIX>Eq(T, T) -> bool` is declared
 */
#define ATB_CMP_DEFINE_FROM_EQ(SPECIFIER, PREFIX, T)                        \
  SPECIFIER bool PREFIX##Ne(T lhs, T rhs) { return !PREFIX##Eq(rhs, lhs); } \
  static_assert(true, "SEMI-COLON NEEDED HERE")

/**
 *  \brief Define 'Is Equal' function `FooEq(lhs, rhs) -> bool` using the
 *         already defined `FooNe(...) -> bool`, corresponding to 'Not Equal'.
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX The PREFIX used when defining the function (e.g.
 *                    PREFIX=Foo will create FooEq() and assume that FooNe()
 *                    exists)
 *  \param[in] T The argument's type of the functions created
 *
 *  \pre The function `<PREFIX>Ne(T, T) -> bool` is declared
 */
#define ATB_CMP_DEFINE_FROM_NE(SPECIFIER, PREFIX, T)                        \
  SPECIFIER bool PREFIX##Eq(T lhs, T rhs) { return !PREFIX##Ne(rhs, lhs); } \
  static_assert(true, "SEMI-COLON NEEDED HERE")

/**
 *  \brief Define Lt, Ge, Le comparison functions  using the already
 *         defined `Gt(...) -> bool`, corresponding to 'Greater Than'.
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX The PREFIX used when defining the function (e.g.
 *                    PREFIX=Foo will create Foo<..>() and assume that FooGt()
 *                    exists)
 *  \param[in] T The argument's type of the functions created
 *
 *  \pre The function `<PREFIX>Gt(T, T) -> bool` is declared
 */
#define ATB_CMP_DEFINE_FROM_GT(SPECIFIER, PREFIX, T)                        \
  SPECIFIER bool PREFIX##Lt(T lhs, T rhs) { return PREFIX##Gt(rhs, lhs); }  \
  SPECIFIER bool PREFIX##Le(T lhs, T rhs) { return !PREFIX##Gt(lhs, rhs); } \
  SPECIFIER bool PREFIX##Ge(T lhs, T rhs) { return !PREFIX##Lt(lhs, rhs); } \
  static_assert(true, "SEMI-COLON NEEDED HERE")

/**
 *  \brief Define Gt, Ge, Le comparison functions using the already
 *         defined `Lt(...) -> bool`, corresponding to 'Less Than'.
 *
 *  \param[in] PREFIX The PREFIX used when defining the function (e.g.
 *                    PREFIX=Foo will create Foo<..>() and assume that FooLt()
 *                    exists)
 *  \param[in] T The argument's type of the functions created
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *
 *  \pre The function `<PREFIX>Lt(T, T) -> bool` is declared
 */
#define ATB_CMP_DEFINE_FROM_LT(SPECIFIER, PREFIX, T, ATTRIBUTES)            \
  SPECIFIER bool PREFIX##Gt(T lhs, T rhs) { return PREFIX##Lt(rhs, lhs); }  \
  SPECIFIER bool PREFIX##Le(T lhs, T rhs) { return !PREFIX##Gt(lhs, rhs); } \
  SPECIFIER bool PREFIX##Ge(T lhs, T rhs) { return !PREFIX##Lt(lhs, rhs); } \
  static_assert(true, "SEMI-COLON NEEDED HERE")

/**
 *  \brief Define Lt, Gt, Le comparison functions  using the already
 *         defined `Ge(...) -> bool`, corresponding to 'Greater or Equals'.
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX The PREFIX used when defining the function (e.g.
 *                    PREFIX=Foo will create Foo<..>() and assume that FooGe()
 *                    exists)
 *  \param[in] T The argument's type of the functions created
 *
 *  \pre The function `<PREFIX>Ge(T, T) -> bool` is declared
 */
#define ATB_CMP_DEFINE_FROM_GE(SPECIFIER, PREFIX, T)                        \
  SPECIFIER bool PREFIX##Lt(T lhs, T rhs) { return !PREFIX##Ge(lhs, rhs); } \
  SPECIFIER bool PREFIX##Gt(T lhs, T rhs) { return PREFIX##Lt(rhs, lhs); }  \
  SPECIFIER bool PREFIX##Le(T lhs, T rhs) { return !PREFIX##Gt(lhs, rhs); } \
  static_assert(true, "SEMI-COLON NEEDED HERE")

/**
 *  \brief Define Lt, Gt, Ge comparison functions using the already
 *         defined `Le(...) -> bool`, corresponding to 'Lesser or Equals'.
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX The PREFIX used when defining the function (e.g.
 *                    PREFIX=Foo will create Foo<..>() and assume that FooLe()
 *                    exists)
 *  \param[in] T The argument's type of the functions created
 *
 *  \pre The function `<PREFIX>Le(T, T) -> bool` is declared
 */
#define ATB_CMP_DEFINE_FROM_LE(SPECIFIER, PREFIX, T)                        \
  SPECIFIER bool PREFIX##Gt(T lhs, T rhs) { return !PREFIX##Le(lhs, rhs); } \
  SPECIFIER bool PREFIX##Lt(T lhs, T rhs) { return PREFIX##Gt(rhs, lhs); }  \
  SPECIFIER bool PREFIX##Ge(T lhs, T rhs) { return !PREFIX##Lt(lhs, rhs); } \
  static_assert(true, "SEMI-COLON NEEDED HERE")

/**
 *  \brief Declare ALL comparison functions (Eq, Ne, Gt, Lt, ...)
 *
 *  The declaration name pattern is the following:
 *
 *  SPECIFIER bool PREFIX<FN>(T lhs, T rhs);
 *
 *  With:
 *  - SPECIFIER: Any C function specifiers/attributs (e.g. inline, static, ...);
 *  - PREFIX: A common function prefix used;
 *  - T: The arguments types of the value we wish to define the compare
 *             functions;
 *  - <FN>: The postfix function names:
 *    - Eq: Equals
 *    - Ne: Not Equals
 *    - Gt: Greater Than
 *    - Lt: Less Than
 *    - Ge: Greater or Equals Than
 *    - Le: Less or Equals Than
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX A token prefixed to all functions name
 *  \param[in] T The argument type
 */
#define ATB_CMP_DECLARE_ALL(SPECIFIER, PREFIX, T) \
  SPECIFIER bool PREFIX##Eq(T lhs, T rhs);        \
  SPECIFIER bool PREFIX##Ne(T lhs, T rhs);        \
  SPECIFIER bool PREFIX##Gt(T lhs, T rhs);        \
  SPECIFIER bool PREFIX##Lt(T lhs, T rhs);        \
  SPECIFIER bool PREFIX##Ge(T lhs, T rhs);        \
  SPECIFIER bool PREFIX##Le(T lhs, T rhs)

/**
 *  \brief Define ALL comparison functions (Eq, Ne, Gt, Lt, Gt, Ge) using
 *         the already defined `Compare(a, b) -> int`, doing a 'three-way
 *         comparison'.
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX The PREFIX used when defining the function (e.g.
 *                    PREFIX=Foo will create Foo<..>() and use FooCompare())
 *  \param[in] T The argument's type of the functions created
 *
 *  \pre The UNSAFE function `<PREFIX>Compare(T, T) -> int` is
 *       declared and follows the 'three-way comparison' idiom, i.e.:
 *       - Compare(a, b) < 0 when a < b;
 *       - Compare(a, b) == 0 when a == b;
 *       - Compare(a, b) > 0 when a > b;
 */
#define ATB_CMP_DEFINE_ALL_FROM_UNSAFE_COMPARE(SPECIFIER, PREFIX, T) \
  SPECIFIER bool PREFIX##Eq(T lhs, T rhs) {                          \
    return PREFIX##Compare(lhs, rhs) == 0;                           \
  }                                                                  \
                                                                     \
  SPECIFIER bool PREFIX##Ne(T lhs, T rhs) {                          \
    return PREFIX##Compare(lhs, rhs) != 0;                           \
  }                                                                  \
                                                                     \
  SPECIFIER bool PREFIX##Gt(T lhs, T rhs) {                          \
    return PREFIX##Compare(lhs, rhs) > 0;                            \
  }                                                                  \
                                                                     \
  SPECIFIER bool PREFIX##Lt(T lhs, T rhs) {                          \
    return PREFIX##Compare(lhs, rhs) < 0;                            \
  }                                                                  \
                                                                     \
  SPECIFIER bool PREFIX##Ge(T lhs, T rhs) {                          \
    return PREFIX##Compare(lhs, rhs) >= 0;                           \
  }                                                                  \
                                                                     \
  SPECIFIER bool PREFIX##Le(T lhs, T rhs) {                          \
    return PREFIX##Compare(lhs, rhs) <= 0;                           \
  }                                                                  \
                                                                     \
  static_assert(true, "SEMI-COLON NEEDED HERE")

/**
 *  \brief Define ALL comparison functions (Eq, Ne, Gt, Lt, Gt, Ge) using
 *         the already defined `Compare(a, b, &res)`, doing a 'three-way
 *         comparison'.
 *
 *  \param[in] SPECIFIER Any function specifier (inline, static, ...) or
 *                       attributes. May be left empty.
 *  \param[in] PREFIX The PREFIX used when defining the function (e.g.
 *                    PREFIX=Foo will create Foo<..>() and use FooCompare())
 *  \param[in] T The argument's type of the functions created
 *
 *  \pre The SAFE function `<PREFIX>Compare(T, T, int*) -> bool` is declared.
 *       It returns false when the comparison failed or, on success, set the
 *       value of 'res' according to the 'three-way comparison' idiom, i.e.:
 *       - res < 0 when a < b;
 *       - res == 0 when a == b;
 *       - res > 0 when a > b;
 */
#define ATB_CMP_DEFINE_ALL_FROM_SAFE_COMPARE(SPECIFIER, PREFIX, T) \
  SPECIFIER bool PREFIX##Eq(T lhs, T rhs) {                        \
    int res;                                                       \
    return PREFIX##Compare(lhs, rhs, &res) && (res == 0);          \
  }                                                                \
                                                                   \
  SPECIFIER bool PREFIX##Ne(T lhs, T rhs) {                        \
    int res;                                                       \
    return PREFIX##Compare(lhs, rhs, &res) && (res != 0);          \
  }                                                                \
                                                                   \
  SPECIFIER bool PREFIX##Gt(T lhs, T rhs) {                        \
    int res;                                                       \
    return PREFIX##Compare(lhs, rhs, &res) && (res > 0);           \
  }                                                                \
                                                                   \
  SPECIFIER bool PREFIX##Lt(T lhs, T rhs) {                        \
    int res;                                                       \
    return PREFIX##Compare(lhs, rhs, &res) && (res < 0);           \
  }                                                                \
                                                                   \
  SPECIFIER bool PREFIX##Ge(T lhs, T rhs) {                        \
    int res;                                                       \
    return PREFIX##Compare(lhs, rhs, &res) && (res >= 0);          \
  }                                                                \
                                                                   \
  SPECIFIER bool PREFIX##Le(T lhs, T rhs) {                        \
    int res;                                                       \
    return PREFIX##Compare(lhs, rhs, &res) && (res <= 0);          \
  }                                                                \
                                                                   \
  static_assert(true, "SEMI-COLON NEEDED HERE")

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* _ATB_CMP_H_ */
