#include "atb/allocator/default.h"
#include "gtest/gtest.h"
#include "helper/Error.hpp"
#include "helper/MemSpan.hpp"
namespace {

TEST(AtbAllocatorDefaultTest, AllocFree) {
  atb_Error err;

  auto mem = K_ATB_MEMSPAN_INVALID;

  // malloc
  EXPECT_PRED5(atb_Allocator_Alloc, atb_DefaultAllocator(), mem, 10, &mem, &err)
      << err;

  EXPECT_THAT(mem, testing::Not(helper::FieldsMatch(K_ATB_MEMSPAN_INVALID)));
  EXPECT_EQ(mem.size, 10);

  // Seg fault ?
  atb_MemSpan_Fill(mem, 0xFF);

  auto previous = mem;

  // realloc
  EXPECT_PRED5(atb_Allocator_Alloc, atb_DefaultAllocator(), mem, 20, &mem, &err)
      << err;
  EXPECT_THAT(mem, testing::Not(helper::FieldsMatch(previous)));
  EXPECT_EQ(mem.size, 20);

  // Seg fault ?
  atb_MemSpan_Fill(mem, 0xAB);

  // free
  EXPECT_PRED3(atb_Allocator_Free, atb_DefaultAllocator(), &mem, &err) << err;
  EXPECT_THAT(mem, helper::FieldsMatch(K_ATB_MEMSPAN_INVALID));
}

} // namespace
