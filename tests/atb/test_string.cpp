#include <iomanip>
#include <ostream>
#include <string>
#include <string_view>
using namespace std::string_view_literals;

#include "atb/string.h"
#include "gtest/gtest.h"
#include "helper/Core.hpp"
using helper::DoNot;
using helper::MakeStringFromFmt;

constexpr auto ToSV(atb_StrView atb_view) -> std::string_view {
  return std::string_view(atb_view.data, atb_view.size);
}

constexpr auto ToSV(atb_StrSpan atb_span) -> std::string_view {
  return std::string_view(atb_span.data, atb_span.size);
}

inline auto operator<<(std::ostream &os, atb_StrView view) -> std::ostream & {
  os << MakeStringFromFmt(K_ATB_STR_FMT_RAW, atb_Str_FMT_RAW_VA_ARG(view));

  if (view.data != nullptr) {
    os << " --> " << std::quoted(ToSV(view));
  }

  return os;
}

inline auto operator<<(std::ostream &os, atb_StrSpan span) -> std::ostream & {
  return os << atb_StrView_From_Span(span);
}

inline auto operator<<(std::ostream &os,
                       atb_StrView_CopyInto_Opt opt) -> std::ostream & {
  os << '{';
  os << ".truncate=" << (opt.truncate ? "True"sv : "False"sv) << ", ";
  os << ".overlap=" << (opt.overlap ? "True"sv : "False"sv) << ", ";
  os << '}';
  return os;
}

namespace {

TEST(AtbStringTest, ViewFromLiteral) {
  atb_StrView view = atb_StrView_From_Literal("Coucou");
  EXPECT_EQ(ToSV(view), "Coucou"sv);
}

TEST(AtbStringTest, ViewFrom) {
  constexpr auto str = "Coucou"sv;
  EXPECT_EQ(ToSV(atb_StrView_From(str.data(), str.size())), str);
}

TEST(AtbStringTest, ViewFromNullTerminated) {
  constexpr char null_terminated_str[] = "Coucou";
  EXPECT_EQ(ToSV(atb_StrView_From_NullTerminated(null_terminated_str)),
            "Coucou"sv);
}

TEST(AtbStringTest, ViewSet) {
  constexpr auto str = "Coucou"sv;
  atb_StrView view = K_ATB_STRVIEW_INVALID;
  atb_StrView_Set(&view, str.data(), str.size());
  EXPECT_EQ(ToSV(view), str);
}

TEST(AtbStringTest, ViewSetNullTerminated) {
  constexpr char null_terminated_str[] = "Coucou";
  atb_StrView view = K_ATB_STRVIEW_INVALID;
  atb_StrView_Set_NullTerminated(&view, null_terminated_str);
  EXPECT_EQ(ToSV(view), "Coucou"sv);
}

TEST(AtbStringTest, ViewIsValid) {
  atb_StrView view = K_ATB_STRVIEW_INVALID;
  EXPECT_PRED1(DoNot(atb_StrView_IsValid), view);

  constexpr auto str = "Coucou"sv;
  view = atb_StrView_From(str.data(), str.size());
  EXPECT_PRED1(atb_StrView_IsValid, view);
}

TEST(AtbStringTest, SpanFromArray) {
  char str[] = {'C', 'o', 'u', 'c', 'o', 'u'};
  atb_StrSpan span = atb_StrSpan_From_Array(str);
  EXPECT_EQ(ToSV(span), "Coucou"sv);
}

TEST(AtbStringTest, SpanFrom) {
  std::string str = "Coucou";
  EXPECT_EQ(ToSV(atb_StrSpan_From(str.data(), str.size())), str);
}

TEST(AtbStringTest, SpanFromNullTerminated) {
  char null_terminated_str[] = "Coucou";
  EXPECT_EQ(ToSV(atb_StrSpan_From_NullTerminated(null_terminated_str)),
            "Coucou"sv);
}

TEST(AtbStringTest, SpanSet) {
  std::string str = "Coucou";
  atb_StrSpan span = K_ATB_STRSPAN_INVALID;
  atb_StrSpan_Set(&span, str.data(), str.size());
  EXPECT_EQ(ToSV(span), str);
}

TEST(AtbStringTest, SpanSetNullTerminated) {
  char null_terminated_str[] = "Coucou";
  atb_StrSpan span = K_ATB_STRSPAN_INVALID;
  atb_StrSpan_Set_NullTerminated(&span, null_terminated_str);
  EXPECT_EQ(ToSV(span), "Coucou"sv);
}

TEST(AtbStringTest, SpanIsValid) {
  atb_StrSpan span = K_ATB_STRSPAN_INVALID;
  EXPECT_PRED1(DoNot(atb_StrSpan_IsValid), span);

  std::string str = "Coucou";
  span = atb_StrSpan_From(str.data(), str.size());
  EXPECT_PRED1(atb_StrSpan_IsValid, span);
}

TEST(AtbStringTest, ViewCopyInto) {
  auto view = atb_StrView_From_Literal("ABCDE");

  char str[5];
  auto str_span = atb_StrSpan_From_Array(str);

  std::fill(std::begin(str), std::end(str), '\0');
  EXPECT_NE(ToSV(str_span), ToSV(view));
  EXPECT_PRED3(atb_StrView_CopyInto, view, str_span,
               (atb_StrView_CopyInto_Opt_t{
                   .truncate = false,
                   .overlap = false,
               }));
  EXPECT_EQ(ToSV(str_span), ToSV(view));

  // Failure - Span too small - No Truncate
  view = atb_StrView_From_Literal("View too large");
  std::fill(std::begin(str), std::end(str), '\0');
  EXPECT_PRED3(DoNot(atb_StrView_CopyInto), view, str_span,
               (atb_StrView_CopyInto_Opt_t{
                   .truncate = false,
                   .overlap = false,
               }));
  EXPECT_NE(ToSV(str_span), ToSV(view));

  // Truncate
  std::fill(std::begin(str), std::end(str), '\0');
  EXPECT_PRED3(atb_StrView_CopyInto, view, str_span,
               (atb_StrView_CopyInto_Opt_t{
                   .truncate = true,
                   .overlap = false,
               }));
  EXPECT_EQ(ToSV(str_span), ToSV(view).substr(0, str_span.size));

  // Overlap - View after Span
  view.data = str_span.data + 2;
  view.size = 2;
  EXPECT_PRED3(atb_StrView_CopyInto, view, str_span,
               (atb_StrView_CopyInto_Opt_t{
                   .truncate = false,
                   .overlap = true,
               }));
  EXPECT_EQ(ToSV(str_span), "ewew "sv);

  // Overlap - View before Span
  // 'ewe'
  view.data = str_span.data;
  view.size = 3;

  str_span.data += 1;
  str_span.size -= 1;
  EXPECT_PRED3(atb_StrView_CopyInto, view, str_span,
               (atb_StrView_CopyInto_Opt_t{
                   .truncate = false,
                   .overlap = true,
               }));

  EXPECT_EQ(std::string_view(str), "eewe "sv);
}

TEST(AtbStringDeathTest, ViewCopyInto) {
  char str[5];
  auto valid_span = atb_StrSpan_From_Array(str);
  auto valid_view = atb_StrView_From_Literal("Coucou");

  EXPECT_DEBUG_DEATH(
      atb_StrView_CopyInto(K_ATB_STRVIEW_INVALID, valid_span, {}),
      "atb_StrView_IsValid\\\(view\\)");

  EXPECT_DEBUG_DEATH(
      atb_StrView_CopyInto(K_ATB_STRVIEW_INVALID, K_ATB_STRSPAN_INVALID, {}),
      "atb_StrView_IsValid\\\(view\\)");

  EXPECT_DEBUG_DEATH(
      atb_StrView_CopyInto(valid_view, K_ATB_STRSPAN_INVALID, {}),
      "atb_StrSpan_IsValid\\\(dest\\)");
}

TEST(AtbStringTest, Slice) {
  char str[] = "Coucou";

  auto view = atb_StrView_From_NullTerminated(str);
  EXPECT_EQ(ToSV(atb_StrView_Slice(view, 0, view.size)), ToSV(view));
  EXPECT_EQ(ToSV(atb_StrView_Slice(view, 2, 0)), ToSV(view).substr(2, 0));
  EXPECT_EQ(ToSV(atb_StrView_Slice(view, 1, 1)), ToSV(view).substr(1, 1));
  EXPECT_EQ(ToSV(atb_StrView_Slice(view, 0, view.size + 10)), ToSV(view));
  EXPECT_EQ(ToSV(atb_StrView_Slice(view, view.size, 0)),
            ToSV(view).substr(view.size, 0));
  EXPECT_EQ(ToSV(atb_StrView_Slice(view, view.size, view.size + 10)),
            ToSV(view).substr(view.size, 0));

  EXPECT_EQ(ToSV(atb_StrView_ShrinkFront(view, 0)), ToSV(view));
  EXPECT_EQ(ToSV(atb_StrView_ShrinkFront(view, 2)), ToSV(view).substr(2));
  EXPECT_EQ(ToSV(atb_StrView_ShrinkFront(view, view.size)),
            ToSV(view).substr(view.size));
  EXPECT_EQ(ToSV(atb_StrView_ShrinkFront(view, view.size + 20)),
            ToSV(view).substr(view.size));

  EXPECT_EQ(ToSV(atb_StrView_ShrinkBack(view, 0)), ToSV(view));
  EXPECT_EQ(ToSV(atb_StrView_ShrinkBack(view, 2)),
            ToSV(view).substr(0, view.size - 2));
  EXPECT_EQ(ToSV(atb_StrView_ShrinkBack(view, view.size)),
            ToSV(view).substr(0, 0));
  EXPECT_EQ(ToSV(atb_StrView_ShrinkBack(view, view.size + 20)),
            ToSV(view).substr(0, 0));

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

TEST(AtbStringDeathTest, Slice) {
  EXPECT_DEBUG_DEATH(atb_StrView_Slice(K_ATB_STRVIEW_INVALID, 0, 0),
                     "atb_StrView_IsValid\\\(view\\)");

  EXPECT_DEBUG_DEATH(atb_StrView_ShrinkBack(K_ATB_STRVIEW_INVALID, 0),
                     "atb_StrView_IsValid\\\(view\\)");

  EXPECT_DEBUG_DEATH(atb_StrSpan_Slice(K_ATB_STRSPAN_INVALID, 0, 0),
                     "atb_StrSpan_IsValid\\\(span\\)");

  EXPECT_DEBUG_DEATH(atb_StrSpan_ShrinkBack(K_ATB_STRSPAN_INVALID, 0),
                     "atb_StrSpan_IsValid\\\(span\\)");
}

TEST(AtbStringTest, ViewCompare) {
  auto view = atb_StrView_From_Literal("Coucou");

  EXPECT_EQ(atb_StrView_Compare(view, view), 0);
  EXPECT_EQ(atb_StrView_Compare(view, atb_StrView_From_Literal("Coucou")), 0);
  EXPECT_EQ(atb_StrView_Compare(atb_StrView_From_Literal("Coucou"), view), 0);

  EXPECT_GT(atb_StrView_Compare(view, atb_StrView_ShrinkBack(view, 1)), 0);
  EXPECT_GT(atb_StrView_Compare(view, atb_StrView_From_Literal("Boucou")), 0);
  EXPECT_GT(atb_StrView_Compare(view, atb_StrView_From_Literal("Boucov")), 0);

  EXPECT_LT(atb_StrView_Compare(atb_StrView_ShrinkBack(view, 1), view), 0);
  EXPECT_LT(atb_StrView_Compare(atb_StrView_From_Literal("Boucou"), view), 0);
  EXPECT_LT(atb_StrView_Compare(atb_StrView_From_Literal("Coucot"), view), 0);

  EXPECT_PRED2(atb_StrView_Eq, view, view);
  EXPECT_PRED2(DoNot(atb_StrView_Ne), view, view);
  EXPECT_PRED2(atb_StrView_Ge, view, view);
  EXPECT_PRED2(atb_StrView_Le, view, view);
  EXPECT_PRED2(DoNot(atb_StrView_Gt), view, view);
  EXPECT_PRED2(DoNot(atb_StrView_Lt), view, view);

  EXPECT_PRED2(DoNot(atb_StrView_Eq), view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_StrView_Ne, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_StrView_Ge, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(DoNot(atb_StrView_Le), view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_StrView_Gt, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(DoNot(atb_StrView_Lt), view, atb_StrView_ShrinkBack(view, 1));

  EXPECT_PRED2(DoNot(atb_StrView_Eq), atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_StrView_Ne, atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(DoNot(atb_StrView_Ge), atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_StrView_Le, atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(DoNot(atb_StrView_Gt), atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_StrView_Lt, atb_StrView_ShrinkBack(view, 1), view);
}

TEST(AtbStringDeathTest, ViewCompare) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_Compare(K_ATB_STRVIEW_INVALID, K_ATB_STRVIEW_INVALID),
      "atb_StrView_IsValid\\\(lhs\\)");

  auto valid_view = atb_StrView_From_Literal("Coucou");
  EXPECT_DEBUG_DEATH(atb_StrView_Compare(K_ATB_STRVIEW_INVALID, valid_view),
                     "atb_StrView_IsValid\\\(lhs\\)");

  EXPECT_DEBUG_DEATH(atb_StrView_Compare(valid_view, K_ATB_STRVIEW_INVALID),
                     "atb_StrView_IsValid\\\(rhs\\)");
}

} // namespace
