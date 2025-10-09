#pragma once

#include "atb/allocator.h"
#include "test_error.hpp"
#include "utils.hpp"

namespace atb {

struct MockAllocator {
  MOCK_METHOD(void, Delete, ());

  MOCK_METHOD(void *, Alloc,
              (void *orig, size_t size, struct atb_Error *const err));

  MOCK_METHOD(bool, Release, (void *mem, struct atb_Error *const err));

  MockAllocator();

  auto Itf() const -> const atb_Allocator *;

 private:
  static void DoDelete(void *mock);

  static void *DoAlloc(void *mock, void *orig, size_t size,
                       struct atb_Error *const err);

  static bool DoRelease(void *mock, void *mem, struct atb_Error *const err);

  const atb_Allocator m_itf;
};

} // namespace atb

auto operator<<(std::ostream &os, const atb_Allocator &a) -> std::ostream &;
