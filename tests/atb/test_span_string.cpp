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

} // namespace

} // namespace atb
