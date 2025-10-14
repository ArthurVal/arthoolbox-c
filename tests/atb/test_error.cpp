#include "test_error.hpp"

namespace atb {
namespace {

TEST(AtbErrorTest, Ignored) {
  EXPECT_PRED1(atb_Error_IsIgnored, (atb_Error *)K_ATB_ERROR_IGNORED);

  atb_Error err;
  EXPECT_NPRED1(atb_Error_IsIgnored, &err);
}

TEST(AtbErrorTest, Set) {
  // No crashs
  atb_Error_Set(K_ATB_ERROR_IGNORED, 0, 0);

  atb_Error err;
  err.category = 0;
  err.code = 0;

  atb_Error_Set(&err, 123, -1);
  EXPECT_EQ(err.category, 123);
  EXPECT_EQ(err.code, -1);

  atb_RawError_Set(&err, -2);
  EXPECT_EQ(err.category, K_ATB_ERROR_RAW);
  EXPECT_EQ(err.code, -2);

  atb_GenericError_Set(&err, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);
  EXPECT_EQ(err.category, K_ATB_ERROR_GENERIC);
  EXPECT_EQ(err.code, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);
}

} // namespace

} // namespace atb

auto operator<<(std::ostream &os, const atb_Error &err) -> std::ostream & {
  os << "atb_Error"
     << atb::MakeStringFromFmt(K_ATB_FMT_ERROR, ATB_FMT_VA_ARG_ERROR(err));
  return os;
}

auto operator<<(std::ostream &os,
                const atb_Error *const err) -> std::ostream & {
  return atb::StreamPtrTo(os, err);
}
