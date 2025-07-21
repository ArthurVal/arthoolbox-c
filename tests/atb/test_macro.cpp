#include "atb/macro.h"
#include "gtest/gtest.h"

namespace {

TEST(TestAtbMacro, ARGN) {
  EXPECT_EQ(0, atb_VA_ARGN());
  EXPECT_EQ(1, atb_VA_ARGN(1));
  EXPECT_EQ(2, atb_VA_ARGN(1, 1));
  EXPECT_EQ(2, atb_VA_ARGN(1, 'v'));
  EXPECT_EQ(5, atb_VA_ARGN(1, 'v', 1, 'v', 1));
}

} // namespace
