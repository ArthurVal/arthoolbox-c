#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "helper/Core.hpp"
#include "helper/Memory.hpp"

#include "atb/memory/view.h"

namespace {

TEST(TestAtbMemView, Init) {
  auto mem = atb_MemView{
      reinterpret_cast<void *>(0x1234),
      42,
  };

  atb_MemView_Init(&mem);
  EXPECT_THAT(mem, helper::FieldsMatch({nullptr, 0u}));
}

TEST(TestAtbMemView, Fmt) {
  const auto mem = atb_MemView{
      reinterpret_cast<void *>(0x1234),
      42,
  };

  const auto fmt_str =
      helper::MakeStringFromFmt(atb_MemView_Fmt, atb_MemView_FmtVaArg(mem));

  EXPECT_EQ(fmt_str, "{.data=0x1234, .size=42}")
      << (const void *)mem.data << " " << mem.size;
}

TEST(TestAtbMemView, MakeEmpty) {
  EXPECT_THAT(atb_MemView_MakeEmpty(), helper::FieldsMatch({nullptr, 0u}));
}

TEST(TestAtbMemView, MakeFromStaticArray) {
  int array[] = {1, 2, 3, 4, 5};
  auto mem = atb_MemView_MakeFromStaticArray(array);
  EXPECT_THAT(mem, helper::FieldsMatch({
                       reinterpret_cast<const void *>(array),
                       sizeof(int) * std::size(array),
                   }));
}

TEST(TestAtbMemView, Iter) {
  std::uint32_t array[] = {0, 1, 2, 3, 4};

  auto mem = atb_MemView_MakeFromStaticArray(array);

  EXPECT_EQ(atb_MemView_SizeAs(std::uint8_t, mem), 20);
  EXPECT_EQ(atb_MemView_SizeAs(std::uint32_t, mem), 5);
  EXPECT_EQ(atb_MemView_SizeAs(std::uint64_t, mem), 2);

  EXPECT_EQ(atb_MemView_BeginAs(std::uint32_t, mem), std::cbegin(array));
  EXPECT_EQ(atb_MemView_EndAs(std::uint32_t, mem), std::cend(array));

  EXPECT_EQ(atb_MemView_BeginAs(std::uint64_t, mem),
            (const std::uint64_t *)array);
  EXPECT_EQ(atb_MemView_EndAs(std::uint64_t, mem),
            (const std::uint64_t *)(array + 4));

  EXPECT_EQ(atb_MemView_BeginAsR(std::uint32_t, mem),
            (std::crbegin(array).base() - 1));
  EXPECT_EQ(atb_MemView_EndAsR(std::uint32_t, mem),
            (std::crend(array).base() - 1));

  EXPECT_EQ(atb_MemView_BeginAsR(std::uint8_t, mem),
            ((const std::uint8_t *)(array + 5) - 1));
  EXPECT_EQ(atb_MemView_EndAsR(std::uint8_t, mem),
            ((const std::uint8_t *)array - 1));

  {
    std::uint32_t expected_value = 0;
    const std::uint32_t *val;
    atb_MemView_ForEachAs(std::uint32_t, val, mem) {
      EXPECT_EQ(*val, expected_value++);
    }

    atb_MemView_ForEachAsR(std::uint32_t, val, mem) {
      EXPECT_EQ(*val, --expected_value);
    }
  }

  // {
  //   short *val;
  //   atb_MemView_ForEachAs(short, val, mem) { EXPECT_TRUE(false) <<
  //   (int)*val; }
  // }
}

TEST(TestAtbMemView, IsInvalid) {

  EXPECT_PRED1(atb_MemView_IsInvalid, (atb_MemView{nullptr, 42}));
  EXPECT_PRED1(helper::DoNot(atb_MemView_IsInvalid), (atb_MemView{nullptr, 0}));

  int array[] = {
      0,
  };
  auto mem = atb_MemView_MakeFromStaticArray(array);
  EXPECT_PRED1(helper::DoNot(atb_MemView_IsInvalid), mem);

  mem.size = 0;
  EXPECT_PRED1(helper::DoNot(atb_MemView_IsInvalid), mem);
}

TEST(TestAtbMemView, Sort) {
  int array[] = {0, 1, 2, 3};
  auto lowest = atb_MemView{array, 1};
  auto highest = atb_MemView{array + 1, 3};

  {
    auto [first, second] =
        atb_MemView_Sort(atb_MemView_IsLower, lowest, highest);

    EXPECT_THAT(first, helper::FieldsMatch(lowest));
    EXPECT_THAT(second, helper::FieldsMatch(highest));
  }

  {
    auto [first, second] =
        atb_MemView_Sort(atb_MemView_IsHigher, lowest, highest);

    EXPECT_THAT(first, helper::FieldsMatch(highest));
    EXPECT_THAT(second, helper::FieldsMatch(lowest));
  }

  {
    auto [first, second] = atb_MemView_Sort(
        [](auto lhs, auto rhs) -> bool { return lhs.size > rhs.size; }, lowest,
        highest);

    EXPECT_THAT(first, helper::FieldsMatch(highest));
    EXPECT_THAT(second, helper::FieldsMatch(lowest));
  }

  EXPECT_DEBUG_DEATH({ atb_MemView_Sort(nullptr, lowest, highest); },
                     "Assertion");

  EXPECT_DEBUG_DEATH(
      {
        atb_MemView_Sort(atb_MemView_IsLower, lowest,
                         (atb_MemView{nullptr, 42}));
      },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      {
        atb_MemView_Sort(atb_MemView_IsLower, (atb_MemView{nullptr, 42}),
                         lowest);
      },
      "Assertion");
}

TEST(TestAtbMemView, IsOverlapping) {
  char array[] = {0, 1, 2, 3};

  auto array_span = atb_MemView_MakeFromStaticArray(array);
  auto left = atb_MemView{array, 2};
  auto middle = atb_MemView{array + 1, 2};
  auto right = atb_MemView{array + 2, 2};

  auto null = atb_MemView{nullptr, 0};
  auto empty = atb_MemView{array, 0};

  EXPECT_PRED2(atb_MemView_IsOverlapping, array_span, array_span);
  EXPECT_PRED2(atb_MemView_IsOverlapping, left, left);
  EXPECT_PRED2(atb_MemView_IsOverlapping, middle, middle);
  EXPECT_PRED2(atb_MemView_IsOverlapping, right, right);

  for (auto [lhs, rhs] : {
           std::pair{array_span, left},
           std::pair{array_span, middle},
           std::pair{array_span, right},
           std::pair{left, middle},
           std::pair{middle, right},
       }) {
    EXPECT_PRED2(atb_MemView_IsOverlapping, lhs, rhs);
    EXPECT_PRED2(atb_MemView_IsOverlapping, rhs, lhs);
  }

  for (auto [lhs, rhs] : {
           std::pair{left, right},
           std::pair{left, null},
           std::pair{array_span, empty},
           std::pair{null, empty},
       }) {
    EXPECT_PRED2(helper::DoNot(atb_MemView_IsOverlapping), lhs, rhs);
    EXPECT_PRED2(helper::DoNot(atb_MemView_IsOverlapping), rhs, lhs);
  }

  for (auto [lhs, rhs] : {
           std::pair{array_span, atb_MemView{nullptr, 42}},
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(lhs, rhs));
    EXPECT_DEBUG_DEATH({ atb_MemView_IsOverlapping(lhs, rhs); }, "Assertion");
  }
}

} // namespace
