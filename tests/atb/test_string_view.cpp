#include <string>
#include <string_view>
using namespace std::string_view_literals;

#include "atb/string/view.h"
#include "gtest/gtest.h"
#include "helper/Core.hpp"
#include "helper/StrView.hpp"

namespace {

TEST(AtbStringViewTest, FromLiteral) {
  atb_StrView view = atb_StrView_From_Literal("Coucou");
  EXPECT_EQ(ToSV(view), "Coucou"sv);
}

TEST(AtbStringViewTest, From) {
  constexpr auto str = "Coucou"sv;
  EXPECT_EQ(ToSV(atb_StrView_From(str.data(), str.size())), str);
}

TEST(AtbStringViewTest, FromNullTerminated) {
  constexpr char null_terminated_str[] = "Coucou";
  EXPECT_EQ(ToSV(atb_StrView_From_NullTerminated(null_terminated_str)),
            "Coucou"sv);
}

TEST(AtbStringViewTest, IsValid) {
  atb_StrView view = K_ATB_STRVIEW_INVALID;
  EXPECT_NPRED1(atb_StrView_IsValid, view);

  constexpr auto str = "Coucou"sv;
  view = atb_StrView_From(str.data(), str.size());
  EXPECT_PRED1(atb_StrView_IsValid, view);
}

TEST(AtbStringViewTest, CopyInto) {
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
  EXPECT_NPRED3(atb_StrView_CopyInto, view, str_span,
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

TEST(AtbStringViewDeathTest, CopyInto) {
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

TEST(AtbStringViewTest, Slice) {
  const char str[] = "Coucou";

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
}

TEST(AtbStringViewDeathTest, Slice) {
  EXPECT_DEBUG_DEATH(atb_StrView_Slice(K_ATB_STRVIEW_INVALID, 0, 0),
                     "atb_StrView_IsValid\\\(view\\)");

  EXPECT_DEBUG_DEATH(atb_StrView_ShrinkBack(K_ATB_STRVIEW_INVALID, 0),
                     "atb_StrView_IsValid\\\(view\\)");
}

TEST(AtbStringViewTest, Compare) {
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
  EXPECT_NPRED2(atb_StrView_Ne, view, view);
  EXPECT_PRED2(atb_StrView_Ge, view, view);
  EXPECT_PRED2(atb_StrView_Le, view, view);
  EXPECT_NPRED2(atb_StrView_Gt, view, view);
  EXPECT_NPRED2(atb_StrView_Lt, view, view);

  EXPECT_NPRED2(atb_StrView_Eq, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_StrView_Ne, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_StrView_Ge, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_NPRED2(atb_StrView_Le, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_StrView_Gt, view, atb_StrView_ShrinkBack(view, 1));
  EXPECT_NPRED2(atb_StrView_Lt, view, atb_StrView_ShrinkBack(view, 1));

  EXPECT_NPRED2(atb_StrView_Eq, atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_StrView_Ne, atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_NPRED2(atb_StrView_Ge, atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_StrView_Le, atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_NPRED2(atb_StrView_Gt, atb_StrView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_StrView_Lt, atb_StrView_ShrinkBack(view, 1), view);
}

TEST(AtbStringViewDeathTest, Compare) {
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
