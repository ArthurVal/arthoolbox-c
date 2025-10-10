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

} // namespace

} // namespace atb
