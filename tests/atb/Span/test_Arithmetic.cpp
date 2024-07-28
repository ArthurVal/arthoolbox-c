#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ArithmeticUtils.hpp"
#include "atb/Span/Arithmetic.h"

#include <array>
#include <iterator>
#include <string_view>
#include <utility>

namespace {

constexpr auto IsEqual = [](auto &&range, auto &&first, auto &&last) {
  return ::IsEqual(std::forward<decltype(range)>(range),
                   std::forward<decltype(first)>(first),
                   std::forward<decltype(last)>(last));
};

template <class T, class... Others>
constexpr auto MakeArray(Others &&...others) {
  return std::array<T, sizeof...(others)>{
      static_cast<T>(std::forward<Others>(others))...};
}

template <class T> struct AnySpan : public ::testing::Test {
  using PtrType = decltype(std::declval<T>().data);
  using ValueType = std::decay_t<decltype(*std::declval<PtrType>())>;

  static inline ValueType TestArray[10u] = {0};
};

TYPED_TEST_SUITE_P(AnySpan);

TYPED_TEST_P(AnySpan, INITIALIZER) {
  TypeParam range = atb_AnySpan_INITIALIZER();
  EXPECT_EQ(nullptr, range.data);
  EXPECT_EQ(0u, range.size);
}

TYPED_TEST_P(AnySpan, MapStaticArray) {
  TypeParam range = atb_AnySpan_INITIALIZER();

  atb_AnySpan_MapStaticArray(range, this->TestArray);

  EXPECT_EQ(std::begin(this->TestArray), range.data);
  EXPECT_EQ(std::size(this->TestArray), range.size);
  EXPECT_PRED3(IsEqual, range, std::cbegin(this->TestArray),
               std::cend(this->TestArray));

  EXPECT_EQ(std::begin(this->TestArray), atb_AnySpan_Begin(range));
  EXPECT_EQ(std::end(this->TestArray), atb_AnySpan_End(range));

  EXPECT_EQ(std::end(this->TestArray) - 1, atb_AnySpan_RBegin(range));
  EXPECT_EQ(std::begin(this->TestArray) - 1, atb_AnySpan_REnd(range));
}

// TYPED_TEST_P(AnySpan, Shrink) {
//   TypeParam range = atb_AnySpan_INITIALIZER();

//   range.data = this->TestArray;
//   range.size = 2;

//   // auto shrinked_range = atb_AnySpan_Shrink(range, 1, true);
//   // WIP - TODO
// }

REGISTER_TYPED_TEST_SUITE_P(AnySpan, INITIALIZER, MapStaticArray);

using AllTestedSpanTypes =
    meta::SwapContainer_t<meta::AllArithmeticSpan_t, ::testing::Types>;

INSTANTIATE_TYPED_TEST_SUITE_P(All, AnySpan, AllTestedSpanTypes);

} // namespace
