#pragma once

#include "atb/allocator.h"
#include "test_error.hpp"
#include "test_memory_span.hpp"
#include "utils.hpp"

namespace atb {

struct MockAllocator {
  MOCK_METHOD(bool, Alloc,
              (struct atb_MemSpan orig, size_t size,
               struct atb_MemSpan *const out, struct atb_Error *const err));

  MOCK_METHOD(bool, Free,
              (struct atb_MemSpan mem, struct atb_Error *const err));

  MockAllocator();

  auto Itf() const -> const atb_Allocator *;

 private:
  static bool DoAlloc(void *mock, struct atb_MemSpan orig, size_t size,
                      struct atb_MemSpan *const out,
                      struct atb_Error *const err);

  static bool DoFree(void *mock, struct atb_MemSpan mem,
                     struct atb_Error *const err);

  const atb_Allocator m_itf;
};

} // namespace atb

auto operator<<(std::ostream &os, const atb_Allocator &a) -> std::ostream &;
