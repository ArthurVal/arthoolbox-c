#include "atb/bits.h"
#include "gtest/gtest.h"

namespace atb {
namespace {

TEST(AtbBitsTest, Mask) {
  EXPECT_EQ(0b0, atb_BitMask());
  EXPECT_EQ(0b1, atb_BitMask(0));
  EXPECT_EQ(0b10, atb_BitMask(1));
  EXPECT_EQ(0b100, atb_BitMask(2));
  EXPECT_EQ(0b1000, atb_BitMask(3));

  EXPECT_EQ(0b1, atb_BitMask(0, 0, 0));
  EXPECT_EQ(0b111, atb_BitMask(0, 1, 2));
  EXPECT_EQ(0b10110, atb_BitMask(1, 4, 2));
}

TEST(AtbBitsTest, HasAllOf) {
  EXPECT_TRUE(atb_Bits_HasAllOf(0b1, 0));
  EXPECT_FALSE(atb_Bits_HasAllOf(0b1, 1));
  EXPECT_TRUE(atb_Bits_HasAllOf(0b11, 0, 1));
  EXPECT_TRUE(atb_Bits_HasAllOf(0b11, 1, 0));
  EXPECT_TRUE(atb_Bits_HasAllOf(0b1011, 1, 0));
  EXPECT_FALSE(atb_Bits_HasAllOf(0b10, 1, 0));
  EXPECT_FALSE(atb_Bits_HasAllOf(0b01, 1, 0));
}

TEST(AtbBitsTest, HasAnyOf) {
  EXPECT_TRUE(atb_Bits_HasAnyOf(0b1, 0));
  EXPECT_FALSE(atb_Bits_HasAnyOf(0b1, 1));
  EXPECT_TRUE(atb_Bits_HasAnyOf(0b11, 0, 1));
  EXPECT_TRUE(atb_Bits_HasAnyOf(0b11, 1, 0));
  EXPECT_TRUE(atb_Bits_HasAnyOf(0b1011, 1, 0));
  EXPECT_TRUE(atb_Bits_HasAnyOf(0b10, 1, 0));
  EXPECT_TRUE(atb_Bits_HasAnyOf(0b01, 1, 0));
}

TEST(AtbBitsTest, Set) {
  std::size_t value = 0;

  atb_Bits_Set(value, 0, 1);
  EXPECT_EQ(value, 0b11);

  atb_Bits_Set(value, 4);
  EXPECT_EQ(value, 0b1'0011);

  atb_Bits_Set(value, 4, 5, 1);
  EXPECT_EQ(value, 0b11'0011);
}

TEST(AtbBitsTest, Unset) {
  std::size_t value = 0xFF;

  atb_Bits_Unset(value, 0, 1);
  EXPECT_EQ(value, 0b1111'1100);

  atb_Bits_Unset(value, 0);
  EXPECT_EQ(value, 0b1111'1100);

  atb_Bits_Unset(value, 4, 5, 1);
  EXPECT_EQ(value, 0b1100'1100);
}

} // namespace
} // namespace atb
