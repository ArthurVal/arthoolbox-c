#include "atb/allocator.h"
#include "test_error.hpp"
#include "test_memory_span.hpp"

namespace {

struct MockAllocator {
  MOCK_METHOD(bool, Alloc,
              (struct atb_MemSpan orig, size_t size,
               struct atb_MemSpan *const out, struct atb_Error *const err));

  MOCK_METHOD(bool, Free,
              (struct atb_MemSpan mem, struct atb_Error *const err));
};

bool MockAllocator_Alloc(void *mock, struct atb_MemSpan orig, size_t size,
                         struct atb_MemSpan *const out,
                         struct atb_Error *const err) {
  return reinterpret_cast<MockAllocator *>(mock)->Alloc(orig, size, out, err);
}

bool MockAllocator_Free(void *mock, struct atb_MemSpan mem,
                        struct atb_Error *const err) {
  return reinterpret_cast<MockAllocator *>(mock)->Free(mem, err);
}

atb_Allocator MakeAtbAllocatorFrom(MockAllocator &m) {
  return {
      .data = &m,
      .Alloc = MockAllocator_Alloc,
      .Free = MockAllocator_Free,
  };
}

struct AtbAllocatorTest : testing::Test {
  static void SetUpTestSuite() {}
  AtbAllocatorTest()
      : mock_allocator(),
        mock_allocator_itf(MakeAtbAllocatorFrom(mock_allocator)) {}
  void SetUp() override {
    using testing::_;
    using testing::Return;
    ON_CALL(mock_allocator, Alloc(_, _, _, _)).WillByDefault(Return(false));
    ON_CALL(mock_allocator, Free(_, _)).WillByDefault(Return(false));
  }

  void TearDown() override {}
  virtual ~AtbAllocatorTest() {}
  static void TearDownTestSuite() {}

  MockAllocator mock_allocator;
  const atb_Allocator mock_allocator_itf;
};

using AtbAllocatorDeathTest = AtbAllocatorTest;

TEST_F(AtbAllocatorDeathTest, Alloc) {
  atb_MemSpan res;

  ON_CALL(mock_allocator, Alloc(atb::FieldsMatch(K_ATB_MEMSPAN_INVALID), 2,
                                &res, K_ATB_ERROR_IGNORED))
      .WillByDefault(testing::Return(true));

  EXPECT_DEBUG_DEATH(atb_Allocator_Alloc(nullptr, K_ATB_MEMSPAN_INVALID, 2,
                                         &res, K_ATB_ERROR_IGNORED),
                     "self != NULL");

  EXPECT_DEBUG_DEATH(
      atb_Allocator_Alloc(&mock_allocator_itf, K_ATB_MEMSPAN_INVALID, 2,
                          nullptr, K_ATB_ERROR_IGNORED),
      "out != NULL");

  EXPECT_DEBUG_DEATH(
      atb_Allocator_Alloc(&mock_allocator_itf, K_ATB_MEMSPAN_INVALID, 2, &res,
                          K_ATB_ERROR_IGNORED),
      "out->size == size");

  atb_Allocator wrong_alloc;
  wrong_alloc.Alloc = nullptr;
  EXPECT_DEBUG_DEATH(atb_Allocator_Alloc(&wrong_alloc, K_ATB_MEMSPAN_INVALID, 2,
                                         &res, K_ATB_ERROR_IGNORED),
                     "self->Alloc != NULL");
}

TEST_F(AtbAllocatorTest, Alloc) {
  using testing::DoAll;
  using testing::Return;
  using testing::SetArgPointee;

  atb_MemSpan orig = K_ATB_MEMSPAN_INVALID;
  atb_MemSpan res;
  EXPECT_CALL(mock_allocator,
              Alloc(atb::FieldsMatch(orig), 42, &res, K_ATB_ERROR_IGNORED))
      .WillOnce(Return(false))
      .WillOnce(
          DoAll(SetArgPointee<2>(atb_MemSpan{.data = nullptr, .size = 42}),
                Return(true)))
      .RetiresOnSaturation();

  EXPECT_FALSE(atb_Allocator_Alloc(&mock_allocator_itf, orig, 42, &res,
                                   K_ATB_ERROR_IGNORED));
  EXPECT_TRUE(atb_Allocator_Alloc(&mock_allocator_itf, orig, 42, &res,
                                  K_ATB_ERROR_IGNORED));

  atb_Error err;
  EXPECT_CALL(mock_allocator, Alloc(atb::FieldsMatch(orig), 42, &res, &err))
      .WillOnce(
          DoAll(SetArgPointee<2>(atb_MemSpan{.data = nullptr, .size = 42}),
                Return(true)))
      .WillOnce(Return(false))
      .RetiresOnSaturation();

  EXPECT_TRUE(atb_Allocator_Alloc(&mock_allocator_itf, orig, 42, &res, &err));
  EXPECT_FALSE(atb_Allocator_Alloc(&mock_allocator_itf, orig, 42, &res, &err));
}

TEST_F(AtbAllocatorDeathTest, Free) {
  atb_MemSpan mem;

  EXPECT_DEBUG_DEATH(atb_Allocator_Free(nullptr, &mem, K_ATB_ERROR_IGNORED),
                     "self != NULL");

  EXPECT_DEBUG_DEATH(
      atb_Allocator_Free(&mock_allocator_itf, nullptr, K_ATB_ERROR_IGNORED),
      "mem != NULL");

  atb_Allocator wrong_alloc;
  wrong_alloc.Free = nullptr;
  EXPECT_DEBUG_DEATH(
      atb_Allocator_Free(&wrong_alloc, &mem, K_ATB_ERROR_IGNORED),
      "self->Free != NULL");
}

TEST_F(AtbAllocatorTest, Free) {
  using testing::Return;

  atb_Error err;
  auto mem = K_ATB_MEMSPAN_INVALID;

  EXPECT_TRUE(
      atb_Allocator_Free(&mock_allocator_itf, &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, atb::FieldsMatch(K_ATB_MEMSPAN_INVALID));

  EXPECT_TRUE(atb_Allocator_Free(&mock_allocator_itf, &mem, &err));
  EXPECT_THAT(mem, atb::FieldsMatch(K_ATB_MEMSPAN_INVALID));

  int v;
  mem = atb_MemSpan_From_Value(v);

  EXPECT_CALL(mock_allocator, Free(atb::FieldsMatch(mem), K_ATB_ERROR_IGNORED))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();

  EXPECT_FALSE(
      atb_Allocator_Free(&mock_allocator_itf, &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, atb::FieldsMatch(atb_MemSpan_From_Value(v)));

  EXPECT_TRUE(
      atb_Allocator_Free(&mock_allocator_itf, &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, atb::FieldsMatch(K_ATB_MEMSPAN_INVALID));
}

} // namespace
