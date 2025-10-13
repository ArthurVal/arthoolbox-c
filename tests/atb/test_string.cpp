#include "atb/string.h"
#include "gtest/gtest.h"
#include "test_error.hpp"
#include "test_span_string.hpp"

using namespace std::string_view_literals;

namespace atb {
namespace {

TEST(AtbStringDeathTest, FromInt) {
  char str[50];
  const atb_StrSpan dest = atb_AnySpan_From_Array(str);

  EXPECT_DEBUG_DEATH(atb_String_FromInt_u(12345, (ATB_INT_BASE)37, dest,
                                          nullptr, K_ATB_ERROR_IGNORED),
                     "IsValid\\(base\\)");

  EXPECT_DEBUG_DEATH(atb_String_FromInt_u(12345, (ATB_INT_BASE)1, dest, nullptr,
                                          K_ATB_ERROR_IGNORED),
                     "IsValid\\(base\\)");

  EXPECT_DEBUG_DEATH(atb_String_FromInt_u(12345, (ATB_INT_BASE)(-1), dest,
                                          nullptr, K_ATB_ERROR_IGNORED),
                     "IsValid\\(base\\)");
}

TEST(AtbStringTest, FromInt) {
  char str[50];
  const atb_StrSpan dest = atb_AnySpan_From_Array(str);

  atb_Error err;
  atb_StrSpan remaining = K_ATB_ANYSPAN_INVALID;

  // dest = INVALID
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_u(12345, K_ATB_INT_DEC, K_ATB_ANYSPAN_INVALID,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(remaining.size, "12345"sv.size());

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_i(-12345, K_ATB_INT_DEC, K_ATB_ANYSPAN_INVALID,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(remaining.size, "-12345"sv.size());

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_u(0xFF, K_ATB_INT_HEX, K_ATB_ANYSPAN_INVALID,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(remaining.size, "FF"sv.size());

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_i(0b111, K_ATB_INT_BIN, K_ATB_ANYSPAN_INVALID,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(remaining.size, "111"sv.size());

  // UNSIGNED - DEC
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(
      atb_String_FromInt_u(12345, K_ATB_INT_DEC, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "12345"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "12345"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "12345"sv.size());

  // SIGNED - DEC
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_i(20, K_ATB_INT_DEC, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "20"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "20"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "20"sv.size());

  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_i(-20, K_ATB_INT_DEC, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "-20"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "-20"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "-20"sv.size());

  // UNSIGNED - BIN
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_u(0b110010110101, K_ATB_INT_BIN, dest,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "110010110101"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "110010110101"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "110010110101"sv.size());

  // SIGNED - BIN
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(
      atb_String_FromInt_u(0b10010101, K_ATB_INT_BIN, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "10010101"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "10010101"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "10010101"sv.size());

  // UNSIGNED - OCT
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_u(0134, K_ATB_INT_OCT, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "134"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "134"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "134"sv.size());

  // SIGNED - OCT
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_FromInt_u(020, K_ATB_INT_OCT, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "20"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "20"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "20"sv.size());

  // UNSIGNED - HEX
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(
      atb_String_FromInt_u(0xCAFEDECA, K_ATB_INT_HEX, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "CAFEDECA"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "CAFEDECA"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "CAFEDECA"sv.size());

  // SIGNED - HEX
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(
      atb_String_FromInt_u(0xDEADBEEF, K_ATB_INT_HEX, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "DEADBEEF"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "DEADBEEF"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "DEADBEEF"sv.size());

  // UNSIGNED - WEIRD BASE
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(
      atb_String_FromInt_u(36 - 1, (ATB_INT_BASE)36, dest, &remaining, &err))
      << err;
  EXPECT_EQ(std::string_view(str), "Z"sv);
  EXPECT_EQ(remaining.data, std::next(dest.data, "Z"sv.size()));
  EXPECT_EQ(remaining.size, dest.size - "Z"sv.size());

  // dest too small
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_FALSE(atb_String_FromInt_u(
      0xCAFE, K_ATB_INT_HEX, atb_StrSpan_First(dest, 2), &remaining, &err));
  EXPECT_THAT(str, testing::Each('\0'));
  EXPECT_THAT(remaining, FieldsMatch((atb_StrSpan)K_ATB_ANYSPAN_INVALID));
  EXPECT_THAT(err, FieldsMatch((atb_Error){
                       K_ATB_ERROR_GENERIC,
                       K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE,
                   }));

  err.category = 0;
  err.code = 0;
  std::fill(std::begin(str), std::end(str), '\0');
  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_FALSE(atb_String_FromInt_i(
      -12345, K_ATB_INT_DEC, atb_StrSpan_First(dest, 2), &remaining, &err));
  EXPECT_THAT(str, testing::Each('\0'));
  EXPECT_THAT(remaining, FieldsMatch((atb_StrSpan)K_ATB_ANYSPAN_INVALID));
  EXPECT_THAT(err, FieldsMatch((atb_Error){
                       K_ATB_ERROR_GENERIC,
                       K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE,
                   }));
}

TEST(AtbStringDeathTest, ToIntBase) {
  EXPECT_DEBUG_DEATH(
      atb_String_ToIntBase(atb_StrView_From_StrLiteral("3058u018u"), nullptr,
                           nullptr, K_ATB_ERROR_IGNORED),
      "base != NULL");
}

TEST(AtbStringTest, ToIntBase) {
  atb_Error err;
  atb_StrView remaining = K_ATB_ANYSPAN_INVALID;

  ATB_INT_BASE base;

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0"), &base,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_DEC);
  EXPECT_EQ(ToSv(remaining), "0");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("12378"), &base,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_DEC);
  EXPECT_EQ(ToSv(remaining), "12378");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("-12"), &base,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_DEC);
  EXPECT_EQ(ToSv(remaining), "-12");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0x12378"),
                                   &base, &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_HEX);
  EXPECT_EQ(ToSv(remaining), "12378");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0XAF"), &base,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_HEX);
  EXPECT_EQ(ToSv(remaining), "AF");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0xbF"), &base,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_HEX);
  EXPECT_EQ(ToSv(remaining), "bF");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0777"), &base,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_OCT);
  EXPECT_EQ(ToSv(remaining), "777");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0o2135"), &base,
                                   &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_OCT);
  EXPECT_EQ(ToSv(remaining), "2135");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0b10010"),
                                   &base, &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_BIN);
  EXPECT_EQ(ToSv(remaining), "10010");

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("0B10010"),
                                   &base, &remaining, &err))
      << err;
  EXPECT_EQ(base, K_ATB_INT_BIN);
  EXPECT_EQ(ToSv(remaining), "10010");

  // FAILS:
  EXPECT_FALSE(atb_String_ToIntBase(atb_StrView_From_StrLiteral(""), &base,
                                    &remaining, &err))
      << "Base: " << base << " | Remaining: " << ToSv(remaining);
  EXPECT_THAT(err, FieldsMatch((atb_Error){
                       K_ATB_ERROR_GENERIC,
                       K_ATB_ERROR_GENERIC_INVALID_ARGUMENT,
                   }));

  EXPECT_FALSE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("A0B10010"),
                                    &base, &remaining, &err))
      << "Base: " << base << " | Remaining: " << ToSv(remaining);
  EXPECT_THAT(err, FieldsMatch((atb_Error){
                       K_ATB_ERROR_GENERIC,
                       K_ATB_ERROR_GENERIC_ARGUMENT_OUT_OF_DOMAIN,
                   }));

  EXPECT_FALSE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("-"), &base,
                                    &remaining, &err))
      << "Base: " << base << " | Remaining: " << ToSv(remaining);
  EXPECT_THAT(err, FieldsMatch((atb_Error){
                       K_ATB_ERROR_GENERIC,
                       K_ATB_ERROR_GENERIC_ARGUMENT_OUT_OF_DOMAIN,
                   }));

  EXPECT_FALSE(atb_String_ToIntBase(atb_StrView_From_StrLiteral("-A"), &base,
                                    &remaining, &err))
      << "Base: " << base << " | Remaining: " << ToSv(remaining);
  EXPECT_THAT(err, FieldsMatch((atb_Error){
                       K_ATB_ERROR_GENERIC,
                       K_ATB_ERROR_GENERIC_ARGUMENT_OUT_OF_DOMAIN,
                   }));
}

// TEST(AtbStringTest, ToInt) {
//   atb_StrView view;
//   atb_Error err;
//   atb_StrView remaining = K_ATB_ANYSPAN_INVALID;

//   uintmax_t v = 0;

//   v = 1;
//   view = atb_StrView_From_StrLiteral("0");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 0);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   view = atb_StrView_From_StrLiteral("10");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 10);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   view = atb_StrView_From_StrLiteral("0x10");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 0x10);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   view = atb_StrView_From_StrLiteral("0XFF");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 0xFF);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   view = atb_StrView_From_StrLiteral("070");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 070);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   view = atb_StrView_From_StrLiteral("0o10");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 010);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   view = atb_StrView_From_StrLiteral("0b10");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 0b10);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   view = atb_StrView_From_StrLiteral("0B10");
//   remaining = K_ATB_ANYSPAN_INVALID;
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 0b10);
//   EXPECT_EQ(ToSv(remaining), "");

//   v = 0;
//   remaining = K_ATB_ANYSPAN_INVALID;
//   view = atb_StrView_From_StrLiteral("12345 ABCVBFEIBNFEOIP");
//   EXPECT_TRUE(atb_String_ToInt_u(view, &v, &remaining, &err)) << err;
//   EXPECT_EQ(v, 12345);
//   EXPECT_EQ(ToSv(remaining), " ABCVBFEIBNFEOIP");
// }

} // namespace
} // namespace atb
