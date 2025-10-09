#include "atb/allocator/default.h"
#include "test_allocator.hpp"

namespace {

TEST(AtbAllocatorDefaultTest, AllocRelease) {
  atb_Error err;

  auto size = 10u;
  char *mem = nullptr;

  // malloc
  mem = reinterpret_cast<char *>(
      atb_Allocator_Alloc(atb_DefaultAllocator(), nullptr, size, &err));
  EXPECT_THAT(mem, testing::Not(nullptr)) << err;

  // Seg fault ?
  std::fill_n(mem, size, 0xFF);

  // realloc
  size = size * 2;
  mem = reinterpret_cast<char *>(
      atb_Allocator_Alloc(atb_DefaultAllocator(), mem, size, &err));
  EXPECT_THAT(mem, testing::Not(nullptr)) << err;

  // Seg fault ?
  std::fill_n(mem, size, 0xAB);

  // free
  EXPECT_TRUE(atb_Allocator_Release(atb_DefaultAllocator(),
                                    reinterpret_cast<void **>(&mem), &err))
      << err;
  EXPECT_THAT(mem, nullptr);
}

} // namespace
