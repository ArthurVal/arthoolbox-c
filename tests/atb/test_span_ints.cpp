#include "test_span_ints.hpp"

#define _DEFINE_STREAMS(_, NAME, ...)                                       \
  auto operator<<(std::ostream &os, atb_Span_##NAME span)->std::ostream & { \
    os << "Span_" #NAME;                                                    \
    atb::StreamSpanTo(os, span);                                        \
    return os;                                                              \
  }                                                                         \
                                                                            \
  auto operator<<(std::ostream &os, atb_View_##NAME view)->std::ostream & { \
    os << "View_" #NAME;                                                    \
    atb::StreamSpanTo(os, view);                                        \
    return os;                                                              \
  }

ATB_INTS_X_FOREACH(_DEFINE_STREAMS)

#undef _DEFINE_STREAMS

namespace atb {

namespace {

TEST(AtbSpanIntsTest, Convert) {
  std::uint64_t arr_u64[] = {1, 2, 3, 4};
  atb_Span_u64 span_u64 = atb_AnySpan_From_Array(arr_u64);

  {
    auto span = atb_Span_u8_From_u64(span_u64);
    EXPECT_EQ((void *)span.data, (void *)span_u64.data);
    EXPECT_EQ(span.size, span_u64.size * 8);
  }

  {
    auto span = atb_Span_u16_From_u64(span_u64);
    EXPECT_EQ((void *)span.data, (void *)span_u64.data);
    EXPECT_EQ(span.size, span_u64.size * 4);
  }

  {
    auto span = atb_Span_u32_From_u64(span_u64);
    EXPECT_EQ((void *)span.data, (void *)span_u64.data);
    EXPECT_EQ(span.size, span_u64.size * 2);
  }

  std::uint8_t arr_u8[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  atb_Span_u8 span_u8 = atb_AnySpan_From_Array(arr_u8);
  {
    auto span = atb_Span_u16_From_u8(span_u8);
    EXPECT_EQ((void *)span.data, (void *)span_u8.data);
    EXPECT_EQ(span.size, span_u8.size / 2);
  }

  {
    auto span = atb_Span_u32_From_u8(span_u8);
    EXPECT_EQ((void *)span.data, (void *)span_u8.data);
    EXPECT_EQ(span.size, span_u8.size / 4);
  }

  {
    auto span = atb_Span_u64_From_u8(span_u8);
    EXPECT_EQ((void *)span.data, (void *)span_u8.data);
    EXPECT_EQ(span.size, span_u8.size / 8);
  }

  span_u8.size = 1;
  {
    auto span = atb_Span_u16_From_u8(span_u8);
    EXPECT_EQ((void *)span.data, (void *)span_u8.data);
    EXPECT_EQ(span.size, 0);
  }

  {
    auto span = atb_Span_u32_From_u8(span_u8);
    EXPECT_EQ((void *)span.data, (void *)span_u8.data);
    EXPECT_EQ(span.size, 0);
  }

  {
    auto span = atb_Span_u64_From_u8(span_u8);
    EXPECT_EQ((void *)span.data, (void *)span_u8.data);
    EXPECT_EQ(span.size, 0);
  }
}

} // namespace

} // namespace atb
