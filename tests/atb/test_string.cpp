#include "atb/string.h"
#include "gtest/gtest.h"
#include "test_error.hpp"
#include "test_span_string.hpp"

using namespace std::string_view_literals;

namespace atb {
namespace {

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

} // namespace
} // namespace atb
