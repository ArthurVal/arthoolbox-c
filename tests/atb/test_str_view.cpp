#include <string_view>

#include "atb/str_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "helper/StrView.hpp"

namespace {

class TestAtbStrView : public ::testing::Test {
  struct TestStrView {
    atb_StrView valid;
    atb_StrView corrupted;
    atb_StrView null;
    atb_StrView empty;
  };

 protected:
  void SetUp() {
    {
      constexpr auto str = atb_StrView_MakeFromLiteral("foo");
      str_example.valid = str;
      str_example.corrupted = atb_StrView{nullptr, str.size};
      str_example.null = atb_StrView{nullptr, 0};
      str_example.empty = atb_StrView_MakeFromLiteral("");
    }
  }

  void TearDown() {}

  TestStrView str_example;
};

/***************************************************************************/
/*                                Construct                                */
/***************************************************************************/
TEST_F(TestAtbStrView, Init) {
  auto str = atb_StrView{
      reinterpret_cast<const char *>(0x1234),
      42,
  };
  atb_StrView_Init(&str);

  EXPECT_THAT(str, helper::FieldsMatch({nullptr, 0u}));
}

TEST_F(TestAtbStrView, MakeEmpty) {
  EXPECT_EQ(atb_StrView_MakeEmpty(), str_example.null);
}

TEST_F(TestAtbStrView, MakeFromLiteral) {
  auto view = atb_StrView_MakeFromLiteral("Foo");
  EXPECT_EQ(view, "Foo");
}

TEST_F(TestAtbStrView, MakeFromCString) {
  EXPECT_THAT(atb_StrView_MakeFromCString(nullptr, -1),
              helper::FieldsMatch({nullptr, 0u}));

  EXPECT_THAT(atb_StrView_MakeFromCString(nullptr, 0),
              helper::FieldsMatch({nullptr, 0u}));

  EXPECT_THAT(atb_StrView_MakeFromCString(nullptr, 2),
              helper::FieldsMatch({nullptr, 0u}));

  constexpr std::string_view str = "Coucou";
  EXPECT_THAT(atb_StrView_MakeFromCString(str.data(), -1),
              helper::FieldsMatch({str.data(), str.size()}));

  EXPECT_THAT(atb_StrView_MakeFromCString(str.data(), 2),
              helper::FieldsMatch({str.data(), 2}));
}

/***************************************************************************/
/*                                Operations                               */
/***************************************************************************/
/* Checks ******************************************************************/
TEST_F(TestAtbStrView, IsCorrupted) {
  EXPECT_PRED1(atb_StrView_IsCorrupted, str_example.corrupted);
  EXPECT_PRED1(helper::DoNot(atb_StrView_IsCorrupted), str_example.valid);
  EXPECT_PRED1(helper::DoNot(atb_StrView_IsCorrupted), str_example.null);
  EXPECT_PRED1(helper::DoNot(atb_StrView_IsCorrupted), str_example.empty);
}

TEST_F(TestAtbStrView, IsOverlapping) {
  const auto str = atb_StrView_MakeFromLiteral("ABCDEF");

  const auto left = atb_StrView_SliceBack(str, 3);    // ABC
  const auto middle = atb_StrView_Slice(str, 1, 4);   // BCDE
  const auto right = atb_StrView_SliceFront(str, 3);  // DEF

  EXPECT_PRED2(atb_StrView_IsOverlapping, str, str);

  for (auto [lhs, rhs] : {
           std::pair{str, left},
           std::pair{str, right},
           std::pair{str, middle},
           std::pair{middle, left},
           std::pair{middle, right},
       }) {
    EXPECT_PRED2(atb_StrView_IsOverlapping, lhs, rhs);
    EXPECT_PRED2(atb_StrView_IsOverlapping, rhs, lhs);
  }

  for (auto [lhs, rhs] : {
           std::pair{left, right},
           std::pair{str, str_example.valid},
           std::pair{str_example.valid, str_example.null},
           std::pair{str_example.valid, str_example.empty},
       }) {
    EXPECT_PRED2(helper::DoNot(atb_StrView_IsOverlapping), lhs, rhs);
    EXPECT_PRED2(helper::DoNot(atb_StrView_IsOverlapping), rhs, lhs);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_IsOverlapping(str_example.corrupted, str_example.valid); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_IsOverlapping(str_example.valid, str_example.corrupted); },
      "Assertion");
}

/* Slicing *****************************************************************/
TEST_F(TestAtbStrView, Slicing) {
  EXPECT_EQ(atb_StrView_SliceBack(str_example.empty, 0), "");
  EXPECT_EQ(atb_StrView_SliceBack(str_example.empty, 1), "");
  EXPECT_EQ(atb_StrView_SliceBack(str_example.empty, 50), "");

  EXPECT_EQ(atb_StrView_SliceBack(atb_StrView_MakeFromLiteral("foo"), 0),
            "foo");

  EXPECT_EQ(atb_StrView_SliceBack(atb_StrView_MakeFromLiteral("foo"), 1), "fo");

  EXPECT_EQ(atb_StrView_SliceBack(atb_StrView_MakeFromLiteral("foo"), 2), "f");

  EXPECT_EQ(atb_StrView_SliceBack(atb_StrView_MakeFromLiteral("foo"), 20), "");

  EXPECT_EQ(atb_StrView_SliceFront(atb_StrView_MakeFromLiteral("bar"), 0),
            "bar");

  EXPECT_EQ(atb_StrView_SliceFront(atb_StrView_MakeFromLiteral("bar"), 1),
            "ar");

  EXPECT_EQ(atb_StrView_SliceFront(atb_StrView_MakeFromLiteral("bar"), 2), "r");

  EXPECT_EQ(atb_StrView_SliceFront(atb_StrView_MakeFromLiteral("bar"), 20), "");

  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 0, 0), "");
  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 1, 0), "");
  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 50, 0), "");
  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 50, 50), "");

  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 0, 1), "b");
  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 0, 2), "ba");
  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 0, 50),
            "baz");

  EXPECT_EQ(atb_StrView_Slice(atb_StrView_MakeFromLiteral("baz"), 1, 1), "a");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_SliceBack(str_example.corrupted, 0); }, "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_SliceFront(str_example.corrupted, 0); }, "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Slice(str_example.corrupted, 0, 0); }, "Assertion");
}

/* Comparison **************************************************************/
TEST_F(TestAtbStrView, Compare) {
  EXPECT_EQ(atb_StrView_Compare(str_example.null, str_example.null),
            atb_StrView_Compare_EQUAL);

  EXPECT_EQ(atb_StrView_Compare(str_example.empty, str_example.empty),
            atb_StrView_Compare_EQUAL);

  EXPECT_EQ(atb_StrView_Compare(str_example.empty, str_example.null),
            atb_StrView_Compare_EQUAL);

  EXPECT_EQ(atb_StrView_Compare(str_example.valid, str_example.valid),
            atb_StrView_Compare_EQUAL);

  for (auto smaller_str : {
           atb_StrView_MakeFromLiteral("fo"),
           atb_StrView_MakeFromLiteral("f"),
           atb_StrView_MakeFromLiteral("foe"),
           atb_StrView_MakeFromLiteral("feo"),
           atb_StrView_MakeFromLiteral("eoo"),
           str_example.empty,
           str_example.null,
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(str_example.valid, smaller_str));

    EXPECT_EQ(atb_StrView_Compare(smaller_str, str_example.valid),
              atb_StrView_Compare_LESS);

    EXPECT_EQ(atb_StrView_Compare(str_example.valid, smaller_str),
              atb_StrView_Compare_GREATER);
  }

  for (auto greater_str : {
           atb_StrView_MakeFromLiteral("fooo"),
           atb_StrView_MakeFromLiteral("fow"),
           atb_StrView_MakeFromLiteral("fwo"),
           atb_StrView_MakeFromLiteral("goo"),
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(str_example.valid, greater_str));

    EXPECT_EQ(atb_StrView_Compare(greater_str, str_example.valid),
              atb_StrView_Compare_GREATER);

    EXPECT_EQ(atb_StrView_Compare(str_example.valid, greater_str),
              atb_StrView_Compare_LESS);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Compare(str_example.corrupted, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Compare(str_example.corrupted, str_example.valid); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Compare(str_example.valid, str_example.corrupted); },
      "Assertion");
}

TEST_F(TestAtbStrView, Eq) {
  EXPECT_PRED2(atb_StrView_Eq, str_example.null, str_example.null);
  EXPECT_PRED2(atb_StrView_Eq, str_example.empty, str_example.empty);

  for (auto str : {
           atb_StrView_MakeFromLiteral("bar"),
           str_example.valid,
           str_example.empty,
           str_example.null,
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_1(str));
    EXPECT_PRED2(atb_StrView_Eq, str, str);
  }

  for (auto other_str : {
           atb_StrView_MakeFromLiteral("bar"),
           str_example.empty,
           str_example.null,
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(str_example.valid, other_str));
    EXPECT_PRED2(helper::DoNot(atb_StrView_Eq), other_str, str_example.valid);
    EXPECT_PRED2(helper::DoNot(atb_StrView_Eq), str_example.valid, other_str);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Eq(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Eq(str_example.corrupted, str_example.valid); },
      "Assertion");
}

TEST_F(TestAtbStrView, Ne) {
  EXPECT_PRED2(helper::DoNot(atb_StrView_Ne), str_example.null,
               str_example.null);
  EXPECT_PRED2(helper::DoNot(atb_StrView_Ne), str_example.empty,
               str_example.empty);

  for (auto str : {
           atb_StrView_MakeFromLiteral("bar"),
           str_example.valid,
           str_example.empty,
           str_example.null,
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_1(str));
    EXPECT_PRED2(helper::DoNot(atb_StrView_Ne), str, str);
  }

  for (auto other_str : {
           atb_StrView_MakeFromLiteral("bar"),
           str_example.empty,
           str_example.null,
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(str_example.valid, other_str));
    EXPECT_PRED2(atb_StrView_Ne, other_str, str_example.valid);
    EXPECT_PRED2(atb_StrView_Ne, str_example.valid, other_str);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Ne(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Ne(str_example.corrupted, str_example.valid); },
      "Assertion");
}

TEST_F(TestAtbStrView, GtLt) {
  EXPECT_PRED2(helper::DoNot(atb_StrView_Lt), str_example.null,
               str_example.null);
  EXPECT_PRED2(helper::DoNot(atb_StrView_Lt), str_example.empty,
               str_example.empty);

  EXPECT_PRED2(helper::DoNot(atb_StrView_Gt), str_example.null,
               str_example.null);
  EXPECT_PRED2(helper::DoNot(atb_StrView_Gt), str_example.empty,
               str_example.empty);

  for (auto greater_str : {
           atb_StrView_MakeFromLiteral("fooo"),
           atb_StrView_MakeFromLiteral("fow"),
           atb_StrView_MakeFromLiteral("fwo"),
           atb_StrView_MakeFromLiteral("goo"),
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(str_example.valid, greater_str));

    EXPECT_PRED2(atb_StrView_Gt, greater_str, str_example.valid);
    EXPECT_PRED2(helper::DoNot(atb_StrView_Gt), str_example.valid, greater_str);

    EXPECT_PRED2(helper::DoNot(atb_StrView_Lt), greater_str, str_example.valid);
    EXPECT_PRED2(atb_StrView_Lt, str_example.valid, greater_str);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Gt(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Gt(str_example.corrupted, str_example.valid); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Lt(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Lt(str_example.corrupted, str_example.valid); },
      "Assertion");
}

TEST_F(TestAtbStrView, GeLe) {
  EXPECT_PRED2(atb_StrView_Le, str_example.null, str_example.null);
  EXPECT_PRED2(atb_StrView_Le, str_example.empty, str_example.empty);

  EXPECT_PRED2(atb_StrView_Ge, str_example.null, str_example.null);
  EXPECT_PRED2(atb_StrView_Ge, str_example.empty, str_example.empty);

  for (auto greater_str : {
           atb_StrView_MakeFromLiteral("fooo"),
           atb_StrView_MakeFromLiteral("fow"),
           atb_StrView_MakeFromLiteral("fwo"),
           atb_StrView_MakeFromLiteral("goo"),
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_2(str_example.valid, greater_str));

    EXPECT_PRED2(atb_StrView_Ge, greater_str, str_example.valid);
    EXPECT_PRED2(helper::DoNot(atb_StrView_Ge), str_example.valid, greater_str);

    EXPECT_PRED2(helper::DoNot(atb_StrView_Le), greater_str, str_example.valid);
    EXPECT_PRED2(atb_StrView_Le, str_example.valid, greater_str);
  }

  for (auto str : {
           atb_StrView_MakeFromLiteral("bar"),
           str_example.valid,
           str_example.empty,
           str_example.null,
       }) {
    SCOPED_TRACE(SCOPE_LOOP_MSG_1(str));
    EXPECT_PRED2(atb_StrView_Ge, str, str);
    EXPECT_PRED2(atb_StrView_Le, str, str);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Ge(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Ge(str_example.corrupted, str_example.valid); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Le(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Le(str_example.corrupted, str_example.valid); },
      "Assertion");
}

/* Lookup *****************************************************************/
TEST_F(TestAtbStrView, Find) {
  constexpr auto main_str = atb_StrView_MakeFromLiteral("This is a phrase");

  for (auto [substr, expected_offset] : {
           std::pair{main_str, 0u},
           std::pair{atb_StrView_MakeFromLiteral("T"), 0u},
           std::pair{atb_StrView_MakeFromLiteral("s"), 3u},
           std::pair{atb_StrView_MakeFromLiteral("se"),
                     (unsigned)(main_str.size - 2)},
           std::pair{atb_StrView_MakeFromLiteral("This"), 0u},
           std::pair{atb_StrView_MakeFromLiteral("a"), 8u},
       }) {
    auto slice = atb_StrView_Find(main_str, substr);

    SCOPED_TRACE(SCOPE_LOOP_MSG_4(main_str, substr, expected_offset, slice));
    EXPECT_EQ(slice.data, main_str.data + expected_offset);
    EXPECT_EQ(slice.size, main_str.size - expected_offset);
  }

  for (auto wrong_substr : {
           atb_StrView_MakeFromLiteral("foo"),
           atb_StrView_MakeFromLiteral("a chocolatine"),
           atb_StrView_MakeFromLiteral("This is a phrase but longer"),
           str_example.empty,
           str_example.null,
       }) {
    auto slice = atb_StrView_Find(main_str, wrong_substr);

    SCOPED_TRACE(SCOPE_LOOP_MSG_3(main_str, wrong_substr, slice));
    EXPECT_EQ(slice.data, main_str.data + main_str.size);
    EXPECT_EQ(slice.size, 0);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Find(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_Find(str_example.corrupted, str_example.valid); },
      "Assertion");
}

TEST_F(TestAtbStrView, FindR) {
  constexpr auto main_str = atb_StrView_MakeFromLiteral("This is a phrase");

  for (auto [substr, expected_offset] : {
           std::pair{main_str, 0u},
           std::pair{atb_StrView_MakeFromLiteral("T"), 0u},
           std::pair{atb_StrView_MakeFromLiteral("s"),
                     (unsigned)(main_str.size - 2)},
           std::pair{atb_StrView_MakeFromLiteral("This"), 0u},
           std::pair{atb_StrView_MakeFromLiteral("a"),
                     (unsigned)(main_str.size - 3)},
       }) {
    auto slice = atb_StrView_FindR(main_str, substr);

    SCOPED_TRACE(SCOPE_LOOP_MSG_4(main_str, substr, expected_offset, slice));
    EXPECT_EQ(slice.data, main_str.data + expected_offset);
    EXPECT_EQ(slice.size, main_str.size - expected_offset);
  }

  for (auto wrong_substr : {
           atb_StrView_MakeFromLiteral("foo"),
           atb_StrView_MakeFromLiteral("a chocolatine"),
           atb_StrView_MakeFromLiteral("This is a phrase but longer"),
           str_example.empty,
           str_example.null,
       }) {
    auto slice = atb_StrView_FindR(main_str, wrong_substr);

    SCOPED_TRACE(SCOPE_LOOP_MSG_3(main_str, wrong_substr, slice));
    EXPECT_EQ(slice.data, main_str.data + main_str.size);
    EXPECT_EQ(slice.size, 0);
  }

  EXPECT_DEBUG_DEATH(
      { atb_StrView_FindR(str_example.valid, str_example.corrupted); },
      "Assertion");

  EXPECT_DEBUG_DEATH(
      { atb_StrView_FindR(str_example.corrupted, str_example.valid); },
      "Assertion");
}

}  // namespace
