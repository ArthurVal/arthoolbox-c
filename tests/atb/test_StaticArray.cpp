#include "gtest/gtest.h"

#include "atb/StaticArray.h"

#include <algorithm>

namespace {

TEST(AtbStaticArray, Size) {
  constexpr auto size = 10u;
  int toto[size];
  EXPECT_EQ(atb_StaticArray_GetSize(toto), size);
}

TEST(AtbStaticArray, Begin) {
  constexpr auto size = 10u;
  int toto[size];
  EXPECT_EQ(atb_StaticArray_Begin(toto), toto);
  EXPECT_EQ(atb_StaticArray_RBegin(toto), toto + size - 1);
}

TEST(AtbStaticArray, End) {
  constexpr auto size = 10u;
  int toto[size];
  EXPECT_EQ(atb_StaticArray_End(toto), toto + size);
  EXPECT_EQ(atb_StaticArray_REnd(toto), toto - 1);
}

TEST(AtbStaticArray, ForEach) {
  constexpr auto size = 10u;
  int toto[size];
  std::generate(std::begin(toto), std::end(toto), []() {
    static int val = 0;
    return val++;
  });

  int *elem;

  int *expected_elem = toto;
  int expected_val = 0;
  atb_StaticArray_ForEach(elem, toto) {
    EXPECT_EQ(*elem, expected_val++);
    EXPECT_EQ(elem, expected_elem++);
  }

  atb_StaticArray_RForEach(elem, toto) {
    EXPECT_EQ(*elem, --expected_val);
    EXPECT_EQ(elem, --expected_elem);
  }
}

} // namespace
