#include "test_span_string.hpp"

using namespace std::string_view_literals;

auto operator<<(std::ostream &os, atb_StrSpan span) -> std::ostream & {
  os << "StrSpan";
  atb::StreamSpanLayoutTo(os, span);

  if (span.data != nullptr) {
    os << " -> " << std::quoted(atb::ToSv(span));
  }

  return os;
}

auto operator<<(std::ostream &os, atb_StrView view) -> std::ostream & {
  os << "StrView";
  atb::StreamSpanLayoutTo(os, view);

  if (view.data != nullptr) {
    os << " -> " << std::quoted(atb::ToSv(view));
  }

  return os;
}

namespace atb {

namespace {

TEST(AtbSpanStringTest, FromNullTerminated) {
  char null_terminated_str[] = "Coucou";
  EXPECT_EQ(ToSv(atb_StrSpan_From_NullTerminated(null_terminated_str)),
            "Coucou"sv);

  const char const_null_terminated_str[] = "Ciao";
  EXPECT_EQ(ToSv(atb_StrView_From_NullTerminated(const_null_terminated_str)),
            "Ciao"sv);
}

TEST(AtbSpanStringDeathTest, StartsWith) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                             K_ATB_ANYSPAN_INVALID),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_StartsWith(K_ATB_ANYSPAN_INVALID,
                             atb_StrView_From_StrLiteral("Coucou")),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_StartsWith(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
      "IsValid");
}

TEST(AtbSpanStringTest, StartsWith) {
  EXPECT_TRUE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                     atb_StrView_From_StrLiteral("C")));

  EXPECT_TRUE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                     atb_StrView_From_StrLiteral("Co")));

  EXPECT_TRUE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                     atb_StrView_From_StrLiteral("Cou")));

  EXPECT_TRUE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                     atb_StrView_From_StrLiteral("Coucou")));

  EXPECT_FALSE(
      atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                             atb_StrView_From_StrLiteral("Coucoucoucou")));

  EXPECT_FALSE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                      atb_StrView_From_StrLiteral("cou")));

  EXPECT_FALSE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                      atb_StrView_From_StrLiteral("Kou")));

  EXPECT_FALSE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                      atb_StrView_From_StrLiteral("Co ")));

  EXPECT_FALSE(atb_StrView_StartsWith(atb_StrView_From_StrLiteral("Coucou"),
                                      atb_StrView_From_StrLiteral("")));
}

TEST(AtbSpanStringDeathTest, EndsWith) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                           K_ATB_ANYSPAN_INVALID),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_EndsWith(K_ATB_ANYSPAN_INVALID,
                           atb_StrView_From_StrLiteral("Chocolatine")),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_EndsWith(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID),
      "IsValid");
}

TEST(AtbSpanStringTest, EndsWith) {
  EXPECT_TRUE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                   atb_StrView_From_StrLiteral("e")));

  EXPECT_TRUE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                   atb_StrView_From_StrLiteral("ne")));

  EXPECT_TRUE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                   atb_StrView_From_StrLiteral("ine")));

  EXPECT_TRUE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                   atb_StrView_From_StrLiteral("Chocolatine")));

  EXPECT_FALSE(
      atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                           atb_StrView_From_StrLiteral("Chocolatinecoucou")));

  EXPECT_FALSE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                    atb_StrView_From_StrLiteral("cou")));

  EXPECT_FALSE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                    atb_StrView_From_StrLiteral("Kou")));

  EXPECT_FALSE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                    atb_StrView_From_StrLiteral("Co ")));

  EXPECT_FALSE(atb_StrView_EndsWith(atb_StrView_From_StrLiteral("Chocolatine"),
                                    atb_StrView_From_StrLiteral("")));
}

TEST(AtbSpanStringDeathTest, Find) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                       K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_Find(K_ATB_ANYSPAN_INVALID,
                       atb_StrView_From_StrLiteral("Chocolatine"), NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_Find(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");
}

TEST(AtbSpanStringTest, Find) {
  EXPECT_TRUE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("co"), NULL));

  size_t offset = 0;
  EXPECT_TRUE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("co"), &offset));
  EXPECT_EQ(offset, 3);

  offset = 1;
  EXPECT_TRUE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("Chocolatine"),
                               &offset));
  EXPECT_EQ(offset, 0);

  offset = 1;
  EXPECT_TRUE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("Cho"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 1;
  EXPECT_TRUE(atb_StrView_Find(atb_StrView_From_StrLiteral("ChotineCho"),
                               atb_StrView_From_StrLiteral("Cho"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 0;
  EXPECT_TRUE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("la"), &offset));
  EXPECT_EQ(offset, 5);

  offset = 0;
  EXPECT_TRUE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("e"), &offset));
  EXPECT_EQ(offset, 10);

  offset = 20;
  EXPECT_FALSE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("tina"), &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("Chocolatine "),
                                &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(atb_StrView_Find(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral(""), &offset));
  EXPECT_EQ(offset, 20);
}

TEST(AtbSpanStringDeathTest, RFind) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                        K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_RFind(K_ATB_ANYSPAN_INVALID,
                        atb_StrView_From_StrLiteral("Chocolatine"), NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_RFind(K_ATB_ANYSPAN_INVALID, K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");
}

TEST(AtbSpanStringTest, RFind) {
  EXPECT_TRUE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("co"), NULL));

  size_t offset = 0;
  EXPECT_TRUE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("co"), &offset));
  EXPECT_EQ(offset, 3);

  offset = 1;
  EXPECT_TRUE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("Chocolatine"),
                                &offset));
  EXPECT_EQ(offset, 0);

  offset = 1;
  EXPECT_TRUE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("Cho"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 1;
  EXPECT_TRUE(atb_StrView_RFind(atb_StrView_From_StrLiteral("ChotineCho"),
                                atb_StrView_From_StrLiteral("Cho"), &offset));
  EXPECT_EQ(offset, 7);

  offset = 0;
  EXPECT_TRUE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("la"), &offset));
  EXPECT_EQ(offset, 5);

  offset = 0;
  EXPECT_TRUE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                atb_StrView_From_StrLiteral("e"), &offset));
  EXPECT_EQ(offset, 10);

  offset = 20;
  EXPECT_FALSE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                 atb_StrView_From_StrLiteral("tina"), &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                 atb_StrView_From_StrLiteral("Chocolatine "),
                                 &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(atb_StrView_RFind(atb_StrView_From_StrLiteral("Chocolatine"),
                                 atb_StrView_From_StrLiteral(""), &offset));
  EXPECT_EQ(offset, 20);
}

TEST(AtbSpanStringDeathTest, Find_FirstOf) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("Chocolatine"),
                               K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(atb_StrView_Find_FirstOf(
                         K_ATB_ANYSPAN_INVALID,
                         atb_StrView_From_StrLiteral("Chocolatine"), NULL),
                     "IsValid");

  EXPECT_DEBUG_DEATH(atb_StrView_Find_FirstOf(K_ATB_ANYSPAN_INVALID,
                                              K_ATB_ANYSPAN_INVALID, NULL),
                     "IsValid");
}

TEST(AtbSpanStringTest, Find_FirstOf) {
  EXPECT_TRUE(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("co"), NULL));

  size_t offset = 0;
  EXPECT_TRUE(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("co"), &offset));
  EXPECT_EQ(offset, 2);

  offset = 1;
  EXPECT_TRUE(atb_StrView_Find_FirstOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("enitalocohC"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 1;
  EXPECT_TRUE(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("580aXV"), &offset));
  EXPECT_EQ(offset, 6);

  offset = 1;
  EXPECT_TRUE(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("ChotineCho"),
                               atb_StrView_From_StrLiteral("C"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral("385"), &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral(" "), &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_FirstOf(atb_StrView_From_StrLiteral("Chocolatine"),
                               atb_StrView_From_StrLiteral(""), &offset));
  EXPECT_EQ(offset, 20);
}

TEST(AtbSpanStringDeathTest, Find_FirstNotOf) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_Find_FirstNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                  K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(atb_StrView_Find_FirstNotOf(
                         K_ATB_ANYSPAN_INVALID,
                         atb_StrView_From_StrLiteral("Chocolatine"), NULL),
                     "IsValid");

  EXPECT_DEBUG_DEATH(atb_StrView_Find_FirstNotOf(K_ATB_ANYSPAN_INVALID,
                                                 K_ATB_ANYSPAN_INVALID, NULL),
                     "IsValid");
}

TEST(AtbSpanStringTest, Find_FirstNotOf) {
  EXPECT_TRUE(
      atb_StrView_Find_FirstNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                  atb_StrView_From_StrLiteral("co"), NULL));

  size_t offset = 0;
  EXPECT_TRUE(atb_StrView_Find_FirstNotOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("alcohC"), &offset));
  EXPECT_EQ(offset, 7);

  offset = 20;
  EXPECT_FALSE(atb_StrView_Find_FirstNotOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("enitalochC"), &offset));
  EXPECT_EQ(offset, 20);

  offset = 1;
  EXPECT_TRUE(atb_StrView_Find_FirstNotOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("580aXV"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 0;
  EXPECT_TRUE(
      atb_StrView_Find_FirstNotOf(atb_StrView_From_StrLiteral("ChotineCho"),
                                  atb_StrView_From_StrLiteral("Cho"), &offset));
  EXPECT_EQ(offset, 3);

  offset = 1;
  EXPECT_TRUE(
      atb_StrView_Find_FirstNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                  atb_StrView_From_StrLiteral("385"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 1;
  EXPECT_TRUE(
      atb_StrView_Find_FirstNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                  atb_StrView_From_StrLiteral(" "), &offset));
  EXPECT_EQ(offset, 0);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_FirstNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                  atb_StrView_From_StrLiteral(""), &offset));
  EXPECT_EQ(offset, 20);
}

TEST(AtbSpanStringDeathTest, Find_LastOf) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("Chocolatine"),
                              K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(
      atb_StrView_Find_LastOf(K_ATB_ANYSPAN_INVALID,
                              atb_StrView_From_StrLiteral("Chocolatine"), NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(atb_StrView_Find_LastOf(K_ATB_ANYSPAN_INVALID,
                                             K_ATB_ANYSPAN_INVALID, NULL),
                     "IsValid");
}

TEST(AtbSpanStringTest, Find_LastOf) {
  EXPECT_TRUE(
      atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("Chocolatine"),
                              atb_StrView_From_StrLiteral("co"), NULL));

  size_t offset = 0;
  EXPECT_TRUE(
      atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("Chocolatine"),
                              atb_StrView_From_StrLiteral("co"), &offset));
  EXPECT_EQ(offset, 4);

  offset = 0;
  EXPECT_TRUE(atb_StrView_Find_LastOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("enitalocohC"), &offset));
  EXPECT_EQ(offset, 10);

  offset = 0;
  EXPECT_TRUE(
      atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("Chocolatine"),
                              atb_StrView_From_StrLiteral("580aXV"), &offset));
  EXPECT_EQ(offset, 6);

  offset = 0;
  EXPECT_TRUE(atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("ChotineCho"),
                                      atb_StrView_From_StrLiteral("C"),
                                      &offset));
  EXPECT_EQ(offset, 7);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("Chocolatine"),
                              atb_StrView_From_StrLiteral("385"), &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("Chocolatine"),
                              atb_StrView_From_StrLiteral(" "), &offset));
  EXPECT_EQ(offset, 20);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_LastOf(atb_StrView_From_StrLiteral("Chocolatine"),
                              atb_StrView_From_StrLiteral(""), &offset));
  EXPECT_EQ(offset, 20);
}

TEST(AtbSpanStringDeathTest, Find_LastNotOf) {
  EXPECT_DEBUG_DEATH(
      atb_StrView_Find_LastNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                 K_ATB_ANYSPAN_INVALID, NULL),
      "IsValid");

  EXPECT_DEBUG_DEATH(atb_StrView_Find_LastNotOf(
                         K_ATB_ANYSPAN_INVALID,
                         atb_StrView_From_StrLiteral("Chocolatine"), NULL),
                     "IsValid");

  EXPECT_DEBUG_DEATH(atb_StrView_Find_LastNotOf(K_ATB_ANYSPAN_INVALID,
                                                K_ATB_ANYSPAN_INVALID, NULL),
                     "IsValid");
}

TEST(AtbSpanStringTest, Find_LastNotOf) {
  EXPECT_TRUE(
      atb_StrView_Find_LastNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                 atb_StrView_From_StrLiteral("co"), NULL));

  size_t offset = 0;
  EXPECT_TRUE(atb_StrView_Find_LastNotOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("alcohC"), &offset));
  EXPECT_EQ(offset, 10);

  offset = 20;
  EXPECT_FALSE(atb_StrView_Find_LastNotOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("enitalochC"), &offset));
  EXPECT_EQ(offset, 20);

  offset = 0;
  EXPECT_TRUE(atb_StrView_Find_LastNotOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("580aXV"), &offset));
  EXPECT_EQ(offset, 10);

  offset = 0;
  EXPECT_TRUE(
      atb_StrView_Find_LastNotOf(atb_StrView_From_StrLiteral("ChotineCho"),
                                 atb_StrView_From_StrLiteral("Cho"), &offset));
  EXPECT_EQ(offset, 6);

  offset = 0;
  EXPECT_TRUE(
      atb_StrView_Find_LastNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                 atb_StrView_From_StrLiteral("385"), &offset));
  EXPECT_EQ(offset, 10);

  offset = 0;
  EXPECT_TRUE(atb_StrView_Find_LastNotOf(
      atb_StrView_From_StrLiteral("Chocolatine"),
      atb_StrView_From_StrLiteral("hoclatine"), &offset));
  EXPECT_EQ(offset, 0);

  offset = 0;
  EXPECT_TRUE(
      atb_StrView_Find_LastNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                 atb_StrView_From_StrLiteral(" "), &offset));
  EXPECT_EQ(offset, 10);

  offset = 20;
  EXPECT_FALSE(
      atb_StrView_Find_LastNotOf(atb_StrView_From_StrLiteral("Chocolatine"),
                                 atb_StrView_From_StrLiteral(""), &offset));
  EXPECT_EQ(offset, 20);
}

} // namespace

} // namespace atb
