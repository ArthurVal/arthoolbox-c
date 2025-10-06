#include "test_allocator.hpp"

namespace atb {
namespace {

struct AtbAllocatorTest : testing::Test {
  static void SetUpTestSuite() {}
  AtbAllocatorTest() : mock() {}
  void SetUp() override {
    using testing::_;
    using testing::Return;
    ON_CALL(mock, Init(_, _)).WillByDefault(Return(true));
    ON_CALL(mock, Delete()).WillByDefault(Return());
    ON_CALL(mock, Alloc(_, _, _, _)).WillByDefault(Return(false));
    ON_CALL(mock, Release(_, _)).WillByDefault(Return(false));
  }

  void TearDown() override {}
  virtual ~AtbAllocatorTest() {}
  static void TearDownTestSuite() {}

  MockAllocator mock;
};

using AtbAllocatorDeathTest = AtbAllocatorTest;

TEST_F(AtbAllocatorDeathTest, Init) {
  int param;

  EXPECT_DEBUG_DEATH(atb_Allocator_Init(nullptr, &param, K_ATB_ERROR_IGNORED),
                     "self != NULL");

  atb_Allocator wrong_alloc = *mock.Itf();

  wrong_alloc.Alloc = nullptr;
  EXPECT_DEBUG_DEATH(
      atb_Allocator_Init(&wrong_alloc, &param, K_ATB_ERROR_IGNORED),
      "self->Alloc != NULL");

  wrong_alloc = *mock.Itf();

  wrong_alloc.Release = nullptr;
  EXPECT_DEBUG_DEATH(
      atb_Allocator_Init(&wrong_alloc, &param, K_ATB_ERROR_IGNORED),
      "self->Release != NULL");

  wrong_alloc.Alloc = nullptr;
  EXPECT_DEBUG_DEATH(
      atb_Allocator_Init(&wrong_alloc, &param, K_ATB_ERROR_IGNORED),
      "self->Alloc != NULL");
}

TEST_F(AtbAllocatorTest, Init) {
  using testing::Return;

  int param;

  EXPECT_CALL(mock, Init(&param, K_ATB_ERROR_IGNORED))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();
  EXPECT_NPRED3(atb_Allocator_Init, mock.Itf(), &param, K_ATB_ERROR_IGNORED);
  EXPECT_PRED3(atb_Allocator_Init, mock.Itf(), &param, K_ATB_ERROR_IGNORED);

  atb_Error err;
  EXPECT_CALL(mock, Init(&param, &err))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .RetiresOnSaturation();
  EXPECT_PRED3(atb_Allocator_Init, mock.Itf(), &param, &err);
  EXPECT_NPRED3(atb_Allocator_Init, mock.Itf(), &param, &err);

  auto allocator_without_init = *mock.Itf();
  allocator_without_init.Init = nullptr;
  EXPECT_PRED3(atb_Allocator_Init, &allocator_without_init, &param,
               K_ATB_ERROR_IGNORED);
  EXPECT_PRED3(atb_Allocator_Init, &allocator_without_init, &param, &err);
}

TEST_F(AtbAllocatorDeathTest, Delete) {
  EXPECT_DEBUG_DEATH(atb_Allocator_Delete(nullptr), "self != NULL");
}

TEST_F(AtbAllocatorTest, Delete) {
  using testing::Return;

  EXPECT_CALL(mock, Delete()).WillOnce(Return()).RetiresOnSaturation();
  atb_Allocator_Delete(mock.Itf());

  auto allocator_without_delete = *mock.Itf();
  allocator_without_delete.Delete = nullptr;
  atb_Allocator_Delete(&allocator_without_delete);
}

TEST_F(AtbAllocatorDeathTest, Alloc) {
  atb_MemSpan res;

  ON_CALL(mock, Alloc(FieldsMatch(K_ATB_MEMSPAN_INVALID), 2, &res,
                      K_ATB_ERROR_IGNORED))
      .WillByDefault(testing::Return(true));

  EXPECT_DEBUG_DEATH(atb_Allocator_Alloc(nullptr, K_ATB_MEMSPAN_INVALID, 2,
                                         &res, K_ATB_ERROR_IGNORED),
                     "self != NULL");

  EXPECT_DEBUG_DEATH(atb_Allocator_Alloc(mock.Itf(), K_ATB_MEMSPAN_INVALID, 2,
                                         nullptr, K_ATB_ERROR_IGNORED),
                     "out != NULL");

  EXPECT_DEBUG_DEATH(atb_Allocator_Alloc(mock.Itf(), K_ATB_MEMSPAN_INVALID, 2,
                                         &res, K_ATB_ERROR_IGNORED),
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
  EXPECT_CALL(mock, Alloc(FieldsMatch(orig), 42, &res, K_ATB_ERROR_IGNORED))
      .WillOnce(Return(false))
      .WillOnce(
          DoAll(SetArgPointee<2>(atb_MemSpan{.data = nullptr, .size = 42}),
                Return(true)))
      .RetiresOnSaturation();

  EXPECT_FALSE(
      atb_Allocator_Alloc(mock.Itf(), orig, 42, &res, K_ATB_ERROR_IGNORED));
  EXPECT_TRUE(
      atb_Allocator_Alloc(mock.Itf(), orig, 42, &res, K_ATB_ERROR_IGNORED));

  atb_Error err;
  EXPECT_CALL(mock, Alloc(FieldsMatch(orig), 42, &res, &err))
      .WillOnce(
          DoAll(SetArgPointee<2>(atb_MemSpan{.data = nullptr, .size = 42}),
                Return(true)))
      .WillOnce(Return(false))
      .RetiresOnSaturation();

  EXPECT_TRUE(atb_Allocator_Alloc(mock.Itf(), orig, 42, &res, &err));
  EXPECT_FALSE(atb_Allocator_Alloc(mock.Itf(), orig, 42, &res, &err));
}

TEST_F(AtbAllocatorDeathTest, Release) {
  atb_MemSpan mem;

  EXPECT_DEBUG_DEATH(atb_Allocator_Release(nullptr, &mem, K_ATB_ERROR_IGNORED),
                     "self != NULL");

  EXPECT_DEBUG_DEATH(
      atb_Allocator_Release(mock.Itf(), nullptr, K_ATB_ERROR_IGNORED),
      "mem != NULL");

  atb_Allocator wrong_alloc;
  wrong_alloc.Release = nullptr;
  EXPECT_DEBUG_DEATH(
      atb_Allocator_Release(&wrong_alloc, &mem, K_ATB_ERROR_IGNORED),
      "self->Release != NULL");
}

TEST_F(AtbAllocatorTest, Release) {
  using testing::Return;

  atb_Error err;
  auto mem = K_ATB_MEMSPAN_INVALID;

  EXPECT_TRUE(atb_Allocator_Release(mock.Itf(), &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, FieldsMatch(K_ATB_MEMSPAN_INVALID));

  EXPECT_TRUE(atb_Allocator_Release(mock.Itf(), &mem, &err));
  EXPECT_THAT(mem, FieldsMatch(K_ATB_MEMSPAN_INVALID));

  int v;
  mem = atb_MemSpan_From_Value(v);

  EXPECT_CALL(mock, Release(FieldsMatch(mem), K_ATB_ERROR_IGNORED))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();

  EXPECT_FALSE(atb_Allocator_Release(mock.Itf(), &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, FieldsMatch(atb_MemSpan_From_Value(v)));

  EXPECT_TRUE(atb_Allocator_Release(mock.Itf(), &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, FieldsMatch(K_ATB_MEMSPAN_INVALID));
}

} // namespace

MockAllocator::MockAllocator()
    : m_itf(atb_Allocator{
          .data = this,
          .Init = MockAllocator::DoInit,
          .Delete = MockAllocator::DoDelete,
          .Alloc = MockAllocator::DoAlloc,
          .Release = MockAllocator::DoRelease,
      }) {}

auto MockAllocator::Itf() const -> const atb_Allocator * { return &(m_itf); }

bool MockAllocator::DoInit(void *mock, void *param,
                           struct atb_Error *const err) {
  return reinterpret_cast<MockAllocator *>(mock)->Init(param, err);
}

void MockAllocator::DoDelete(void *mock) {
  return reinterpret_cast<MockAllocator *>(mock)->Delete();
}

bool MockAllocator::DoAlloc(void *mock, struct atb_MemSpan orig, size_t size,
                            struct atb_MemSpan *const out,
                            struct atb_Error *const err) {
  return reinterpret_cast<MockAllocator *>(mock)->Alloc(orig, size, out, err);
}

bool MockAllocator::DoRelease(void *mock, struct atb_MemSpan mem,
                              struct atb_Error *const err) {
  return reinterpret_cast<MockAllocator *>(mock)->Release(mem, err);
}

} // namespace atb

auto operator<<(std::ostream &os, const atb_Allocator &a) -> std::ostream & {
  os << "atb_Allocator{";
  os << ".data=" << a.data << ", ";
  os << ".Init=" << (void *)a.Init << ", ";
  os << ".Delete=" << (void *)a.Delete << ", ";
  os << ".Alloc=" << (void *)a.Alloc << ", ";
  os << ".Release=" << (void *)a.Release << ", ";
  os << '}';
  return os;
}
