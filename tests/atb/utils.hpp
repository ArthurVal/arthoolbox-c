#pragma once

#include <cstdio>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace atb {

template <class... Args>
auto MakeStringFromFmt(const char *fmt, Args &&...args) -> std::string {
  std::string out;
  out.resize(std::snprintf(nullptr, 0, fmt, std::forward<Args>(args)...));
  std::sprintf(out.data(), fmt, std::forward<Args>(args)...);
  return out;
}

template <class T>
auto PrintPtrTo(std::ostream &os, T const *const ptr) -> std::ostream & {
  os << (void *)ptr;

  if (ptr != nullptr) {
    os << " -> " << *ptr;
  }

  return os;
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
