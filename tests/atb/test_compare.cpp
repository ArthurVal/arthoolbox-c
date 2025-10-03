#include "test_compare.hpp"

namespace atb {
namespace {

TEST(AtbCompareTest, AtbCmpType) {
  EXPECT_EQ(K_ATB_CMP_EQUAL, 0);
  EXPECT_GT(K_ATB_CMP_GREATER, 0);
  EXPECT_LT(K_ATB_CMP_LESS, 0);
}

} // namespace

} // namespace atb

auto operator<<(std::ostream &os, atb_Cmp_t cmp_res) -> std::ostream & {
  os << static_cast<int>(cmp_res);
  os << "(= " << std::quoted(atb::ToString(cmp_res).value_or("<UNKNOWN>"))
     << ')';
  return os;
}
