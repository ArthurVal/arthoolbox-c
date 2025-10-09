#include "test_allocator.hpp"

namespace atb {
namespace {

struct AtbAllocatorTest : testing::Test {
  static void SetUpTestSuite() {}
  AtbAllocatorTest() : mock() {}
  void SetUp() override {
    using testing::_;
    using testing::Return;
    ON_CALL(mock, Delete()).WillByDefault(Return());
    ON_CALL(mock, Alloc(_, _, _)).WillByDefault(Return(nullptr));
    ON_CALL(mock, Release(_, _)).WillByDefault(Return(false));
  }

  void TearDown() override {}
  virtual ~AtbAllocatorTest() {}
  static void TearDownTestSuite() {}

  MockAllocator mock;
};

using AtbAllocatorDeathTest = AtbAllocatorTest;

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
  ON_CALL(mock, Alloc(nullptr, 2, K_ATB_ERROR_IGNORED))
      .WillByDefault(testing::Return(reinterpret_cast<void *>(0x42)));

  EXPECT_DEBUG_DEATH(
      atb_Allocator_Alloc(nullptr, nullptr, 2, K_ATB_ERROR_IGNORED),
      "self != NULL");

  atb_Allocator wrong_alloc;
  wrong_alloc.Alloc = nullptr;
  EXPECT_DEBUG_DEATH(
      atb_Allocator_Alloc(&wrong_alloc, nullptr, 2, K_ATB_ERROR_IGNORED),
      "self->Alloc != NULL");
}

TEST_F(AtbAllocatorTest, Alloc) {
  using testing::Return;

  void *orig = nullptr;
  EXPECT_CALL(mock, Alloc(orig, 42, K_ATB_ERROR_IGNORED))
      .WillOnce(Return(nullptr))
      .WillOnce(Return(reinterpret_cast<void *>(0x42)))
      .RetiresOnSaturation();

  EXPECT_EQ(atb_Allocator_Alloc(mock.Itf(), orig, 42, K_ATB_ERROR_IGNORED),
            nullptr);

  EXPECT_EQ(atb_Allocator_Alloc(mock.Itf(), orig, 42, K_ATB_ERROR_IGNORED),
            reinterpret_cast<void *>(0x42));

  atb_Error err;
  EXPECT_CALL(mock, Alloc(orig, 42, &err))
      .WillOnce(Return(nullptr))
      .WillOnce(Return(reinterpret_cast<void *>(0x43)))
      .RetiresOnSaturation();

  EXPECT_EQ(atb_Allocator_Alloc(mock.Itf(), orig, 42, &err), nullptr);
  EXPECT_EQ(atb_Allocator_Alloc(mock.Itf(), orig, 42, &err),
            reinterpret_cast<void *>(0x43));
}

TEST_F(AtbAllocatorDeathTest, Release) {
  int v;
  void *mem = &v;

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
  void *mem = nullptr;

  EXPECT_TRUE(atb_Allocator_Release(mock.Itf(), &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, nullptr);

  EXPECT_TRUE(atb_Allocator_Release(mock.Itf(), &mem, &err));
  EXPECT_THAT(mem, nullptr);

  int v;
  mem = &v;

  EXPECT_CALL(mock, Release(&v, K_ATB_ERROR_IGNORED))
      .WillOnce(Return(false))
      .WillOnce(Return(true))
      .RetiresOnSaturation();

  mem = &v;
  EXPECT_FALSE(atb_Allocator_Release(mock.Itf(), &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, &v);

  EXPECT_TRUE(atb_Allocator_Release(mock.Itf(), &mem, K_ATB_ERROR_IGNORED));
  EXPECT_THAT(mem, nullptr);
}

} // namespace

MockAllocator::MockAllocator()
    : m_itf(atb_Allocator{
          .data = this,
          .Delete = MockAllocator::DoDelete,
          .Alloc = MockAllocator::DoAlloc,
          .Release = MockAllocator::DoRelease,
      }) {}

auto MockAllocator::Itf() const -> const atb_Allocator * { return &(m_itf); }

void MockAllocator::DoDelete(void *mock) {
  return reinterpret_cast<MockAllocator *>(mock)->Delete();
}

void *MockAllocator::DoAlloc(void *mock, void *orig, size_t size,
                             struct atb_Error *const err) {
  return reinterpret_cast<MockAllocator *>(mock)->Alloc(orig, size, err);
}

bool MockAllocator::DoRelease(void *mock, void *const mem,
                              struct atb_Error *const err) {
  return reinterpret_cast<MockAllocator *>(mock)->Release(mem, err);
}

} // namespace atb

auto operator<<(std::ostream &os, const atb_Allocator &a) -> std::ostream & {
  os << "atb_Allocator{";
  os << ".data=" << a.data << ", ";
  os << ".Delete=" << (void *)a.Delete << ", ";
  os << ".Alloc=" << (void *)a.Alloc << ", ";
  os << ".Release=" << (void *)a.Release << ", ";
  os << '}';
  return os;
}
