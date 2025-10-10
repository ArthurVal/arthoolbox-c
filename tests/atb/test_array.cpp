#include "atb/array.h"
#include "gtest/gtest.h"

namespace atb {
namespace {

TEST(AtbArrayTest, Size) {
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

TEST(AtbArrayTest, IsInRangeOf) {
  int toto[20] = {0};
  EXPECT_FALSE(atb_Array_IsOutOfRange(toto, 0));
  EXPECT_FALSE(atb_Array_IsOutOfRange(toto, 5));
  EXPECT_FALSE(atb_Array_IsOutOfRange(toto, 19));
  EXPECT_TRUE(atb_Array_IsOutOfRange(toto, 20));
  EXPECT_TRUE(atb_Array_IsOutOfRange(toto, -1));
  EXPECT_TRUE(atb_Array_IsOutOfRange(toto, 0xff));
}

TEST(AtbArrayTest, ForEach) {
  int toto[] = {0, 1, 2, 3};

  int *v = nullptr;
  int *expected_ptr = toto;
  std::size_t i = 0;

  atb_Array_ForEachUnsafe(v, toto) {
    EXPECT_EQ(v, expected_ptr++);
    EXPECT_EQ(*v, toto[i++]);
  }

  atb_Array_RForEachUnsafe(v, toto) {
    EXPECT_EQ(v, --expected_ptr);
    EXPECT_EQ(*v, toto[--i]);
  }

  atb_Array_ForEach(v, toto, *v == 2) {}
  EXPECT_EQ(*v, toto[2]);

  atb_Array_RForEach(v, toto, *v == 1) {}
  EXPECT_EQ(*v, toto[1]);
}

} // namespace

} // namespace atb
