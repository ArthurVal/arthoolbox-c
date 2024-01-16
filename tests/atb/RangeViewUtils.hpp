#include "atb/RangeView.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <iterator>
#include <optional>
#include <ostream>
#include <string_view>
#include <type_traits>

namespace meta {

template <class... T> struct TypeList;

/////////////////////////////////////////////////////////////////////////////
//                                   Combine                               //
/////////////////////////////////////////////////////////////////////////////
template <class T, class U> struct Combine;

template <template <class...> class TypeHolder, class... FirstTypes,
          class... SecondTypes>
struct Combine<TypeHolder<FirstTypes...>, TypeHolder<SecondTypes...>> {
  using type = TypeHolder<FirstTypes..., SecondTypes...>;
};

template <template <class...> class TypeHolder, class... Types, class T>
struct Combine<TypeHolder<Types...>, T> {
  using type = TypeHolder<Types..., T>;
};

template <template <class...> class TypeHolder, class... Types, class T>
struct Combine<T, TypeHolder<Types...>> {
  using type = TypeHolder<T, Types...>;
};

template <class T, class U> using Combine_t = typename Combine<T, U>::type;

/////////////////////////////////////////////////////////////////////////////
//                              SwapContainer                              //
/////////////////////////////////////////////////////////////////////////////
template <class Source, template <class...> class Dest> struct SwapContainer;

template <template <class...> class Source, template <class...> class Dest,
          class... Types>
struct SwapContainer<Source<Types...>, Dest> {
  using type = Dest<Types...>;
};

template <class Source, template <class...> class Dest>
using SwapContainer_t = typename SwapContainer<Source, Dest>::type;

/////////////////////////////////////////////////////////////////////////////
//                                 Contains                                //
/////////////////////////////////////////////////////////////////////////////
template <class List, class T> struct Contains : std::false_type {};

template <template <class...> class List, class T, class U, class... Other>
struct Contains<List<U, Other...>, T> : Contains<List<Other...>, T> {};

template <template <class...> class List, class T, class... Other>
struct Contains<List<T, Other...>, T> : std::true_type {};

template <template <class...> class List, class T>
struct Contains<List<>, T> : std::false_type {};

template <class List, class T>
constexpr bool Contains_v = Contains<List, T>::value;

using AllNonConstSignedRangeView_t =
    meta::TypeList<atb_8RangeView, atb_16RangeView, atb_32RangeView,
                   atb_64RangeView>;

using AllConstSignedRangeView_t =
    meta::TypeList<atb_Const8RangeView, atb_Const16RangeView,
                   atb_Const32RangeView, atb_Const64RangeView>;

using AllSignedRangeView_t =
    meta::Combine_t<AllNonConstSignedRangeView_t, AllConstSignedRangeView_t>;

using AllNonConstUnsignedRangeView_t =
    meta::TypeList<atb_u8RangeView, atb_u16RangeView, atb_u32RangeView,
                   atb_u64RangeView>;

using AllConstUnsignedRangeView_t =
    meta::TypeList<atb_Constu8RangeView, atb_Constu16RangeView,
                   atb_Constu32RangeView, atb_Constu64RangeView>;

using AllUnsignedRangeView_t = meta::Combine_t<AllNonConstUnsignedRangeView_t,
                                               AllConstUnsignedRangeView_t>;

using AllIntRangeView_t =
    meta::Combine_t<AllSignedRangeView_t, AllUnsignedRangeView_t>;

using AllRangeView_t = meta::Combine_t<AllIntRangeView_t, atb_CharRangeView>;

/////////////////////////////////////////////////////////////////////////////
//                               IsRangeView                               //
/////////////////////////////////////////////////////////////////////////////
template <class T> struct IsRangeView : Contains<AllRangeView_t, T> {};
template <class T> constexpr bool IsRangeView_v = IsRangeView<T>::value;

} // namespace meta

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
  return std::string_view(array.data(), array.size() - 1);
}

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
