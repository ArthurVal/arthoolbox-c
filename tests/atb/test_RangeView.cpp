#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RangeViewUtils.hpp"
#include "atb/RangeView.h"

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

template <class T> struct RangeView : public ::testing::Test {
  using PtrType = decltype(std::declval<T>().data);
  using ValueType = std::decay_t<decltype(*std::declval<PtrType>())>;

  static inline ValueType TestArray[10u] = {0};
};

TYPED_TEST_SUITE_P(RangeView);

TYPED_TEST_P(RangeView, INITIALIZER) {
  TypeParam range = atb_AnyRangeView_INITIALIZER();
  EXPECT_EQ(nullptr, range.data);
  EXPECT_EQ(0u, range.size);
}

TYPED_TEST_P(RangeView, MapStaticArray) {
  TypeParam range = atb_AnyRangeView_INITIALIZER();

  atb_AnyRangeView_MapStaticArray(range, this->TestArray);

  EXPECT_EQ(std::begin(this->TestArray), range.data);
  EXPECT_EQ(std::size(this->TestArray), range.size);
  EXPECT_PRED3(IsEqual, range, std::cbegin(this->TestArray),
               std::cend(this->TestArray));

  EXPECT_EQ(std::begin(this->TestArray), atb_AnyRangeView_Begin(range));
  EXPECT_EQ(std::end(this->TestArray), atb_AnyRangeView_End(range));

  EXPECT_EQ(std::end(this->TestArray) - 1, atb_AnyRangeView_RBegin(range));
  EXPECT_EQ(std::begin(this->TestArray) - 1, atb_AnyRangeView_REnd(range));
}

TYPED_TEST_P(RangeView, Shrink) {
  TypeParam range = atb_AnyRangeView_INITIALIZER();

  range.data = this->TestArray;
  range.size = 2;

  // auto shrinked_range = atb_AnyRangeView_Shrink(range, 1, true);
  // WIP - TODO
}

REGISTER_TYPED_TEST_SUITE_P(RangeView, INITIALIZER, MapStaticArray, Shrink);

using AllTestedRangeViewTypes =
    meta::SwapContainer_t<meta::AllRangeView_t, ::testing::Types>;

INSTANTIATE_TYPED_TEST_SUITE_P(All, RangeView, AllTestedRangeViewTypes);

} // namespace
