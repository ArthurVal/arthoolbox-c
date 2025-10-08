#pragma once

#include <cstdio>
#include <string>
#include <type_traits>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace atb {

namespace details {

template <class, template <class...> class Trait, class... Args>
struct HasTraitImpl : std::false_type {};

template <template <class...> class Trait, class... Args>
struct HasTraitImpl<std::void_t<Trait<Args...>>, Trait, Args...>
    : std::true_type {};

} // namespace details

/// Returns TRUE when the given type Trait<Args...> is a valid type (SFINAE)
///
/// Use it like this:
///
/// // Create a trait...
/// template <class ...T>
/// using FooFunction = decltype(Foo(std::declval<T>()...));
///
/// // ...Then use HasTrait:
/// static_assert(HasTrait_v<FooFunction, int, float, char>);
/// // -> True if `Foo(int, float, char)` exists
template <template <class...> class Trait, class... Args>
struct HasTrait : details::HasTraitImpl<void, Trait, Args...> {};

template <template <class...> class Trait, class... Args>
constexpr bool HasTrait_v = HasTrait<Trait, Args...>::value;

template <class... Args>
auto MakeStringFromFmt(const char *fmt, Args &&...args) -> std::string {
  std::string out;
  out.resize(std::snprintf(nullptr, 0, fmt, std::forward<Args>(args)...));
  std::sprintf(out.data(), fmt, std::forward<Args>(args)...);
  return out;
}

namespace details {

template <class Sink, class T>
using StreamOperatorExists =
    decltype(std::declval<Sink>() << std::declval<T>());

template <class Sink, class T, bool>
struct IsStreamableToImpl : std::false_type {};

template <class Sink, class T>
struct IsStreamableToImpl<Sink, T, true>
    : std::is_same<Sink, decltype(std::declval<Sink>() << std::declval<T>())> {
};

} // namespace details

template <class Sink, class T>
struct IsStreamableTo
    : details::IsStreamableToImpl<
          Sink, T, HasTrait_v<details::StreamOperatorExists, Sink, T>> {};

template <class Sink, class T>
constexpr bool IsStreamableTo_v = IsStreamableTo<Sink, T>::value;

template <class Sink, class T>
constexpr auto TryToStreamTo(Sink &s, T &&value,
                             std::string_view backup = "<?>") -> Sink & {
  if constexpr (IsStreamableTo_v<decltype(s), decltype(value)>) {
    s << std::forward<T>(value);
  } else {
    s << backup;
  }

  return s;
}

template <class Sink, class T>
constexpr auto StreamPtrTo(Sink &s, T const *const ptr) -> Sink & {
  s << (void *)ptr;

  if (ptr != nullptr) {
    s << " -> ";
    TryToStreamTo(s, *ptr);
  }

  return s;
}

template <class Pred>
constexpr auto DoNot(Pred &&pred) {
  return [&](auto &&...args) -> bool {
    return not pred(std::forward<decltype(args)>(args)...);
  };
}

template <class T>
struct NamedValue {
  using value_type = T;

  std::string_view name;
  const T &value;
};

#define NVALUE(v) \
  atb::NamedValue<std::decay_t<decltype(v)>> { #v, v, }

template <class Sink, class T, class... Others>
constexpr auto FormatTo(Sink &&sink, const NamedValue<T> &nv,
                        const NamedValue<Others> &...others) -> Sink && {
  if constexpr (sizeof...(others) == 0) {
    sink << "\n - " << nv.name << ": " << nv.value;
    return std::forward<Sink>(sink);
  } else {
    return std::forward<Sink>(
        FormatTo(FormatTo(std::forward<Sink>(sink), nv), others...));
  }
}

template <class T, class... Others>
auto ToString(const NamedValue<T> &nv,
              const NamedValue<Others> &...others) -> std::string {
  std::stringstream ss;
  return FormatTo(ss, nv, others...).str();
}

} // namespace atb

#define EXPECT_NPRED1(pred, v1) EXPECT_PRED1(atb::DoNot(pred), v1)
#define EXPECT_NPRED2(pred, v1, v2) EXPECT_PRED2(atb::DoNot(pred), v1, v2)
#define EXPECT_NPRED3(pred, v1, v2, v3) \
  EXPECT_PRED3(atb::DoNot(pred), v1, v2, v3)
#define EXPECT_NPRED4(pred, v1, v2, v3, v4) \
  EXPECT_PRED4(atb::DoNot(pred), v1, v2, v3, v4)

#define AddFieldMatchFor_1(type, val, p1) testing::Field(#p1, &type::p1, val.p1)

#define AddFieldMatchFor_2(type, val, p1, p2) \
  AddFieldMatchFor_1(type, val, p1), AddFieldMatchFor_1(type, val, p2)

#define AddFieldMatchFor_3(type, val, p1, p2, p3) \
  AddFieldMatchFor_2(type, val, p1, p2), AddFieldMatchFor_1(type, val, p3)

#define AddFieldMatchFor_4(type, val, p1, p2, p3, p4) \
  AddFieldMatchFor_3(type, val, p1, p2, p3), AddFieldMatchFor_1(type, val, p4)

#define AddFieldMatchFor_5(type, val, p1, p2, p3, p4, p5) \
  AddFieldMatchFor_4(type, val, p1, p2, p3, p4),          \
      AddFieldMatchFor_1(type, val, p5)

#define AddFieldMatchFor_6(type, val, p1, p2, p3, p4, p5, p6) \
  AddFieldMatchFor_5(type, val, p1, p2, p3, p4, p5),          \
      AddFieldMatchFor_1(type, val, p6)

#define DefineFieldsMatchFor(type, count, ...)                                 \
  inline auto FieldsMatch(const type &arg) noexcept {                          \
    return testing::AllOf(AddFieldMatchFor_##count(type, arg, ##__VA_ARGS__)); \
  }                                                                            \
  static_assert(true)
