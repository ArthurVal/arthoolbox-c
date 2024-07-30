#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <iterator>
#include <optional>
#include <string>
#include <type_traits>

#define SCOPE_MSG_ENTRY(val) "\n - " #val ": " << (val)
#define SCOPE_LOOP_MSG_1(val) (testing::Message() << SCOPE_MSG_ENTRY(val))
#define SCOPE_LOOP_MSG_2(val_1, val_2)                                         \
  (testing::Message() << SCOPE_MSG_ENTRY(val_1) << SCOPE_MSG_ENTRY(val_2))
#define SCOPE_LOOP_MSG_3(val_1, val_2, val_3)                                  \
  (testing::Message() << SCOPE_MSG_ENTRY(val_1) << SCOPE_MSG_ENTRY(val_2)      \
                      << SCOPE_MSG_ENTRY(val_3))
#define SCOPE_LOOP_MSG_4(val_1, val_2, val_3, val_4)                           \
  (testing::Message() << SCOPE_MSG_ENTRY(val_1) << SCOPE_MSG_ENTRY(val_2)      \
                      << SCOPE_MSG_ENTRY(val_3) << SCOPE_MSG_ENTRY(val_4))

namespace helper {

template <typename... Args>
auto MakeStringFromFmt(const char *fmt, Args &&... args) -> std::string {
  std::string out;
  out.resize(std::snprintf(nullptr, 0, fmt, std::forward<Args>(args)...));
  std::sprintf(out.data(), fmt, std::forward<Args>(args)...);
  return out;
}

template <typename Pred> constexpr auto DoNot(Pred &&pred) {
  return [&](auto &&... args) -> bool {
    return not pred(std::forward<decltype(args)>(args)...);
  };
}

struct FormatOptions {
  std::size_t base = 10;
  std::optional<char> fill = std::nullopt;
};

template <typename Integer>
constexpr auto IntToStr(char *d_first, char *d_last, Integer val,
                        FormatOptions options = FormatOptions{})
    -> std::optional<char *> {

  static_assert(std::is_integral_v<Integer>);

  constexpr char digit_available[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if ((options.base <= 1) or (std::size(digit_available) <= options.base)) {
    return std::nullopt;
  }

  if (d_first == d_last) {
    return d_first;
  }

  if constexpr (std::is_signed_v<Integer>) {
    if (val < 0) {
      *d_first++ = '-';
      val = (~val + 1);
    }
  }

  auto d_byte = d_last;

  while (d_byte != d_first) {
    auto idx = val % options.base;
    val = val / options.base;

    *(--d_byte) = digit_available[idx];

    if (val == 0)
      break;
  }

  if (options.fill.has_value()) {
    std::fill(d_first, d_byte, *(options.fill));
    return d_last;
  } else {
    return std::move(d_byte, d_last, d_first);
  }
}

#define AddFieldMatchFor_1(type, val, p1) testing::Field(#p1, &type::p1, val.p1)

#define AddFieldMatchFor_2(type, val, p1, p2)                                  \
  AddFieldMatchFor_1(type, val, p1), AddFieldMatchFor_1(type, val, p2)

#define AddFieldMatchFor_3(type, val, p1, p2, p3)                              \
  AddFieldMatchFor_2(type, val, p1, p2), AddFieldMatchFor_1(type, val, p3)

#define AddFieldMatchFor_4(type, val, p1, p2, p3, p4)                          \
  AddFieldMatchFor_3(type, val, p1, p2, p3), AddFieldMatchFor_1(type, val, p4)

#define AddFieldMatchFor_5(type, val, p1, p2, p3, p4, p5)                      \
  AddFieldMatchFor_4(type, val, p1, p2, p3, p4),                               \
      AddFieldMatchFor_1(type, val, p5)

#define AddFieldMatchFor_6(type, val, p1, p2, p3, p4, p5, p6)                  \
  AddFieldMatchFor_5(type, val, p1, p2, p3, p4, p5),                           \
      AddFieldMatchFor_1(type, val, p6)

#define DefineFieldsMatchFor(type, count, ...)                                 \
  inline auto FieldsMatch(const type &arg) noexcept {                          \
    return testing::AllOf(AddFieldMatchFor_##count(type, arg, ##__VA_ARGS__)); \
  }                                                                            \
  static_assert(true)

} // namespace helper
