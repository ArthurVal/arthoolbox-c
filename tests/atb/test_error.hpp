#pragma once

#include <ostream>

#include "atb/error.h"
#include "utils.hpp"

auto operator<<(std::ostream &os, const atb_Error &err) -> std::ostream &;
auto operator<<(std::ostream &os, const atb_Error *const err) -> std::ostream &;

namespace atb {

struct MockErrorFormatter {
  MOCK_METHOD(size_t, Describe, (atb_ErrorCode_t, char *, size_t, size_t *));

  MockErrorFormatter();

  auto Itf() const -> const atb_ErrorFormatter *;

 private:
  static auto DoDescribe(void *data, atb_ErrorCode_t code, char *d_first,
                         size_t d_size, size_t *written) -> bool;

  const atb_ErrorFormatter m_itf;
};

DefineFieldsMatchFor(atb_Error, 2, category, code);
} // namespace atb
