#include "atb/Span/Arithmetic.h"

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

using AllNonConstSignedIntSpan_t =
    meta::TypeList<atb_8Span, atb_16Span, atb_32Span, atb_64Span>;

using AllConstSignedIntSpan_t =
    meta::TypeList<atb_Const8Span, atb_Const16Span, atb_Const32Span,
                   atb_Const64Span>;

using AllSignedIntSpan_t =
    meta::Combine_t<AllNonConstSignedIntSpan_t, AllConstSignedIntSpan_t>;

using AllNonConstUnsignedIntSpan_t =
    meta::TypeList<atb_u8Span, atb_u16Span, atb_u32Span, atb_u64Span>;

using AllConstUnsignedIntSpan_t =
    meta::TypeList<atb_Constu8Span, atb_Constu16Span, atb_Constu32Span,
                   atb_Constu64Span>;

using AllUnsignedIntSpan_t =
    meta::Combine_t<AllNonConstUnsignedIntSpan_t, AllConstUnsignedIntSpan_t>;

using AllIntSpan_t = meta::Combine_t<AllSignedIntSpan_t, AllUnsignedIntSpan_t>;

using AllNonConstFloatingSpan_t = meta::TypeList<atb_FltSpan, atb_DblSpan>;

using AllConstFloatingSpan_t =
    meta::TypeList<atb_ConstFltSpan, atb_ConstDblSpan>;

using AllFloatingSpan_t =
    meta::Combine_t<AllNonConstFloatingSpan_t, AllConstFloatingSpan_t>;

using AllArithmeticSpan_t = meta::Combine_t<AllIntSpan_t, AllFloatingSpan_t>;

/////////////////////////////////////////////////////////////////////////////
//                               IsSpan                               //
/////////////////////////////////////////////////////////////////////////////
template <class T>
struct IsArithmeticSpan : Contains<AllArithmeticSpan_t, T> {};
template <class T>
constexpr bool IsArithmeticSpan_v = IsArithmeticSpan<T>::value;

} // namespace meta

template <class T, class..., class ArithmeticType = std::decay_t<T>,
          std::enable_if_t<std::is_arithmetic_v<ArithmeticType>, bool> = true>
constexpr auto ToString(T &&val) {

  if constexpr (std::is_integral_v<ArithmeticType>) {
    constexpr auto hex_fmt = []() {
      if constexpr (sizeof(ArithmeticType) == 1) {
        return "%02X";
      } else if constexpr (sizeof(ArithmeticType) == 2) {
        return "%04X";
      } else if constexpr (sizeof(ArithmeticType) == 4) {
        return "%08X";
      } else if constexpr (sizeof(ArithmeticType) == 8) {
        return "%016lX";
      } else {
        return "%X";
      }
    }();

    std::array<char, (sizeof(ArithmeticType) * 2) + 1> out;
    std::snprintf(out.data(), out.size(), hex_fmt, std::forward<T>(val));

    return out;
  } else {
    std::array<char, (sizeof(ArithmeticType) * 2) + 1> out;

    constexpr auto float_fmt = []() {
      if constexpr (sizeof(ArithmeticType) == 1) {
        return "%02X";
      } else if constexpr (sizeof(ArithmeticType) == 2) {
        return "%04X";
      } else if constexpr (sizeof(ArithmeticType) == 4) {
        return "%08X";
      } else if constexpr (sizeof(ArithmeticType) == 8) {
        return "%016lX";
      } else {
        return "%X";
      }
    }();

    std::snprintf(out.data(), out.size(), hex_fmt, std::forward<T>(val));
  }
}

template <std::size_t N>
constexpr auto ToStringView(const std::array<char, N> &array)
    -> std::string_view {
  return std::string_view(array.data(), array.size() - 1);
}

template <class T, class..., class DecayType = std::decay_t<T>,
          std::enable_if_t<meta::IsArithmeticSpan_v<DecayType>, bool> = true>
std::ostream &PrintTo(std::ostream &os, T &&range,
                      std::optional<size_t> byte_count = std::nullopt) {

  os << "atb_Span{.data=" << static_cast<const void *>(range.data);

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
          std::enable_if_t<meta::IsArithmeticSpan_v<DecayType>, bool> = true>
constexpr std::ostream &operator<<(std::ostream &os, T &&range) {
  return PrintTo(os, range, 20u);
}

template <class AnySpan, class InputIt, class...,
          std::enable_if_t<meta::IsArithmeticSpan_v<std::decay_t<AnySpan>>,
                           bool> = true>
constexpr bool IsEqual(AnySpan &&range, InputIt first, InputIt last) {

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
          std::enable_if_t<meta::IsArithmeticSpan_v<DecayType>, bool> = true>
constexpr bool operator==(T &&lhs, T &&rhs) {
  return ((lhs.data == rhs.data) and (lhs.size == rhs.size));
}

template <class T, class..., class DecayType = std::decay_t<T>,
          std::enable_if_t<meta::IsArithmeticSpan_v<DecayType>, bool> = true>
constexpr bool operator!=(T &&lhs, T &&rhs) {
  return not(lhs == rhs);
}
