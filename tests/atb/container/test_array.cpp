#include "atb/container/array.h"
#include "gtest/gtest.h"

namespace {

TEST(AtbArray, Size) {
  {
    int toto[20] = {};
    EXPECT_EQ(20, atb_Array_Size(toto));
  }

  {
    char toto[1] = {0};
    EXPECT_EQ(1, atb_Array_Size(toto));
  }

  {
    float toto[200] = {0};
    EXPECT_EQ(200, atb_Array_Size(toto));
  }
}

TEST(AtbArray, IsInRangeOf) {
  int toto[20] = {0};
  EXPECT_TRUE(atb_Array_IsInRangeOf(toto, 0));
  EXPECT_TRUE(atb_Array_IsInRangeOf(toto, 5));
  EXPECT_TRUE(atb_Array_IsInRangeOf(toto, 19));
  EXPECT_FALSE(atb_Array_IsInRangeOf(toto, 20));
  EXPECT_FALSE(atb_Array_IsInRangeOf(toto, -1));
  EXPECT_FALSE(atb_Array_IsInRangeOf(toto, 0xff));
}

TEST(AtbArray, ForEach) {
  int toto[] = {0, 1, 2, 3};

  int *v = nullptr;
  int *expected_ptr = toto;
  std::size_t i = 0;

  atb_Array_ForEach(v, toto) {
    EXPECT_EQ(v, expected_ptr++);
    EXPECT_EQ(*v, toto[i++]);
  }

  atb_Array_ForEachR(v, toto) {
    EXPECT_EQ(v, --expected_ptr);
    EXPECT_EQ(*v, toto[--i]);
  }
}

} // namespace
