#include "atb/memory/format.h"
#include "atb/memory/view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "helper/Core.hpp"
#include "helper/MemView.hpp"

namespace {

TEST(AtbMemoryViewTest, From) {
  atb_MemView view;
  {
    char str[] = "Coucou";

    view = atb_MemView_From(str, 1);
    EXPECT_EQ(view.data, str);
    EXPECT_EQ(view.size, 1);

    view = atb_MemView_From_Array(str);
    EXPECT_EQ(view.data, str);
    EXPECT_EQ(view.size, std::size(str));
  }

  {
    uint64_t v;
    view = atb_MemView_From_Value(v);
    EXPECT_EQ(view.data, &v);
    EXPECT_EQ(view.size, 8);

    uint32_t *v_p_32 = reinterpret_cast<uint32_t *>(&v);
    view = atb_MemView_From_Ptr(v_p_32);
    EXPECT_EQ(view.data, &v);
    EXPECT_EQ(view.size, 4);

    uint16_t *v_p_16 = reinterpret_cast<uint16_t *>(&v);
    view = atb_MemView_From_Ptr(v_p_16);
    EXPECT_EQ(view.data, &v);
    EXPECT_EQ(view.size, 2);

    uint8_t *v_p_8 = reinterpret_cast<uint8_t *>(&v);
    view = atb_MemView_From_Ptr(v_p_8);
    EXPECT_EQ(view.data, &v);
    EXPECT_EQ(view.size, 1);
  }
}

TEST(AtbMemoryViewTest, Format) {
  using helper::MakeStringFromFmt;

  char str[] = "Coucou";
  auto view = atb_MemView_From(str, std::size(str));

  std::stringstream ss;
  ss << "{.data=" << (void *)str << ", .size=" << std::size(str) << '}';

  EXPECT_EQ(MakeStringFromFmt(K_ATB_FMT_MEM, ATB_FMT_VA_ARG_MEM(view)),
            ss.str());
}

TEST(AtbMemoryViewTest, IsValid) {
  atb_MemView view = K_ATB_MEMVIEW_INVALID;
  EXPECT_NPRED1(atb_MemView_IsValid, view);

  char str[] = "Coucou";
  view = atb_MemView_From(str, std::size(str));
  EXPECT_PRED1(atb_MemView_IsValid, view);
}

TEST(AtbMemoryViewDeathTest, IsOverlapping) {
  EXPECT_DEBUG_DEATH(
      atb_MemView_IsOverlapping(K_ATB_MEMVIEW_INVALID, K_ATB_MEMVIEW_INVALID),
      "atb_MemView_IsValid\\(lhs\\)");

  int v;
  const atb_MemView valid_view = atb_MemView_From_Value(v);

  EXPECT_DEBUG_DEATH(
      atb_MemView_IsOverlapping(K_ATB_MEMVIEW_INVALID, valid_view),
      "atb_MemView_IsValid\\(lhs\\)");

  EXPECT_DEBUG_DEATH(
      atb_MemView_IsOverlapping(valid_view, K_ATB_MEMVIEW_INVALID),
      "atb_MemView_IsValid\\(rhs\\)");
}

TEST(AtbMemoryViewTest, IsOverlapping) {
  uint64_t v;
  atb_MemView view = atb_MemView_From_Value(v);
  EXPECT_PRED2(atb_MemView_IsOverlapping, view, view);

  atb_MemView other_view = view;
  other_view.size -= 1;
  EXPECT_PRED2(atb_MemView_IsOverlapping, view, other_view);
  EXPECT_PRED2(atb_MemView_IsOverlapping, other_view, view);

  other_view = view;
  other_view.data = ((uint8_t *)other_view.data) + 1;
  EXPECT_PRED2(atb_MemView_IsOverlapping, view, other_view);
  EXPECT_PRED2(atb_MemView_IsOverlapping, other_view, view);

  other_view.data = ((uint8_t *)view.data) + view.size - 1;
  EXPECT_PRED2(atb_MemView_IsOverlapping, view, other_view);
  EXPECT_PRED2(atb_MemView_IsOverlapping, other_view, view);

  other_view.data = ((uint8_t *)view.data) + view.size;
  EXPECT_NPRED2(atb_MemView_IsOverlapping, view, other_view);
  EXPECT_NPRED2(atb_MemView_IsOverlapping, other_view, view);

  other_view.data = ((uint8_t *)view.data) + view.size + 1;
  EXPECT_NPRED2(atb_MemView_IsOverlapping, view, other_view);
  EXPECT_NPRED2(atb_MemView_IsOverlapping, other_view, view);

  other_view.size = 0;
  EXPECT_NPRED2(atb_MemView_IsOverlapping, view, other_view);
  EXPECT_NPRED2(atb_MemView_IsOverlapping, other_view, view);
}

TEST(AtbMemoryViewDeathTest, Slice) {
  EXPECT_DEBUG_DEATH(atb_MemView_Slice(K_ATB_MEMVIEW_INVALID, 0, 0),
                     "atb_MemView_IsValid\\(view\\)");
}

TEST(AtbMemoryViewTest, Slice) {
  using helper::FieldsMatch;

  char str[] = "Coucou";

  auto view = atb_MemView_From_Array(str);

  // No op
  EXPECT_THAT(atb_MemView_Slice(view, 0, view.size), FieldsMatch(view));

  EXPECT_THAT(atb_MemView_Slice(view, 1, 1), FieldsMatch({
                                                 .data = (char *)view.data + 1,
                                                 .size = 1,
                                             }));

  EXPECT_THAT(atb_MemView_Slice(view, 3, 1), FieldsMatch({
                                                 .data = (char *)view.data + 3,
                                                 .size = 1,
                                             }));

  EXPECT_THAT(atb_MemView_Slice(view, 3, 3), FieldsMatch({
                                                 .data = (char *)view.data + 3,
                                                 .size = 3,
                                             }));

  EXPECT_THAT(atb_MemView_Slice(view, 1, 3), FieldsMatch({
                                                 .data = (char *)view.data + 1,
                                                 .size = 3,
                                             }));

  EXPECT_THAT(atb_MemView_Slice(view, 4, view.size + 20),
              FieldsMatch({
                  .data = (char *)view.data + 4,
                  .size = view.size - 4,
              }));

  EXPECT_THAT(atb_MemView_Slice(view, view.size, 0),
              FieldsMatch({
                  .data = (char *)view.data + view.size,
                  .size = 0,
              }));

  EXPECT_THAT(atb_MemView_Slice(view, view.size, 20),
              FieldsMatch({
                  .data = (char *)view.data + view.size,
                  .size = 0,
              }));
}

TEST(AtbMemoryViewDeathTest, ShrinkFront) {
  EXPECT_DEBUG_DEATH(atb_MemView_ShrinkFront(K_ATB_MEMVIEW_INVALID, 0),
                     "atb_MemView_IsValid\\(view\\)");
}

TEST(AtbMemoryViewTest, ShrinkFront) {
  using helper::FieldsMatch;

  char str[] = "Coucou";

  auto view = atb_MemView_From_Array(str);

  // No op
  EXPECT_THAT(atb_MemView_ShrinkFront(view, 0), FieldsMatch(view));

  EXPECT_THAT(atb_MemView_ShrinkFront(view, 2),
              FieldsMatch({
                  .data = (char *)view.data + 2,
                  .size = view.size - 2,
              }));

  EXPECT_THAT(atb_MemView_ShrinkFront(view, view.size),
              FieldsMatch({
                  .data = (char *)view.data + view.size,
                  .size = 0,
              }));

  EXPECT_THAT(atb_MemView_ShrinkFront(view, view.size + 20),
              FieldsMatch({
                  .data = (char *)view.data + view.size,
                  .size = 0,
              }));
}

TEST(AtbMemoryViewDeathTest, ShrinkBack) {
  EXPECT_DEBUG_DEATH(atb_MemView_ShrinkBack(K_ATB_MEMVIEW_INVALID, 0),
                     "atb_MemView_IsValid\\(view\\)");
}

TEST(AtbMemoryViewTest, ShrinkBack) {
  using helper::FieldsMatch;

  char str[] = "Coucou";

  auto view = atb_MemView_From_Array(str);

  // No op
  EXPECT_THAT(atb_MemView_ShrinkBack(view, 0), FieldsMatch(view));

  EXPECT_THAT(atb_MemView_ShrinkBack(view, 2), FieldsMatch({
                                                   .data = view.data,
                                                   .size = view.size - 2,
                                               }));

  EXPECT_THAT(atb_MemView_ShrinkBack(view, view.size), FieldsMatch({
                                                           .data = view.data,
                                                           .size = 0,
                                                       }));

  EXPECT_THAT(atb_MemView_ShrinkBack(view, view.size + 20),
              FieldsMatch({
                  .data = view.data,
                  .size = 0,
              }));
}

TEST(AtbMemoryViewDeathTest, Compare) {
  EXPECT_DEBUG_DEATH(
      atb_MemView_Compare(K_ATB_MEMVIEW_INVALID, K_ATB_MEMVIEW_INVALID),
      "atb_MemView_IsValid\\(lhs\\)");

  int v;
  const atb_MemView valid_view = atb_MemView_From_Value(v);

  EXPECT_DEBUG_DEATH(atb_MemView_Compare(K_ATB_MEMVIEW_INVALID, valid_view),
                     "atb_MemView_IsValid\\(lhs\\)");

  EXPECT_DEBUG_DEATH(atb_MemView_Compare(valid_view, K_ATB_MEMVIEW_INVALID),
                     "atb_MemView_IsValid\\(rhs\\)");
}

TEST(AtbMemoryViewTest, Compare) {
  uint8_t data[2] = {1, 2};
  uint8_t data_same[2] = {1, 2};

  uint8_t data_gt[2] = {1, 3};
  uint8_t data_lt[2] = {0, 3};

  uint8_t data_bigger[3] = {0, 3, 0};
  uint8_t data_smaller[1] = {4};

  auto view = atb_MemView_From_Array(data);
  EXPECT_EQ(atb_MemView_Compare(view, view), 0);
  EXPECT_EQ(atb_MemView_Compare(view, atb_MemView_From_Array(data_same)), 0);
  EXPECT_EQ(atb_MemView_Compare(atb_MemView_From_Array(data_same), view), 0);

  EXPECT_GT(atb_MemView_Compare(view, atb_MemView_ShrinkBack(view, 1)), 0);
  EXPECT_GT(atb_MemView_Compare(view, atb_MemView_From_Array(data_smaller)), 0);
  EXPECT_GT(atb_MemView_Compare(view, atb_MemView_From_Array(data_lt)), 0);
  EXPECT_GT(atb_MemView_Compare(atb_MemView_From_Array(data_bigger), view), 0);
  EXPECT_GT(atb_MemView_Compare(atb_MemView_From_Array(data_gt), view), 0);

  EXPECT_LT(atb_MemView_Compare(atb_MemView_ShrinkBack(view, 1), view), 0);
  EXPECT_LT(atb_MemView_Compare(view, atb_MemView_From_Array(data_bigger)), 0);
  EXPECT_LT(atb_MemView_Compare(view, atb_MemView_From_Array(data_gt)), 0);
  EXPECT_LT(atb_MemView_Compare(atb_MemView_From_Array(data_smaller), view), 0);
  EXPECT_LT(atb_MemView_Compare(atb_MemView_From_Array(data_lt), view), 0);

  EXPECT_PRED2(atb_MemView_Eq, view, view);
  EXPECT_NPRED2(atb_MemView_Ne, view, view);
  EXPECT_PRED2(atb_MemView_Ge, view, view);
  EXPECT_PRED2(atb_MemView_Le, view, view);
  EXPECT_NPRED2(atb_MemView_Gt, view, view);
  EXPECT_NPRED2(atb_MemView_Lt, view, view);

  EXPECT_NPRED2(atb_MemView_Eq, view, atb_MemView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_MemView_Ne, view, atb_MemView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_MemView_Ge, view, atb_MemView_ShrinkBack(view, 1));
  EXPECT_NPRED2(atb_MemView_Le, view, atb_MemView_ShrinkBack(view, 1));
  EXPECT_PRED2(atb_MemView_Gt, view, atb_MemView_ShrinkBack(view, 1));
  EXPECT_NPRED2(atb_MemView_Lt, view, atb_MemView_ShrinkBack(view, 1));

  EXPECT_NPRED2(atb_MemView_Eq, atb_MemView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_MemView_Ne, atb_MemView_ShrinkBack(view, 1), view);
  EXPECT_NPRED2(atb_MemView_Ge, atb_MemView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_MemView_Le, atb_MemView_ShrinkBack(view, 1), view);
  EXPECT_NPRED2(atb_MemView_Gt, atb_MemView_ShrinkBack(view, 1), view);
  EXPECT_PRED2(atb_MemView_Lt, atb_MemView_ShrinkBack(view, 1), view);
}

TEST(AtbMemoryViewDeathTest, CopyInto) {
  uint64_t v = 0;
  auto valid_span = atb_MemSpan_From_Value(v);
  auto valid_view = atb_MemView_From_Value(v);

  EXPECT_DEBUG_DEATH(
      atb_MemView_CopyInto(K_ATB_MEMVIEW_INVALID, valid_span, {}),
      "atb_MemView_IsValid\\\(view\\)");

  EXPECT_DEBUG_DEATH(
      atb_MemView_CopyInto(K_ATB_MEMVIEW_INVALID, K_ATB_MEMSPAN_INVALID, {}),
      "atb_MemView_IsValid\\\(view\\)");

  EXPECT_DEBUG_DEATH(
      atb_MemView_CopyInto(valid_view, K_ATB_MEMSPAN_INVALID, {}),
      "atb_MemSpan_IsValid\\\(dest\\)");

  EXPECT_DEBUG_DEATH(atb_MemView_CopyInto(valid_view, valid_span,
                                          {
                                              .truncate = false,
                                              .overlap = false,
                                          }),
                     "!atb_MemView_IsOverlapping");
}

TEST(AtbMemoryViewTest, CopyInto) {
  const uint8_t from[4] = {1, 2, 3, 4};
  uint8_t dest[4];

  std::fill(std::begin(dest), std::end(dest), 0);
  EXPECT_THAT(dest, testing::ElementsAre(0, 0, 0, 0));

  EXPECT_TRUE(atb_MemView_CopyInto(atb_MemView_From_Array(from),
                                   atb_MemSpan_From_Array(dest),
                                   {
                                       .truncate = false,
                                       .overlap = false,
                                   }));
  EXPECT_THAT(dest, testing::ElementsAre(1, 2, 3, 4));

  // Truncate
  std::fill(std::begin(dest), std::end(dest), 0);
  EXPECT_FALSE(atb_MemView_CopyInto(
      atb_MemView_From_Array(from),
      atb_MemSpan_ShrinkBack(atb_MemSpan_From_Array(dest), 1),
      {
          .truncate = false,
          .overlap = false,
      }));
  EXPECT_THAT(dest, testing::ElementsAre(0, 0, 0, 0));

  EXPECT_TRUE(atb_MemView_CopyInto(
      atb_MemView_From_Array(from),
      atb_MemSpan_ShrinkBack(atb_MemSpan_From_Array(dest), 1),
      {
          .truncate = true,
          .overlap = false,
      }));
  EXPECT_THAT(dest, testing::ElementsAre(1, 2, 3, 0));

  // Overlap - view after dest
  EXPECT_TRUE(atb_MemView_CopyInto(
      atb_MemView_Slice(atb_MemView_From_Array(dest), 2, 2),
      atb_MemSpan_Slice(atb_MemSpan_From_Array(dest), 1, 2),
      {
          .truncate = false,
          .overlap = true,
      }));
  EXPECT_THAT(dest, testing::ElementsAre(1, 3, 0, 0));

  // Overlap - view before dest
  EXPECT_TRUE(atb_MemView_CopyInto(
      atb_MemView_Slice(atb_MemView_From_Array(dest), 0, 2),
      atb_MemSpan_Slice(atb_MemSpan_From_Array(dest), 1, 2),
      {
          .truncate = false,
          .overlap = true,
      }));
  EXPECT_THAT(dest, testing::ElementsAre(1, 1, 3, 0));
}

} // namespace
