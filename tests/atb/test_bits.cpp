#include "atb/bits.h"
#include "gtest/gtest.h"

namespace atb {
namespace {

TEST(AtbBitsTest, Mask) {
  EXPECT_EQ(0b0, ATB_BITMASK());
  EXPECT_EQ(0b1, ATB_BITMASK(0));
  EXPECT_EQ(0b10, ATB_BITMASK(1));
  EXPECT_EQ(0b100, ATB_BITMASK(2));
  EXPECT_EQ(0b1000, ATB_BITMASK(3));

  EXPECT_EQ(0b1, ATB_BITMASK(0, 0, 0));
  EXPECT_EQ(0b111, ATB_BITMASK(0, 1, 2));
  EXPECT_EQ(0b10110, ATB_BITMASK(1, 4, 2));
}

TEST(AtbBitsTest, HasAllOf) {
  EXPECT_TRUE(ATB_BITS_HasAllOf(0b1, 0));
  EXPECT_FALSE(ATB_BITS_HasAllOf(0b1, 1));
  EXPECT_TRUE(ATB_BITS_HasAllOf(0b11, 0, 1));
  EXPECT_TRUE(ATB_BITS_HasAllOf(0b11, 1, 0));
  EXPECT_TRUE(ATB_BITS_HasAllOf(0b1011, 1, 0));
  EXPECT_FALSE(ATB_BITS_HasAllOf(0b10, 1, 0));
  EXPECT_FALSE(ATB_BITS_HasAllOf(0b01, 1, 0));
}

TEST(AtbBitsTest, HasAnyOf) {
  EXPECT_TRUE(ATB_BITS_HasAnyOf(0b1, 0));
  EXPECT_FALSE(ATB_BITS_HasAnyOf(0b1, 1));
  EXPECT_TRUE(ATB_BITS_HasAnyOf(0b11, 0, 1));
  EXPECT_TRUE(ATB_BITS_HasAnyOf(0b11, 1, 0));
  EXPECT_TRUE(ATB_BITS_HasAnyOf(0b1011, 1, 0));
  EXPECT_TRUE(ATB_BITS_HasAnyOf(0b10, 1, 0));
  EXPECT_TRUE(ATB_BITS_HasAnyOf(0b01, 1, 0));
}

TEST(AtbBitsTest, Set) {
  std::size_t value = 0;

  ATB_BITS_Set(value, 0, 1);
  EXPECT_EQ(value, 0b11);

  ATB_BITS_Set(value, 4);
  EXPECT_EQ(value, 0b1'0011);

  ATB_BITS_Set(value, 4, 5, 1);
  EXPECT_EQ(value, 0b11'0011);
}

TEST(AtbBitsTest, Unset) {
  std::size_t value = 0xFF;

  ATB_BITS_Unset(value, 0, 1);
  EXPECT_EQ(value, 0b1111'1100);

  ATB_BITS_Unset(value, 0);
  EXPECT_EQ(value, 0b1111'1100);

  ATB_BITS_Unset(value, 4, 5, 1);
  EXPECT_EQ(value, 0b1100'1100);
}

} // namespace
} // namespace atb
