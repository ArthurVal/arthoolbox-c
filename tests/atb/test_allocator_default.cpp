#include "atb/allocator/default.h"
#include "gtest/gtest.h"
#include "test_error.hpp"
#include "test_memory_span.hpp"

namespace {

TEST(AtbAllocatorDefaultTest, AllocRelease) {
  atb_Error err;

  auto mem = K_ATB_MEMSPAN_INVALID;

  // malloc
  EXPECT_PRED5(atb_Allocator_Alloc, atb_DefaultAllocator(), mem, 10, &mem, &err)
      << err;

  EXPECT_THAT(mem, testing::Not(atb::FieldsMatch(K_ATB_MEMSPAN_INVALID)));
  EXPECT_EQ(mem.size, 10);

  // Seg fault ?
  atb_MemSpan_Fill(mem, 0xFF);

  auto previous = mem;

  // realloc
  EXPECT_PRED5(atb_Allocator_Alloc, atb_DefaultAllocator(), mem, 20, &mem, &err)
      << err;
  EXPECT_THAT(mem, testing::Not(atb::FieldsMatch(previous)));
  EXPECT_EQ(mem.size, 20);

  // Seg fault ?
  atb_MemSpan_Fill(mem, 0xAB);

  // free
  EXPECT_PRED3(atb_Allocator_Release, atb_DefaultAllocator(), &mem, &err)
      << err;
  EXPECT_THAT(mem, atb::FieldsMatch(K_ATB_MEMSPAN_INVALID));
}

} // namespace
