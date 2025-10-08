#pragma once

#include <ostream>

#include "atb/span.h"
#include "utils.hpp"

namespace atb {

namespace details {
template <class T>
using HasMember_data = decltype(std::declval<T>().data);

template <class T>
using HasMember_size = decltype(std::declval<T>().size);
} // namespace details

template <class T>
struct IsSpan : std::conjunction<HasTrait<details::HasMember_data, T>,
                                 HasTrait<details::HasMember_size, T>> {};

template <class T>
constexpr bool IsSpan_v = IsSpan<T>::value;

namespace details {

template <class T, bool>
struct SpanTraitsImpl;

template <class T>
struct SpanTraitsImpl<T, true> {
  using pointer = decltype(std::declval<T>().data);
  using reference = decltype(*std::declval<pointer>());
  using value_type = std::remove_reference_t<reference>;
};

template <class T>
struct SpanTraitsImpl<T, false> {};

} // namespace details

template <class T>
struct SpanTraits : details::SpanTraitsImpl<T, IsSpan_v<T>> {};

template <class Sink, class T>
auto StreamSpanTo(Sink &sink, T span) -> Sink & {
  static_assert(IsSpan_v<T>);

  sink << '{';
  sink << ".data=" << (void *)span.data << ", ";
  sink << ".size=" << span.size << ", ";
  sink << '}';

  if (span.data == nullptr) {
    sink << " -> /!\\ INVALID";
  } else if (span.size != 0 and
             IsStreamableTo_v<Sink &, decltype(*span.data)>) {
    sink << " -> [";
    auto const *value = span.data;
    atb_AnySpan_ForEach(value, span) { sink << *value << ", "; }
    sink << ']';
  }

  return sink;
}

} // namespace atb

auto operator<<(std::ostream &os,
                const atb_View_Copy_Opt_t &opt) -> std::ostream &;
