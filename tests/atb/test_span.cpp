#include "test_span.hpp"

#include <algorithm>
#include <numeric>

namespace atb {

auto operator<<(std::ostream &os,
                const atb_View_Copy_Opt_t &opt) -> std::ostream & {
  os << '{';
  os << ".truncate=" << (opt.truncate ? 'Y' : 'N') << ", ";
  os << ".overlap=" << (opt.overlap ? 'Y' : 'N') << ", ";
  os << '}';
  return os;
}

ATB_SPAN_VIEW_DEFINE(, Span_u32, View_u32, std::uint32_t);

auto WriteTo(std::ostream &os, const View_u32 &view) {
  os << '{';
  os << ".data=" << (void *)view.data << ", ";
  os << ".size=" << view.size << ", ";
  os << '}';

  if (view.data == nullptr) {
    os << " -> /!\\ INVALID";
  } else if (view.size != 0) {
    os << " -> [";
    std::uint32_t const *value = nullptr;
    atb_AnySpan_ForEach(value, view) { os << *value << ", "; }
    os << ']';
  }
}

std::ostream &operator<<(std::ostream &os, View_u32 view) {
  os << "View_u32";
  WriteTo(os, view);
  return os;
}

auto operator==(View_u32 lhs, View_u32 rhs) noexcept -> bool {
  return (lhs.data == rhs.data) && (lhs.size == rhs.size);
}

auto operator!=(View_u32 lhs, View_u32 rhs) noexcept -> bool {
  return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, Span_u32 span) {
  os << "Span_u32";
  WriteTo(os, View_u32_From_Span(span));
  return os;
}

auto operator==(Span_u32 lhs, Span_u32 rhs) noexcept -> bool {
  return (lhs.data == rhs.data) && (lhs.size == rhs.size);
}

auto operator!=(Span_u32 lhs, Span_u32 rhs) noexcept -> bool {
  return !(lhs == rhs);
}

namespace {

DefineFieldsMatchFor(Span_u32, 2, data, size);

DefineFieldsMatchFor(View_u32, 2, data, size);

struct AtbSpanTest : testing::Test {
  static inline std::uint32_t m_arr[] = {0, 1, 2, 3, 4, 5, 6};
};

using AtbSpanDeathTest = AtbSpanTest;
using AtbViewTest = AtbSpanTest;
using AtbViewDeathTest = AtbSpanTest;

TEST_F(AtbSpanTest, From) {
  auto span = Span_u32_From(nullptr, 2);
  EXPECT_THAT(span, FieldsMatch(Span_u32{nullptr, 2}));

  span = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(span, FieldsMatch(Span_u32{m_arr, std::size(m_arr)}));

  auto view = View_u32_From(nullptr, 4);
  EXPECT_THAT(view, FieldsMatch(View_u32{nullptr, 4}));

  view = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(view, FieldsMatch(View_u32{m_arr, std::size(m_arr)}));
}

TEST_F(AtbSpanTest, IsValid) {
  Span_u32 span = K_ATB_ANYSPAN_INVALID;
  EXPECT_NPRED1(Span_u32_IsValid, span);

  span = atb_AnySpan_From_Array(m_arr);
  EXPECT_PRED1(Span_u32_IsValid, span);
}

TEST_F(AtbSpanDeathTest, SizeBytes) {
  EXPECT_DEBUG_DEATH(Span_u32_SizeBytes(K_ATB_ANYSPAN_INVALID), "IsValid");
}

TEST_F(AtbSpanDeathTest, IsOverlapping) {
  Span_u32 valid_span = atb_AnySpan_From_Array(m_arr);
  EXPECT_DEBUG_DEATH(Span_u32_IsOverlapping(K_ATB_ANYSPAN_INVALID, valid_span),
                     "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_IsOverlapping(valid_span, K_ATB_ANYSPAN_INVALID),
                     "IsValid");
  EXPECT_DEBUG_DEATH(
      Span_u32_IsOverlapping(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
      "IsValid");
}

TEST_F(AtbSpanTest, IsOverlapping) {
  const Span_u32 all = atb_AnySpan_From_Array(m_arr);
  auto left = Span_u32_First(all, 3);
  auto right = Span_u32_Last(all, 3);
  auto middle = Span_u32_Slice(all, 2, 3);

  // [0, 1, 2, 3, 4, 5, 6]
  //  <----------------->  all
  //  <----->              left
  //        <----->        middle
  //              <-----> right

  EXPECT_PRED2(Span_u32_IsOverlapping, all, all);
  EXPECT_PRED2(Span_u32_IsOverlapping, left, all);
  EXPECT_PRED2(Span_u32_IsOverlapping, all, left);
  EXPECT_PRED2(Span_u32_IsOverlapping, right, all);
  EXPECT_PRED2(Span_u32_IsOverlapping, all, right);
  EXPECT_PRED2(Span_u32_IsOverlapping, middle, all);
  EXPECT_PRED2(Span_u32_IsOverlapping, all, middle);

  EXPECT_PRED2(Span_u32_IsOverlapping, left, middle);
  EXPECT_PRED2(Span_u32_IsOverlapping, middle, right);
  EXPECT_NPRED2(Span_u32_IsOverlapping, left, right);
}

TEST_F(AtbSpanTest, SizeBytes) {
  Span_u32 span = atb_AnySpan_From_Array(m_arr);
  EXPECT_EQ(Span_u32_SizeBytes(span), sizeof(m_arr));
}

TEST_F(AtbSpanDeathTest, First) {
  EXPECT_DEBUG_DEATH(Span_u32_First(K_ATB_ANYSPAN_INVALID, 0), "IsValid");
}

TEST_F(AtbSpanTest, First) {
  Span_u32 span = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(Span_u32_First(span, 2), FieldsMatch(Span_u32{m_arr, 2}));
  EXPECT_THAT(Span_u32_First(span, 4), FieldsMatch(Span_u32{m_arr, 4}));
  EXPECT_THAT(Span_u32_First(span, 5), FieldsMatch(Span_u32{m_arr, 5}));
  EXPECT_THAT(Span_u32_First(span, span.size), FieldsMatch(span));
  EXPECT_THAT(Span_u32_First(span, span.size + 20), FieldsMatch(span));
}

TEST_F(AtbSpanDeathTest, Last) {
  EXPECT_DEBUG_DEATH(Span_u32_Last(K_ATB_ANYSPAN_INVALID, 0), "IsValid");
}

TEST_F(AtbSpanTest, Last) {
  Span_u32 span = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(Span_u32_Last(span, 2),
              FieldsMatch(Span_u32{m_arr + (std::size(m_arr) - 2), 2}));
  EXPECT_THAT(Span_u32_Last(span, 4),
              FieldsMatch(Span_u32{m_arr + (std::size(m_arr) - 4), 4}));
  EXPECT_THAT(Span_u32_Last(span, 5),
              FieldsMatch(Span_u32{m_arr + (std::size(m_arr) - 5), 5}));
  EXPECT_THAT(Span_u32_Last(span, span.size), FieldsMatch(span));
  EXPECT_THAT(Span_u32_Last(span, span.size + 20), FieldsMatch(span));
  EXPECT_THAT(Span_u32_Last(span, 0),
              FieldsMatch(Span_u32{m_arr + std::size(m_arr), 0}));
}

TEST_F(AtbSpanDeathTest, Shrink) {
  EXPECT_DEBUG_DEATH(
      Span_u32_Shrink(K_ATB_ANYSPAN_INVALID, 0, K_ATB_SPAN_SHRINK_FRONT),
      "IsValid");

  Span_u32 valid_span = atb_AnySpan_From_Array(m_arr);
  EXPECT_DEBUG_DEATH(
      Span_u32_Shrink(valid_span, 0, static_cast<ATB_SPAN_SHRINK_DIR>(20)),
      "IsValid");
}

TEST_F(AtbSpanTest, Shrink) {
  Span_u32 span = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(Span_u32_Shrink(span, 2, K_ATB_SPAN_SHRINK_FRONT),
              FieldsMatch(Span_u32{m_arr + 2, (std::size(m_arr) - 2)}));

  EXPECT_THAT(Span_u32_Shrink(span, span.size, K_ATB_SPAN_SHRINK_FRONT),
              FieldsMatch(Span_u32{m_arr + std::size(m_arr), 0}));

  EXPECT_THAT(Span_u32_Shrink(span, span.size + 10, K_ATB_SPAN_SHRINK_FRONT),
              FieldsMatch(Span_u32{m_arr + std::size(m_arr), 0}));

  EXPECT_THAT(Span_u32_Shrink(span, 2, K_ATB_SPAN_SHRINK_BACK),
              FieldsMatch(Span_u32{m_arr, (std::size(m_arr) - 2)}));

  EXPECT_THAT(Span_u32_Shrink(span, span.size, K_ATB_SPAN_SHRINK_BACK),
              FieldsMatch(Span_u32{m_arr, 0}));

  EXPECT_THAT(Span_u32_Shrink(span, span.size + 10, K_ATB_SPAN_SHRINK_BACK),
              FieldsMatch(Span_u32{m_arr, 0}));
}

TEST_F(AtbSpanDeathTest, Slice) {
  EXPECT_DEBUG_DEATH(Span_u32_Slice(K_ATB_ANYSPAN_INVALID, 0, 0), "IsValid");
}

TEST_F(AtbSpanTest, Slice) {
  Span_u32 span = atb_AnySpan_From_Array(m_arr);

  EXPECT_THAT(Span_u32_Slice(span, 0, span.size), FieldsMatch(span));
  EXPECT_THAT(Span_u32_Slice(span, 1, 1), FieldsMatch(Span_u32{m_arr + 1, 1}));
  EXPECT_THAT(Span_u32_Slice(span, 4, 2), FieldsMatch(Span_u32{m_arr + 4, 2}));
  EXPECT_THAT(Span_u32_Slice(span, 4, 0), FieldsMatch(Span_u32{m_arr + 4, 0}));
  EXPECT_THAT(Span_u32_Slice(span, 4, 20),
              FieldsMatch(Span_u32{m_arr + 4, span.size - 4}));
  EXPECT_THAT(Span_u32_Slice(span, 20, 2),
              FieldsMatch(Span_u32{m_arr + span.size, 0}));
  EXPECT_THAT(Span_u32_Slice(span, 20, 20),
              FieldsMatch(Span_u32{m_arr + span.size, 0}));
}

TEST_F(AtbSpanDeathTest, Compare) {
  Span_u32 valid_span = atb_AnySpan_From_Array(m_arr);
  EXPECT_DEBUG_DEATH(Span_u32_Compare(K_ATB_ANYSPAN_INVALID, valid_span),
                     "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Compare(valid_span, K_ATB_ANYSPAN_INVALID),
                     "IsValid");
  EXPECT_DEBUG_DEATH(
      Span_u32_Compare(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
      "IsValid");

  EXPECT_DEBUG_DEATH(Span_u32_Eq(K_ATB_ANYSPAN_INVALID, valid_span), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Eq(valid_span, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Eq(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(Span_u32_Ne(K_ATB_ANYSPAN_INVALID, valid_span), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Ne(valid_span, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Ne(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(Span_u32_Lt(K_ATB_ANYSPAN_INVALID, valid_span), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Lt(valid_span, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Lt(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(Span_u32_Gt(K_ATB_ANYSPAN_INVALID, valid_span), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Gt(valid_span, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Gt(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(Span_u32_Le(K_ATB_ANYSPAN_INVALID, valid_span), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Le(valid_span, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Le(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(Span_u32_Ge(K_ATB_ANYSPAN_INVALID, valid_span), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Ge(valid_span, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Ge(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");
}

TEST_F(AtbSpanTest, Compare) {
  using testing::Eq;
  using testing::Ge;
  using testing::Gt;
  using testing::Le;
  using testing::Lt;
  using testing::Ne;

  const Span_u32 initial = atb_AnySpan_From_Array(m_arr);

  auto lhs = initial;
  auto rhs = initial;

  // Same span
  EXPECT_THAT(Span_u32_Compare(lhs, rhs), Eq(0));
  EXPECT_PRED2(Span_u32_Eq, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Lt, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Gt, lhs, rhs);
  EXPECT_PRED2(Span_u32_Le, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ge, lhs, rhs);

  EXPECT_THAT(Span_u32_Compare(rhs, lhs), Eq(0));
  EXPECT_PRED2(Span_u32_Eq, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Ne, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Gt, rhs, lhs);
  EXPECT_PRED2(Span_u32_Le, rhs, lhs);
  EXPECT_PRED2(Span_u32_Ge, rhs, lhs);

  // Different size
  rhs = Span_u32_Shrink(lhs, 1, K_ATB_SPAN_SHRINK_FRONT);
  EXPECT_THAT(Span_u32_Compare(lhs, rhs), Gt(0));
  EXPECT_NPRED2(Span_u32_Eq, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Lt, lhs, rhs);
  EXPECT_PRED2(Span_u32_Gt, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Le, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ge, lhs, rhs);

  EXPECT_THAT(Span_u32_Compare(rhs, lhs), Lt(0));
  EXPECT_NPRED2(Span_u32_Eq, rhs, lhs);
  EXPECT_PRED2(Span_u32_Ne, rhs, lhs);
  EXPECT_PRED2(Span_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Gt, rhs, lhs);
  EXPECT_PRED2(Span_u32_Le, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Ge, rhs, lhs);

  rhs = Span_u32_Shrink(lhs, 1, K_ATB_SPAN_SHRINK_BACK);
  EXPECT_THAT(Span_u32_Compare(lhs, rhs), Gt(0));
  EXPECT_NPRED2(Span_u32_Eq, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Lt, lhs, rhs);
  EXPECT_PRED2(Span_u32_Gt, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Le, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ge, lhs, rhs);

  EXPECT_THAT(Span_u32_Compare(rhs, lhs), Lt(0));
  EXPECT_NPRED2(Span_u32_Eq, rhs, lhs);
  EXPECT_PRED2(Span_u32_Ne, rhs, lhs);
  EXPECT_PRED2(Span_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Gt, rhs, lhs);
  EXPECT_PRED2(Span_u32_Le, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Ge, rhs, lhs);

  // Same size
  lhs = Span_u32_Slice(initial, 1, 2); // 1, 2
  rhs = Span_u32_Slice(initial, 0, 2); // 0, 1

  EXPECT_THAT(Span_u32_Compare(lhs, rhs), Gt(0));
  EXPECT_NPRED2(Span_u32_Eq, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Lt, lhs, rhs);
  EXPECT_PRED2(Span_u32_Gt, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Le, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ge, lhs, rhs);

  EXPECT_THAT(Span_u32_Compare(rhs, lhs), Lt(0));
  EXPECT_NPRED2(Span_u32_Eq, rhs, lhs);
  EXPECT_PRED2(Span_u32_Ne, rhs, lhs);
  EXPECT_PRED2(Span_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Gt, rhs, lhs);
  EXPECT_PRED2(Span_u32_Le, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Ge, rhs, lhs);

  std::uint32_t other[] = {1, 2};
  rhs = atb_AnySpan_From_Array(other);

  // Same value, different array
  EXPECT_THAT(Span_u32_Compare(lhs, rhs), Eq(0));
  EXPECT_PRED2(Span_u32_Eq, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Lt, lhs, rhs);
  EXPECT_NPRED2(Span_u32_Gt, lhs, rhs);
  EXPECT_PRED2(Span_u32_Le, lhs, rhs);
  EXPECT_PRED2(Span_u32_Ge, lhs, rhs);

  EXPECT_THAT(Span_u32_Compare(rhs, lhs), Eq(0));
  EXPECT_PRED2(Span_u32_Eq, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Ne, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(Span_u32_Gt, rhs, lhs);
  EXPECT_PRED2(Span_u32_Le, rhs, lhs);
  EXPECT_PRED2(Span_u32_Ge, rhs, lhs);
}

TEST_F(AtbSpanDeathTest, Fill) {
  Span_u32 valid_span = atb_AnySpan_From_Array(m_arr);
  uint32_t v = 0;

  EXPECT_DEBUG_DEATH(Span_u32_Fill(K_ATB_ANYSPAN_INVALID, &v), "IsValid");
  EXPECT_DEBUG_DEATH(Span_u32_Fill(valid_span, nullptr), "!= NULL");
}

TEST_F(AtbSpanTest, Fill) {
  using testing::Each;
  using testing::Eq;
  using testing::Ne;

  std::uint32_t arr[] = {1, 1, 1, 1, 1};

  std::uint32_t value = 42;
  EXPECT_THAT(arr, Each(Ne(value)));
  Span_u32_Fill(atb_AnySpan_From_Array(arr), &value);
  EXPECT_THAT(arr, Each(Eq(value)));
}

TEST_F(AtbViewTest, From) {
  auto view = View_u32_From(nullptr, 2);
  EXPECT_THAT(view, FieldsMatch(View_u32{nullptr, 2}));

  view = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(view, FieldsMatch(View_u32{m_arr, std::size(m_arr)}));
}

TEST_F(AtbViewTest, IsValid) {
  View_u32 view = K_ATB_ANYSPAN_INVALID;
  EXPECT_NPRED1(View_u32_IsValid, view);

  view = atb_AnySpan_From_Array(m_arr);
  EXPECT_PRED1(View_u32_IsValid, view);
}

TEST_F(AtbViewDeathTest, SizeBytes) {
  EXPECT_DEBUG_DEATH(View_u32_SizeBytes(K_ATB_ANYSPAN_INVALID), "IsValid");
}

TEST_F(AtbViewDeathTest, IsOverlapping) {
  View_u32 valid_view = atb_AnySpan_From_Array(m_arr);
  EXPECT_DEBUG_DEATH(View_u32_IsOverlapping(K_ATB_ANYSPAN_INVALID, valid_view),
                     "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_IsOverlapping(valid_view, K_ATB_ANYSPAN_INVALID),
                     "IsValid");
  EXPECT_DEBUG_DEATH(
      View_u32_IsOverlapping(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
      "IsValid");
}

TEST_F(AtbViewTest, IsOverlapping) {
  const View_u32 all = atb_AnySpan_From_Array(m_arr);
  auto left = View_u32_First(all, 3);
  auto right = View_u32_Last(all, 3);
  auto middle = View_u32_Slice(all, 2, 3);

  // [0, 1, 2, 3, 4, 5, 6]
  //  <----------------->  all
  //  <----->              left
  //        <----->        middle
  //              <-----> right

  EXPECT_PRED2(View_u32_IsOverlapping, all, all);
  EXPECT_PRED2(View_u32_IsOverlapping, left, all);
  EXPECT_PRED2(View_u32_IsOverlapping, all, left);
  EXPECT_PRED2(View_u32_IsOverlapping, right, all);
  EXPECT_PRED2(View_u32_IsOverlapping, all, right);
  EXPECT_PRED2(View_u32_IsOverlapping, middle, all);
  EXPECT_PRED2(View_u32_IsOverlapping, all, middle);

  EXPECT_PRED2(View_u32_IsOverlapping, left, middle);
  EXPECT_PRED2(View_u32_IsOverlapping, middle, right);
  EXPECT_NPRED2(View_u32_IsOverlapping, left, right);
}

TEST_F(AtbViewTest, SizeBytes) {
  View_u32 view = atb_AnySpan_From_Array(m_arr);
  EXPECT_EQ(View_u32_SizeBytes(view), sizeof(m_arr));
}

TEST_F(AtbViewDeathTest, First) {
  EXPECT_DEBUG_DEATH(View_u32_First(K_ATB_ANYSPAN_INVALID, 0), "IsValid");
}

TEST_F(AtbViewTest, First) {
  View_u32 view = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(View_u32_First(view, 2), FieldsMatch(View_u32{m_arr, 2}));
  EXPECT_THAT(View_u32_First(view, 4), FieldsMatch(View_u32{m_arr, 4}));
  EXPECT_THAT(View_u32_First(view, 5), FieldsMatch(View_u32{m_arr, 5}));
  EXPECT_THAT(View_u32_First(view, view.size), FieldsMatch(view));
  EXPECT_THAT(View_u32_First(view, view.size + 20), FieldsMatch(view));
}

TEST_F(AtbViewDeathTest, Last) {
  EXPECT_DEBUG_DEATH(View_u32_Last(K_ATB_ANYSPAN_INVALID, 0), "IsValid");
}

TEST_F(AtbViewTest, Last) {
  View_u32 view = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(View_u32_Last(view, 2),
              FieldsMatch(View_u32{m_arr + (std::size(m_arr) - 2), 2}));
  EXPECT_THAT(View_u32_Last(view, 4),
              FieldsMatch(View_u32{m_arr + (std::size(m_arr) - 4), 4}));
  EXPECT_THAT(View_u32_Last(view, 5),
              FieldsMatch(View_u32{m_arr + (std::size(m_arr) - 5), 5}));
  EXPECT_THAT(View_u32_Last(view, view.size), FieldsMatch(view));
  EXPECT_THAT(View_u32_Last(view, view.size + 20), FieldsMatch(view));
  EXPECT_THAT(View_u32_Last(view, 0),
              FieldsMatch(View_u32{m_arr + std::size(m_arr), 0}));
}

TEST_F(AtbViewDeathTest, Shrink) {
  EXPECT_DEBUG_DEATH(
      View_u32_Shrink(K_ATB_ANYSPAN_INVALID, 0, K_ATB_SPAN_SHRINK_FRONT),
      "IsValid");

  View_u32 valid_view = atb_AnySpan_From_Array(m_arr);
  EXPECT_DEBUG_DEATH(
      View_u32_Shrink(valid_view, 0, static_cast<ATB_SPAN_SHRINK_DIR>(20)),
      "IsValid");
}

TEST_F(AtbViewTest, Shrink) {
  View_u32 view = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(View_u32_Shrink(view, 2, K_ATB_SPAN_SHRINK_FRONT),
              FieldsMatch(View_u32{m_arr + 2, (std::size(m_arr) - 2)}));

  EXPECT_THAT(View_u32_Shrink(view, view.size, K_ATB_SPAN_SHRINK_FRONT),
              FieldsMatch(View_u32{m_arr + std::size(m_arr), 0}));

  EXPECT_THAT(View_u32_Shrink(view, view.size + 10, K_ATB_SPAN_SHRINK_FRONT),
              FieldsMatch(View_u32{m_arr + std::size(m_arr), 0}));

  EXPECT_THAT(View_u32_Shrink(view, 2, K_ATB_SPAN_SHRINK_BACK),
              FieldsMatch(View_u32{m_arr, (std::size(m_arr) - 2)}));

  EXPECT_THAT(View_u32_Shrink(view, view.size, K_ATB_SPAN_SHRINK_BACK),
              FieldsMatch(View_u32{m_arr, 0}));

  EXPECT_THAT(View_u32_Shrink(view, view.size + 10, K_ATB_SPAN_SHRINK_BACK),
              FieldsMatch(View_u32{m_arr, 0}));
}

TEST_F(AtbViewDeathTest, Slice) {
  EXPECT_DEBUG_DEATH(View_u32_Slice(K_ATB_ANYSPAN_INVALID, 0, 0), "IsValid");
}

TEST_F(AtbViewTest, Slice) {
  View_u32 view = atb_AnySpan_From_Array(m_arr);

  EXPECT_THAT(View_u32_Slice(view, 0, view.size), FieldsMatch(view));
  EXPECT_THAT(View_u32_Slice(view, 1, 1), FieldsMatch(View_u32{m_arr + 1, 1}));
  EXPECT_THAT(View_u32_Slice(view, 4, 2), FieldsMatch(View_u32{m_arr + 4, 2}));
  EXPECT_THAT(View_u32_Slice(view, 4, 0), FieldsMatch(View_u32{m_arr + 4, 0}));
  EXPECT_THAT(View_u32_Slice(view, 4, 20),
              FieldsMatch(View_u32{m_arr + 4, view.size - 4}));
  EXPECT_THAT(View_u32_Slice(view, 20, 2),
              FieldsMatch(View_u32{m_arr + view.size, 0}));
  EXPECT_THAT(View_u32_Slice(view, 20, 20),
              FieldsMatch(View_u32{m_arr + view.size, 0}));
}

TEST_F(AtbViewDeathTest, Compare) {
  View_u32 valid_view = atb_AnySpan_From_Array(m_arr);
  EXPECT_DEBUG_DEATH(View_u32_Compare(K_ATB_ANYSPAN_INVALID, valid_view),
                     "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Compare(valid_view, K_ATB_ANYSPAN_INVALID),
                     "IsValid");
  EXPECT_DEBUG_DEATH(
      View_u32_Compare(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
      "IsValid");

  EXPECT_DEBUG_DEATH(View_u32_Eq(K_ATB_ANYSPAN_INVALID, valid_view), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Eq(valid_view, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Eq(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(View_u32_Ne(K_ATB_ANYSPAN_INVALID, valid_view), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Ne(valid_view, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Ne(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(View_u32_Lt(K_ATB_ANYSPAN_INVALID, valid_view), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Lt(valid_view, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Lt(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(View_u32_Gt(K_ATB_ANYSPAN_INVALID, valid_view), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Gt(valid_view, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Gt(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(View_u32_Le(K_ATB_ANYSPAN_INVALID, valid_view), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Le(valid_view, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Le(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");

  EXPECT_DEBUG_DEATH(View_u32_Ge(K_ATB_ANYSPAN_INVALID, valid_view), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Ge(valid_view, K_ATB_ANYSPAN_INVALID), "IsValid");
  EXPECT_DEBUG_DEATH(View_u32_Ge(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
                     "IsValid");
}

TEST_F(AtbViewTest, Compare) {
  using testing::Eq;
  using testing::Ge;
  using testing::Gt;
  using testing::Le;
  using testing::Lt;
  using testing::Ne;

  const View_u32 initial = atb_AnySpan_From_Array(m_arr);

  auto lhs = initial;
  auto rhs = initial;

  // Same view
  EXPECT_THAT(View_u32_Compare(lhs, rhs), Eq(0));
  EXPECT_PRED2(View_u32_Eq, lhs, rhs);
  EXPECT_NPRED2(View_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(View_u32_Lt, lhs, rhs);
  EXPECT_NPRED2(View_u32_Gt, lhs, rhs);
  EXPECT_PRED2(View_u32_Le, lhs, rhs);
  EXPECT_PRED2(View_u32_Ge, lhs, rhs);

  EXPECT_THAT(View_u32_Compare(rhs, lhs), Eq(0));
  EXPECT_PRED2(View_u32_Eq, rhs, lhs);
  EXPECT_NPRED2(View_u32_Ne, rhs, lhs);
  EXPECT_NPRED2(View_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(View_u32_Gt, rhs, lhs);
  EXPECT_PRED2(View_u32_Le, rhs, lhs);
  EXPECT_PRED2(View_u32_Ge, rhs, lhs);

  // Different size
  rhs = View_u32_Shrink(lhs, 1, K_ATB_SPAN_SHRINK_FRONT);
  EXPECT_THAT(View_u32_Compare(lhs, rhs), Gt(0));
  EXPECT_NPRED2(View_u32_Eq, lhs, rhs);
  EXPECT_PRED2(View_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(View_u32_Lt, lhs, rhs);
  EXPECT_PRED2(View_u32_Gt, lhs, rhs);
  EXPECT_NPRED2(View_u32_Le, lhs, rhs);
  EXPECT_PRED2(View_u32_Ge, lhs, rhs);

  EXPECT_THAT(View_u32_Compare(rhs, lhs), Lt(0));
  EXPECT_NPRED2(View_u32_Eq, rhs, lhs);
  EXPECT_PRED2(View_u32_Ne, rhs, lhs);
  EXPECT_PRED2(View_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(View_u32_Gt, rhs, lhs);
  EXPECT_PRED2(View_u32_Le, rhs, lhs);
  EXPECT_NPRED2(View_u32_Ge, rhs, lhs);

  rhs = View_u32_Shrink(lhs, 1, K_ATB_SPAN_SHRINK_BACK);
  EXPECT_THAT(View_u32_Compare(lhs, rhs), Gt(0));
  EXPECT_NPRED2(View_u32_Eq, lhs, rhs);
  EXPECT_PRED2(View_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(View_u32_Lt, lhs, rhs);
  EXPECT_PRED2(View_u32_Gt, lhs, rhs);
  EXPECT_NPRED2(View_u32_Le, lhs, rhs);
  EXPECT_PRED2(View_u32_Ge, lhs, rhs);

  EXPECT_THAT(View_u32_Compare(rhs, lhs), Lt(0));
  EXPECT_NPRED2(View_u32_Eq, rhs, lhs);
  EXPECT_PRED2(View_u32_Ne, rhs, lhs);
  EXPECT_PRED2(View_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(View_u32_Gt, rhs, lhs);
  EXPECT_PRED2(View_u32_Le, rhs, lhs);
  EXPECT_NPRED2(View_u32_Ge, rhs, lhs);

  // Same size
  lhs = View_u32_Slice(initial, 1, 2); // 1, 2
  rhs = View_u32_Slice(initial, 0, 2); // 0, 1

  EXPECT_THAT(View_u32_Compare(lhs, rhs), Gt(0));
  EXPECT_NPRED2(View_u32_Eq, lhs, rhs);
  EXPECT_PRED2(View_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(View_u32_Lt, lhs, rhs);
  EXPECT_PRED2(View_u32_Gt, lhs, rhs);
  EXPECT_NPRED2(View_u32_Le, lhs, rhs);
  EXPECT_PRED2(View_u32_Ge, lhs, rhs);

  EXPECT_THAT(View_u32_Compare(rhs, lhs), Lt(0));
  EXPECT_NPRED2(View_u32_Eq, rhs, lhs);
  EXPECT_PRED2(View_u32_Ne, rhs, lhs);
  EXPECT_PRED2(View_u32_Lt, rhs, lhs);
  EXPECT_NPRED2(View_u32_Gt, rhs, lhs);
  EXPECT_PRED2(View_u32_Le, rhs, lhs);
  EXPECT_NPRED2(View_u32_Ge, rhs, lhs);

  std::uint32_t other[] = {1, 2};
  rhs = atb_AnySpan_From_Array(other);

  // Same value, different array
  EXPECT_THAT(View_u32_Compare(lhs, rhs), Eq(0));
  EXPECT_PRED2(View_u32_Eq, lhs, rhs);
  EXPECT_NPRED2(View_u32_Ne, lhs, rhs);
  EXPECT_NPRED2(View_u32_Lt, lhs, rhs);
  EXPECT_NPRED2(View_u32_Gt, lhs, rhs);
  EXPECT_PRED2(View_u32_Le, lhs, rhs);
  EXPECT_PRED2(View_u32_Ge, lhs, rhs);

  EXPECT_THAT(View_u32_Compare(rhs, lhs), Eq(0));
  EXPECT_PRED2(View_u32_Eq, rhs, lhs);
  EXPECT_NPRED2(View_u32_Ne, rhs, lhs);
  EXPECT_NPRED2(View_u32_Lt, rhs, lhs);

  EXPECT_PRED2(View_u32_Ge, rhs, lhs);
}

TEST_F(AtbViewTest, From_Span) {
  EXPECT_THAT(View_u32_From_Span(K_ATB_ANYSPAN_INVALID),
              FieldsMatch((View_u32)K_ATB_ANYSPAN_INVALID));

  Span_u32 span = atb_AnySpan_From_Array(m_arr);
  EXPECT_THAT(View_u32_From_Span(span),
              FieldsMatch(View_u32{m_arr, std::size(m_arr)}));
}

TEST_F(AtbViewDeathTest, Copy) {
  Span_u32 valid_span = atb_AnySpan_From_Array(m_arr);
  View_u32 valid_view = atb_AnySpan_From_Array(m_arr);

  EXPECT_DEBUG_DEATH(
      View_u32_Copy(K_ATB_ANYSPAN_INVALID, valid_span, {}, &valid_span),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      View_u32_Copy(valid_view, K_ATB_ANYSPAN_INVALID, {}, &valid_span),
      "IsValid");

  EXPECT_DEBUG_DEATH(View_u32_Copy(valid_view, valid_span, {}, nullptr),
                     "!= NULL");

  EXPECT_DEBUG_DEATH(
      View_u32_Copy(valid_view, valid_span,
                    {.truncate = false, .overlap = false}, &valid_span),
      "IsOverlapping");
}

TEST_F(AtbViewTest, Copy) {
  using testing::ContainerEq;
  using testing::Each;
  using testing::ElementsAre;

  std::uint32_t buff[std::size(m_arr)];
  const Span_u32 dest = atb_AnySpan_From_Array(buff);

  Span_u32 span = dest;
  View_u32 view = atb_AnySpan_From_Array(m_arr);
  Span_u32 remaining;

  remaining = K_ATB_ANYSPAN_INVALID;
  std::fill(std::begin(buff), std::end(buff), 0);
  EXPECT_TRUE(View_u32_Copy(view, span, {.truncate = false, .overlap = false},
                            &remaining));
  EXPECT_THAT(remaining, FieldsMatch(Span_u32{std::end(buff), 0}));
  EXPECT_THAT(buff, ContainerEq(m_arr));

  // Dest too big
  span = Span_u32_First(dest, 2);

  remaining = K_ATB_ANYSPAN_INVALID;
  std::fill(std::begin(buff), std::end(buff), 0);
  EXPECT_FALSE(View_u32_Copy(view, span, {.truncate = false, .overlap = false},
                             &remaining));
  EXPECT_THAT(remaining, FieldsMatch((Span_u32)K_ATB_ANYSPAN_INVALID));
  EXPECT_THAT(buff, Each(0));

  // Truncate
  remaining = K_ATB_ANYSPAN_INVALID;
  std::fill(std::begin(buff), std::end(buff), 0);
  EXPECT_TRUE(View_u32_Copy(view, span, {.truncate = true, .overlap = false},
                            &remaining));
  EXPECT_THAT(remaining,
              FieldsMatch(Span_u32{std::next(span.data, span.size), 0}));
  EXPECT_THAT(buff, ElementsAre(0, 1, 0, 0, 0, 0, 0));

  // Overlap
  // -- Dest BEFORE
  // buff = 10 11 12 13 14 15 16
  std::iota(std::begin(buff), std::end(buff), 10);
  span = Span_u32_First(dest, 4);                    // 10 11 12 13
  view = View_u32_Last(View_u32_From_Span(dest), 4); // 13 14 15 16

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(View_u32_Copy(view, span, {.truncate = false, .overlap = true},
                            &remaining));
  EXPECT_THAT(remaining,
              FieldsMatch(Span_u32{std::next(span.data, view.size), 0}));
  EXPECT_THAT(buff, ElementsAre(13, 14, 15, 16, 14, 15, 16));

  // -- Dest AFTER
  // buff = 10 11 12 13 14 15 16
  std::iota(std::begin(buff), std::end(buff), 10);
  span = Span_u32_Last(dest, 4);                      // 13 14 15 16
  view = View_u32_First(View_u32_From_Span(dest), 4); // 10 11 12 13

  remaining = K_ATB_ANYSPAN_INVALID;
  EXPECT_TRUE(View_u32_Copy(view, span, {.truncate = false, .overlap = true},
                            &remaining));
  EXPECT_THAT(remaining,
              FieldsMatch(Span_u32{std::next(span.data, view.size), 0}));
  EXPECT_THAT(buff, ElementsAre(10, 11, 12, 10, 11, 12, 13));
}

} // namespace

} // namespace atb
