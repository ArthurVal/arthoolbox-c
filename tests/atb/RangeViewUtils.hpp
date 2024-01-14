#include "atb/RangeView.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <iterator>
#include <optional>
#include <ostream>
#include <string_view>
#include <type_traits>

template <class T, class..., class IntegerType = std::decay_t<T>,
          std::enable_if_t<std::is_integral_v<IntegerType>, bool> = true>
constexpr auto ToHexString(T &&val)
    -> std::array<char, (sizeof(IntegerType) * 2) + 1> {

  constexpr auto hex_fmt = []() {
    if constexpr (sizeof(IntegerType) == 1) {
      return "%02X";
    } else if constexpr (sizeof(IntegerType) == 2) {
      return "%04X";
    } else if constexpr (sizeof(IntegerType) == 4) {
      return "%08X";
    } else if constexpr (sizeof(IntegerType) == 8) {
      return "%016X";
    } else {
      return "%X";
    }
  }();

  std::array<char, (sizeof(IntegerType) * 2) + 1> out;
  std::snprintf(out.data(), out.size(), hex_fmt, std::forward<T>(val));

  return out;
}

template <std::size_t N>
constexpr auto ToStringView(const std::array<char, N> &array)
    -> std::string_view {
  return std::string_view(array.data(), array.size());
}

namespace meta {

template <class T, class = void> struct IsRangeView : std::false_type {};
template <class T>
struct IsRangeView<T, std::void_t<decltype(std::declval<T>().data),
                                  decltype(std::declval<T>().size)>>
    : std::is_pointer<decltype(std::declval<T>().data)> {};

template <class T> constexpr bool IsRangeView_v = IsRangeView<T>::value;

} // namespace meta

template <class T, class..., class DecayType = std::decay_t<T>,
          std::enable_if_t<meta::IsRangeView_v<DecayType>, bool> = true>
std::ostream &PrintTo(std::ostream &os, T &&range,
                      std::optional<size_t> byte_count = std::nullopt) {

  os << "atb_RangeView{.data=" << static_cast<const void *>(range.data);

  os << ", .size=" << range.size;
  os << " (" << (range.size * sizeof(*range.data)) << " bytes)";

  if (range.data != nullptr and byte_count.has_value()) {
    os << " [Ox";

    auto first = range.data;
    const auto last = first + std::min(range.size, *byte_count);

    while (first != last) {
      os << " " << ToStringView(ToHexString(*first++));
    }

    if (*byte_count < range.size)
      os << " ...";

    os << "]";
  }

  os << "}";

  return os;
}

template <class T, class..., class DecayType = std::decay_t<T>,
          std::enable_if_t<meta::IsRangeView_v<DecayType>, bool> = true>
constexpr std::ostream &operator<<(std::ostream &os, T &&range) {
  return PrintTo(os, range, 20u);
}

template <class AnyRangeView, class InputIt, class...,
          std::enable_if_t<meta::IsRangeView_v<std::decay_t<AnyRangeView>>,
                           bool> = true>
constexpr bool IsEqual(AnyRangeView &&range, InputIt first, InputIt last) {

  const auto other_range_size = std::distance(first, last);
  if (other_range_size < 0)
    return false;

  if (range.size != static_cast<std::size_t>(other_range_size) or
      (range.data == nullptr)) {
    return false;
  }

  return std::equal(first, last, range.data);
}

template <class T, class..., class DecayType = std::decay_t<T>,
          std::enable_if_t<meta::IsRangeView_v<DecayType>, bool> = true>
constexpr bool operator==(T &&lhs, T &&rhs) {
  return ((lhs.data == rhs.data) and (lhs.size == rhs.size));
}

template <class T, class..., class DecayType = std::decay_t<T>,
          std::enable_if_t<meta::IsRangeView_v<DecayType>, bool> = true>
constexpr bool operator!=(T &&lhs, T &&rhs) {
  return not(lhs == rhs);
}
