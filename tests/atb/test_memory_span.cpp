#include "test_memory_span.hpp"

#include "atb/memory/format.h"

namespace {

TEST(AtbMemorySpanTest, From) {
  atb_MemSpan span;
  {
    char str[] = "Coucou";

    span = atb_MemSpan_From(str, 1);
    EXPECT_EQ(span.data, str);
    EXPECT_EQ(span.size, 1);

    span = atb_MemSpan_From_Array(str);
    EXPECT_EQ(span.data, str);
    EXPECT_EQ(span.size, std::size(str));
  }

  {
    uint64_t v;
    span = atb_MemSpan_From_Value(v);
    EXPECT_EQ(span.data, &v);
    EXPECT_EQ(span.size, 8);

    uint32_t *v_p_32 = reinterpret_cast<uint32_t *>(&v);
    span = atb_MemSpan_From_Ptr(v_p_32);
    EXPECT_EQ(span.data, &v);
    EXPECT_EQ(span.size, 4);

    uint16_t *v_p_16 = reinterpret_cast<uint16_t *>(&v);
    span = atb_MemSpan_From_Ptr(v_p_16);
    EXPECT_EQ(span.data, &v);
    EXPECT_EQ(span.size, 2);

    uint8_t *v_p_8 = reinterpret_cast<uint8_t *>(&v);
    span = atb_MemSpan_From_Ptr(v_p_8);
    EXPECT_EQ(span.data, &v);
    EXPECT_EQ(span.size, 1);
  }
}

TEST(AtbMemorySpanTest, Format) {
  using atb::MakeStringFromFmt;

  char str[] = "Coucou";
  auto span = atb_MemSpan_From(str, std::size(str));

  std::stringstream ss;
  ss << "{.data=" << (void *)str << ", .size=" << std::size(str) << '}';

  EXPECT_EQ(MakeStringFromFmt(K_ATB_FMT_MEM, ATB_FMT_VA_ARG_MEM(span)),
            ss.str());
}

TEST(AtbMemorySpanTest, IsValid) {
  atb_MemSpan span = K_ATB_MEMSPAN_INVALID;
  EXPECT_NPRED1(atb_MemSpan_IsValid, span);

  char str[] = "Coucou";
  span = atb_MemSpan_From(str, std::size(str));
  EXPECT_PRED1(atb_MemSpan_IsValid, span);
}

TEST(AtbMemorySpanDeathTest, Slice) {
  EXPECT_DEBUG_DEATH(atb_MemSpan_Slice(K_ATB_MEMSPAN_INVALID, 0, 0),
                     "atb_MemSpan_IsValid\\(span\\)");
}

TEST(AtbMemorySpanTest, Slice) {
  using atb::FieldsMatch;

  char str[] = "Coucou";

  auto span = atb_MemSpan_From_Array(str);

  // No op
  EXPECT_THAT(atb_MemSpan_Slice(span, 0, span.size), FieldsMatch(span));

  EXPECT_THAT(atb_MemSpan_Slice(span, 1, 1), FieldsMatch({
                                                 .data = (char *)span.data + 1,
                                                 .size = 1,
                                             }));

  EXPECT_THAT(atb_MemSpan_Slice(span, 3, 1), FieldsMatch({
                                                 .data = (char *)span.data + 3,
                                                 .size = 1,
                                             }));

  EXPECT_THAT(atb_MemSpan_Slice(span, 3, 3), FieldsMatch({
                                                 .data = (char *)span.data + 3,
                                                 .size = 3,
                                             }));

  EXPECT_THAT(atb_MemSpan_Slice(span, 1, 3), FieldsMatch({
                                                 .data = (char *)span.data + 1,
                                                 .size = 3,
                                             }));

  EXPECT_THAT(atb_MemSpan_Slice(span, 4, span.size + 20),
              FieldsMatch({
                  .data = (char *)span.data + 4,
                  .size = span.size - 4,
              }));

  EXPECT_THAT(atb_MemSpan_Slice(span, span.size, 0),
              FieldsMatch({
                  .data = (char *)span.data + span.size,
                  .size = 0,
              }));

  EXPECT_THAT(atb_MemSpan_Slice(span, span.size, 20),
              FieldsMatch({
                  .data = (char *)span.data + span.size,
                  .size = 0,
              }));
}

TEST(AtbMemorySpanDeathTest, ShrinkFront) {
  EXPECT_DEBUG_DEATH(atb_MemSpan_ShrinkFront(K_ATB_MEMSPAN_INVALID, 0),
                     "atb_MemSpan_IsValid\\(span\\)");
}

TEST(AtbMemorySpanTest, ShrinkFront) {
  using atb::FieldsMatch;

  char str[] = "Coucou";

  auto span = atb_MemSpan_From_Array(str);

  // No op
  EXPECT_THAT(atb_MemSpan_ShrinkFront(span, 0), FieldsMatch(span));

  EXPECT_THAT(atb_MemSpan_ShrinkFront(span, 2),
              FieldsMatch({
                  .data = (char *)span.data + 2,
                  .size = span.size - 2,
              }));

  EXPECT_THAT(atb_MemSpan_ShrinkFront(span, span.size),
              FieldsMatch({
                  .data = (char *)span.data + span.size,
                  .size = 0,
              }));

  EXPECT_THAT(atb_MemSpan_ShrinkFront(span, span.size + 20),
              FieldsMatch({
                  .data = (char *)span.data + span.size,
                  .size = 0,
              }));
}

TEST(AtbMemorySpanDeathTest, ShrinkBack) {
  EXPECT_DEBUG_DEATH(atb_MemSpan_ShrinkBack(K_ATB_MEMSPAN_INVALID, 0),
                     "atb_MemSpan_IsValid\\(span\\)");
}

TEST(AtbMemorySpanTest, ShrinkBack) {
  using atb::FieldsMatch;

  char str[] = "Coucou";

  auto span = atb_MemSpan_From_Array(str);

  // No op
  EXPECT_THAT(atb_MemSpan_ShrinkBack(span, 0), FieldsMatch(span));

  EXPECT_THAT(atb_MemSpan_ShrinkBack(span, 2), FieldsMatch({
                                                   .data = span.data,
                                                   .size = span.size - 2,
                                               }));

  EXPECT_THAT(atb_MemSpan_ShrinkBack(span, span.size), FieldsMatch({
                                                           .data = span.data,
                                                           .size = 0,
                                                       }));

  EXPECT_THAT(atb_MemSpan_ShrinkBack(span, span.size + 20),
              FieldsMatch({
                  .data = span.data,
                  .size = 0,
              }));
}

TEST(AtbMemorySpanDeathTest, Fill) {
  EXPECT_DEBUG_DEATH(atb_MemSpan_Fill(K_ATB_MEMSPAN_INVALID, 0),
                     "atb_MemSpan_IsValid\\(span\\)");
}

struct Dummy {
  int32_t i32;
  uint64_t u64;
  char str[20];
};

TEST(AtbMemorySpanTest, Fill) {
  auto u64 = ~((uint64_t)0);
  EXPECT_NE(u64, 0);

  atb_MemSpan_Fill(atb_MemSpan_From_Value(u64), 0x00);
  EXPECT_EQ(u64, 0);

  uint64_t u64_array[200];
  std::fill(std::begin(u64_array), std::end(u64_array), ~((uint64_t)0));
  EXPECT_THAT(u64_array, testing::Not(testing::Each(0)));

  atb_MemSpan_Fill(atb_MemSpan_From_Array(u64_array), 0x00);
  EXPECT_THAT(u64_array, testing::Each(0));

  Dummy d;
  d.i32 = -15;
  d.u64 = 3141592;
  std::fill(std::begin(d.str), std::end(d.str), 'A');

  atb_MemSpan_Fill(atb_MemSpan_From_Value(d), 0x00);
  EXPECT_EQ(d.i32, 0);
  EXPECT_EQ(d.u64, 0);
  EXPECT_THAT(d.str, testing::Each(0x00));
}

} // namespace

std::ostream &operator<<(std::ostream &os, atb_MemSpan mem) {
  os << atb::MakeStringFromFmt(K_ATB_FMT_MEM, ATB_FMT_VA_ARG_MEM(mem));

  if (mem.data == nullptr) {
    os << " -> /!\\ INVALID";
  } else if (mem.size != 0) {
    const uint8_t *byte = (const uint8_t *)mem.data;
    const uint8_t *const end = (byte + mem.size);

    os << " -> 0x";

    const auto flags = os.flags();

    os << std::hex;
    while (byte != end) os << " " << (int)(*byte++);

    os.flags(flags);
  }

  return os;
}
