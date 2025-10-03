#include <string>
#include <string_view>
using namespace std::string_view_literals;

#include "atb/string/format.h"
#include "test_string_span.hpp"

namespace atb {
namespace {

TEST(AtbStringSpanTest, FromArray) {
  char str[] = {'C', 'o', 'u', 'c', 'o', 'u'};
  atb_StrSpan span = atb_StrSpan_From_Array(str);
  EXPECT_EQ(ToSV(span), "Coucou"sv);
}

TEST(AtbStringSpanTest, From) {
  std::string str = "Coucou";
  EXPECT_EQ(ToSV(atb_StrSpan_From(str.data(), str.size())), str);
}

TEST(AtbStringSpanTest, FromNullTerminated) {
  char null_terminated_str[] = "Coucou";
  EXPECT_EQ(ToSV(atb_StrSpan_From_NullTerminated(null_terminated_str)),
            "Coucou"sv);
}

TEST(AtbStringSpanTest, Format) {
  char str[] = "Coucou";
  const atb_StrSpan span = atb_StrSpan_From_NullTerminated(str);

  auto expected_str = std::string{str};
  EXPECT_EQ(MakeStringFromFmt(K_ATB_FMT_STR, ATB_FMT_VA_ARG_STR(span)),
            expected_str);

  expected_str = '"' + expected_str + '"';
  EXPECT_EQ(
      MakeStringFromFmt(K_ATB_FMT_STR_QUOTED, ATB_FMT_VA_ARG_STR_QUOTED(span)),
      expected_str);

  {
    std::stringstream ss;
    ss << "{.data=" << (void *)str << ", .size=" << (std::size(str) - 1) << '}';
    expected_str = ss.str();
  }

  EXPECT_EQ(MakeStringFromFmt(K_ATB_FMT_STR_RAW, ATB_FMT_VA_ARG_STR_RAW(span)),
            expected_str);
}

TEST(AtbStringSpanTest, IsValid) {
  atb_StrSpan span = K_ATB_STRSPAN_INVALID;
  EXPECT_NPRED1(atb_StrSpan_IsValid, span);

  std::string str = "Coucou";
  span = atb_StrSpan_From(str.data(), str.size());
  EXPECT_PRED1(atb_StrSpan_IsValid, span);
}

TEST(AtbStringSpanTest, Slice) {
  char str[] = "Coucou";
  auto span = atb_StrSpan_From_Array(str);

  EXPECT_EQ(ToSV(atb_StrSpan_Slice(span, 0, span.size)), ToSV(span));
  EXPECT_EQ(ToSV(atb_StrSpan_Slice(span, 2, 0)), ToSV(span).substr(2, 0));
  EXPECT_EQ(ToSV(atb_StrSpan_Slice(span, 1, 1)), ToSV(span).substr(1, 1));
  EXPECT_EQ(ToSV(atb_StrSpan_Slice(span, 0, span.size + 10)), ToSV(span));
  EXPECT_EQ(ToSV(atb_StrSpan_Slice(span, span.size, 0)),
            ToSV(span).substr(span.size, 0));
  EXPECT_EQ(ToSV(atb_StrSpan_Slice(span, span.size, span.size + 10)),
            ToSV(span).substr(span.size, 0));

  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkFront(span, 0)), ToSV(span));
  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkFront(span, 2)), ToSV(span).substr(2));
  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkFront(span, span.size)),
            ToSV(span).substr(span.size));
  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkFront(span, span.size + 20)),
            ToSV(span).substr(span.size));

  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkBack(span, 0)), ToSV(span));
  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkBack(span, 2)),
            ToSV(span).substr(0, span.size - 2));
  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkBack(span, span.size)),
            ToSV(span).substr(0, 0));
  EXPECT_EQ(ToSV(atb_StrSpan_ShrinkBack(span, span.size + 20)),
            ToSV(span).substr(0, 0));
}

TEST(AtbStringSpanDeathTest, Slice) {
  EXPECT_DEBUG_DEATH(atb_StrSpan_Slice(K_ATB_STRSPAN_INVALID, 0, 0),
                     "atb_StrSpan_IsValid\\\(span\\)");

  EXPECT_DEBUG_DEATH(atb_StrSpan_ShrinkBack(K_ATB_STRSPAN_INVALID, 0),
                     "atb_StrSpan_IsValid\\\(span\\)");
}

} // namespace

} // namespace atb

auto operator<<(std::ostream &os, atb_StrSpan span) -> std::ostream & {
  using atb::MakeStringFromFmt;

  os << MakeStringFromFmt(K_ATB_FMT_STR_RAW, ATB_FMT_VA_ARG_STR_RAW(span));

  if (span.data != nullptr) {
    os << " --> "
       << MakeStringFromFmt(K_ATB_FMT_STR_QUOTED,
                            ATB_FMT_VA_ARG_STR_QUOTED(span));
  }

  return os;
}
