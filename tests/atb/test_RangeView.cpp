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
}

REGISTER_TYPED_TEST_SUITE_P(RangeView, INITIALIZER, MapStaticArray);

using AllTestedRangeViewTypes =
    meta::SwapContainer_t<meta::AllRangeView_t, ::testing::Types>;

INSTANTIATE_TYPED_TEST_SUITE_P(All, RangeView, AllTestedRangeViewTypes);

// TEST(AtbRangeView, Shift) {}

// TEST(AtbRangeView, Shrink) {
// auto range = MakeArray<std::uint8_t>(0x01, 0x02, 0x03, 0x04);

// auto view = atb_RangeView{range.data(), 2};
// EXPECT_EQ(atb_RangeView_Advance(view, 2),
//           (atb_RangeView{view.data + 2, view.size - 2}));

// auto c_view = atb_ConstRangeView{range.data(), 2};
// EXPECT_EQ(atb_ConstRangeView_Advance(c_view, 1),
//           (atb_ConstRangeView{view.data + 1, view.size - 1}));
// }

// TEST(AtbRangeView, IsEqualTo) {
// const auto str_1 = std::string{"Coucou"};
// const auto str_2 = str_1;

// EXPECT_PRED2(atb_ConstRangeView_IsEqualTo,
//              (atb_ConstRangeView{str_1.data(), str_1.size()}),
//              (atb_ConstRangeView{str_2.data(), str_2.size()}));

// EXPECT_PRED2(atb_ConstRangeView_IsEqualTo, &str_pool[0], &str_pool[1]);
// }

// test(AtbRangeView, CopyInto) {
//   constexpr auto other_str = std::string_view{"Chocolatine"};
//   std::array<char, other_str.size() * 2> buffer = {0};
//   auto buffer_view = atb_RangeView{};

//   // Buffer empty
//   std::fill(std::begin(buffer), std::end(buffer), '\0');
//   EXPECT_EQ(atb_RangeView_CopyInto(atb_RangeView{buffer.data(), 0},
//                                     atb_ConstRangeView{
//                                         other_str.data(),
//                                         other_str.size(),
//                                     }),
//             (atb_RangeView{buffer.data(), 0}));
//   EXPECT_EQ(buffer.data(), std::string_view{""});

//   // Empty view
//   std::fill(std::begin(buffer), std::end(buffer), '\0');
//   buffer_view = atb_RangeView{buffer.data(), buffer.size()};
//   EXPECT_EQ(atb_RangeView_CopyInto(buffer_view,
//                                     atb_ConstRangeView{other_str.data(), 0}),
//             buffer_view);
//   EXPECT_EQ(buffer.data(), std::string_view{""});

//   // Buffer big enougth
//   std::fill(std::begin(buffer), std::end(buffer), '\0');
//   EXPECT_EQ(
//       atb_RangeView_CopyInto(
//           buffer_view, atb_ConstRangeView{other_str.data(),
//           other_str.size()}),
//       (atb_RangeView{
//           buffer.data() + other_str.size(),
//           buffer_view.size - other_str.size(),
//       }));
//   EXPECT_EQ(buffer.data(), other_str);

//   // Buffer too small
//   std::fill(std::begin(buffer), std::end(buffer), '\0');
//   buffer_view = atb_RangeView{buffer.data(), 3};
//   EXPECT_EQ(
//       atb_RangeView_CopyInto(
//           buffer_view, atb_ConstRangeView{other_str.data(),
//           other_str.size()}),
//       (atb_RangeView{
//           buffer.data() + buffer_view.size,
//           0,
//       }));

//   EXPECT_EQ(std::string_view(buffer.data(), buffer_view.size),
//             std::string_view(other_str.data(), buffer_view.size));
// }

} // namespace
