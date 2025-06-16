#include <array>
#include <cassert>
#include <limits>
#include <random>
#include <utility>
#include <variant>

#include "atb/arithmetic.h"
#include "gtest/gtest.h"

#define FWD(v) std::forward<decltype(v)>(v)

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define CONCAT_3(a, b, c) CONCAT(a, CONCAT(b, c))

// I'm sooo lazy ...
#define FOR_(count, var) for (auto var = 0; var < (count); ++var)
#define REPEAT(count) FOR_(count, CONCAT_3(_unused_, __LINE__, _))

namespace {

/// Implementation of the overloads pattern
///(see https://en.cppreference.com/w/cpp/utility/variant/visit)
template <typename... Matchers>
struct Overload : Matchers... {
  using Matchers::operator()...;
};

/// CTAD (Not needed as of c++20)
template <typename... Matchers>
Overload(Matchers...) -> Overload<Matchers...>;

template <class Pred>
constexpr auto Not(Pred &&pred) noexcept {
  return [&](auto &&...v) -> bool { return !std::invoke(pred, FWD(v)...); };
}

template <class T>
struct Excluding {
  constexpr Excluding() = default;
  constexpr explicit Excluding(T v) : value(v) {}

  T value;
};

template <class T>
using bound_t = std::variant<T, Excluding<T>>;

template <class T>
struct Interval {
  static_assert(std::is_arithmetic_v<T>);

  constexpr Interval()
      : Interval(std::numeric_limits<T>::min(), std::numeric_limits<T>::max()) {
  }

  constexpr Interval(bound_t<T> min, bound_t<T> max)
      : m_min(std::visit(Overload{
                             [](T v) -> T { return v; },
                             [](Excluding<T> v) -> T {
                               if constexpr (std::is_integral_v<T>) {
                                 return v.value + 1;
                               } else {
                                 return v.value +
                                        std::numeric_limits<T>::epsilon();
                               }
                             },
                         },
                         min)),
        m_max(std::visit(Overload{
                             [](T v) -> T { return v; },
                             [](Excluding<T> v) -> T {
                               if constexpr (std::is_integral_v<T>) {
                                 return v.value - 1;
                               } else {
                                 return v.value -
                                        std::numeric_limits<T>::epsilon();
                               }
                             },
                         },
                         max)) {
    assert(Min() <= Max());
  }

  constexpr auto Min() const -> T { return m_min; }
  constexpr auto Max() const -> T { return m_max; }

 private:
  T m_min;
  T m_max;
};

template <class T>
constexpr auto operator<<(std::ostream &os,
                          const Interval<T> &i) -> std::ostream & {
  if constexpr (sizeof(T) == 1) {
    return os << "[" << +i.Min() << ", " << +i.Max() << "]";
  } else {
    return os << "[" << i.Min() << ", " << i.Max() << "]";
  }
}

template <class T>
constexpr auto ComplementsOf(Interval<T> interval)
    -> std::array<std::optional<Interval<T>>, 2> {
  std::array<std::optional<Interval<T>>, 2> complement = {
      std::nullopt,
      std::nullopt,
  };

  auto &[complement_lower_range, complement_upper_range] = complement;

  if (interval.Min() != std::numeric_limits<T>::min()) {
    complement_lower_range = Interval<T>{
        std::numeric_limits<T>::min(),
        Excluding<T>(interval.Min()),
    };
  }

  if (interval.Max() != std::numeric_limits<T>::max()) {
    complement_upper_range = Interval<T>{
        Excluding<T>(interval.Max()),
        std::numeric_limits<T>::max(),
    };
  }

  return complement;
}

template <class T>
constexpr auto AreOverlaping(Interval<T> interval_1,
                             Interval<T> interval_2) -> bool {
  const auto &smallest =
      interval_1.Min() < interval_2.Min() ? interval_1 : interval_2;

  const auto &highest =
      interval_1.Max() > interval_2.Max() ? interval_1 : interval_2;

  return smallest.Max() >= highest.Min();
}

template <class T>
struct AddingTo {
  static_assert(std::is_arithmetic_v<T>);

  constexpr AddingTo() = default;
  constexpr explicit AddingTo(T v) : value(v) {}

  T value;
};

template <class T>
constexpr auto IntervalThatOverflows(AddingTo<T> lhs)
    -> std::optional<Interval<T>> {
  // Cannot overflows when adding to negative
  if (lhs.value <= static_cast<T>(0)) return std::nullopt;

  // Overflows when rhs > (max - lhs)
  return Interval<T>{
      Excluding<T>(std::numeric_limits<T>::max() - lhs.value),
      std::numeric_limits<T>::max(),
  };
}

template <class T>
constexpr auto IntervalThatUnderflows(AddingTo<T> lhs)
    -> std::optional<Interval<T>> {
  if constexpr (std::is_unsigned_v<T>) {
    // Additions to unsigned will never underflows
    return std::nullopt;
  } else {
    // Signed ints

    // Cannot underflows when adding to positive
    if (lhs.value >= static_cast<T>(0)) return std::nullopt;

    // Underflows when rhs < (min - lhs)
    return Interval<T>{
        std::numeric_limits<T>::min(),
        Excluding<T>(std::numeric_limits<T>::min() - lhs.value),
    };
  }
}

template <class T>
struct SubstractingTo {
  static_assert(std::is_arithmetic_v<T>);

  constexpr SubstractingTo() = default;
  constexpr explicit SubstractingTo(T v) : value(v) {}

  T value;
};

template <class T>
constexpr auto IntervalThatOverflows(SubstractingTo<T> lhs)
    -> std::optional<Interval<T>> {
  if constexpr (std::is_unsigned_v<T>) {
    // Substraction to unsigned will never overflows
    return std::nullopt;
  } else {
    // Signed ints

    // Substracting to negative can't overflows
    if (lhs.value < static_cast<T>(0)) return std::nullopt;

    // Overflows when rhs < (lhs - max)
    return Interval<T>{
        std::numeric_limits<T>::min(),
        Excluding<T>(lhs.value - std::numeric_limits<T>::max()),
    };
  }
}

template <class T>
constexpr auto IntervalThatUnderflows(SubstractingTo<T> lhs)
    -> std::optional<Interval<T>> {
  if constexpr (std::is_unsigned_v<T>) {
    // When unsigned, cannot underflow when equals to max
    if (lhs.value == std::numeric_limits<T>::max()) return std::nullopt;

    // Any value ABOVE lhs underflows when substracting unsigned
    return Interval<T>{
        Excluding<T>(lhs.value),
        std::numeric_limits<T>::max(),
    };

  } else {
    // Cannot underflow when positive
    if (lhs.value >= static_cast<T>(-1)) return std::nullopt;

    // Underflows when rhs > (lhs - min)
    return Interval<T>{
        Excluding<T>(lhs.value - std::numeric_limits<T>::min()),
        std::numeric_limits<T>::max(),
    };
  }
}

template <class T>
struct MultiplyingTo {
  static_assert(std::is_arithmetic_v<T>);

  constexpr MultiplyingTo() = default;
  constexpr explicit MultiplyingTo(T v) : value(v) {}

  T value;
};

template <class T>
constexpr auto IntervalThatOverflows(MultiplyingTo<T> lhs)
    -> std::optional<Interval<T>> {
  if (lhs.value == static_cast<T>(0) || lhs.value == static_cast<T>(1)) {
    return std::nullopt;
  } else {
    if (lhs.value > 0) {
      return Interval<T>{
          Excluding<T>{
              static_cast<T>(std::numeric_limits<T>::max() / lhs.value),
          },
          std::numeric_limits<T>::max(),
      };
    } else {
      return Interval<T>{
          std::numeric_limits<T>::min(),
          Excluding<T>{
              static_cast<T>(std::numeric_limits<T>::max() / lhs.value),
          },
      };
    }
  }
}

template <class T>
constexpr auto IntervalThatUnderflows(MultiplyingTo<T> lhs)
    -> std::optional<Interval<T>> {
  if constexpr (std::is_unsigned_v<T>) {
    return std::nullopt;
  } else {
    if (lhs.value == static_cast<T>(0) || lhs.value == static_cast<T>(1) ||
        lhs.value == static_cast<T>(-1)) {
      return std::nullopt;
    } else {
      if (lhs.value > 0) {
        return Interval<T>{
            std::numeric_limits<T>::min(),
            Excluding<T>{
                static_cast<T>(std::numeric_limits<T>::min() / lhs.value),
            },
        };
      } else {
        return Interval<T>{
            Excluding<T>{
                static_cast<T>(std::numeric_limits<T>::min() / lhs.value),
            },
            std::numeric_limits<T>::max(),
        };
      }
    }
  }
}

template <class T>
auto MakeUniformDistribution(T min, T max) {
  static_assert(std::is_arithmetic_v<T>);

  if constexpr (std::is_integral_v<T>) {
    // For some unknown reason, uniform int distrib is UB for int8 and uint8...
    //
    // Therefore we create a distribution using max_t and then decorate the
    // distribution object with a lambda in order to cast from max_t to
    // int8/uint8
    if constexpr (std::is_same_v<T, std::int8_t>) {
      auto d = std::uniform_int_distribution<std::intmax_t>{
          static_cast<std::intmax_t>(min),
          static_cast<std::intmax_t>(max),
      };

      return [d = std::move(d)](auto &gen) mutable -> T {
        return static_cast<T>(d(gen));
      };

    } else if constexpr (std::is_same_v<T, std::uint8_t>) {
      auto d = std::uniform_int_distribution<std::uintmax_t>{
          static_cast<std::uintmax_t>(min),
          static_cast<std::uintmax_t>(max),
      };

      return [d = std::move(d)](auto &gen) mutable -> T {
        return static_cast<T>(d(gen));
      };
    } else {
      return std::uniform_int_distribution<T>{
          min,
          max,
      };
    }
  } else {
    return std::uniform_real_distribution<T>{
        min,
        max,
    };
  }
}

template <class T>
auto MakeUniformDistribution(Interval<T> interval) {
  return MakeUniformDistribution<T>(interval.Min(), interval.Max());
}

template <std::size_t N, class AnyDistribution>
constexpr auto MakeOneToNDistributionFrom(AnyDistribution d) noexcept {
  return [distrib = std::move(d)](auto &gen) mutable {
    using value_t = std::decay_t<decltype(std::invoke(distrib, gen))>;

    std::array<value_t, N> values;
    for (auto &v : values) {
      v = std::invoke(distrib, gen);
    }

    return values;
  };
}

template <std::size_t N, class AnyDistribution>
constexpr auto TieOneToNDistributionTo(AnyDistribution &d) noexcept {
  return [&distrib = d](auto &gen) {
    using value_t = std::decay_t<decltype(std::invoke(distrib, gen))>;

    std::array<value_t, N> values;
    for (auto &v : values) {
      v = std::invoke(distrib, gen);
    }

    return values;
  };
}

template <class T>
struct AtbArithmetic : testing::Test {
  using engine_t = std::mt19937;
  using distrib_t = decltype(MakeUniformDistribution(Interval<T>()));

  static void SetUpTestSuite() {}

  AtbArithmetic()
      : m_rnd_gen(std::random_device{}()),
        m_all_possible_values_uniform(MakeUniformDistribution(Interval<T>())){};

  void SetUp() override {
    // TODO: Give the possibility to set the SEED using GTest command line ?
    // m_rnd_gen.seed(make_rnd_seed());
  }

  template <class AnyDistribution>
  constexpr auto MakeRndValueUsing(AnyDistribution &&distribution) -> T {
    return std::invoke(FWD(distribution), m_rnd_gen);
  }

  constexpr auto MakeRndValue(Interval<T> interval) -> T {
    return MakeRndValueUsing(MakeUniformDistribution(interval));
  }

  constexpr auto MakeRndValue() -> T {
    return MakeRndValueUsing(m_all_possible_values_uniform);
  }

  void TearDown() override {}

  virtual ~AtbArithmetic() {}

  static void TearDownTestSuite() {}

 private:
  engine_t m_rnd_gen;
  distrib_t m_all_possible_values_uniform;
};

using AllInts =
    testing::Types<std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                   std::intmax_t, std::uint8_t, std::uint16_t, std::uint32_t,
                   std::uint64_t, std::uintmax_t>;

TYPED_TEST_SUITE(AtbArithmetic, AllInts);

template <class T>
constexpr auto IsOverflowing_Add(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_IsOverflowing_Add_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_IsOverflowing_Add_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_IsOverflowing_Add_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_IsOverflowing_Add_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_IsOverflowing_Add_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_IsOverflowing_Add_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_IsOverflowing_Add_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_IsOverflowing_Add_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_IsOverflowing_Add_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_IsOverflowing_Add_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(AtbArithmetic, IsOverflowing_Add) {
  REPEAT(1000) {
    EXPECT_PRED2(Not(IsOverflowing_Add<TypeParam>), static_cast<TypeParam>(0),
                 this->MakeRndValue());

    EXPECT_PRED2(Not(IsOverflowing_Add<TypeParam>), this->MakeRndValue(),
                 static_cast<TypeParam>(0));
  }

  REPEAT(1000) {
    const auto lhs = this->MakeRndValue();
    const auto interval_overflowing = IntervalThatOverflows(AddingTo{lhs});

    if (interval_overflowing.has_value()) {
      REPEAT(500) {
        EXPECT_PRED2(IsOverflowing_Add<TypeParam>, lhs,
                     this->MakeRndValue(*interval_overflowing))
            << "Interval: " << *interval_overflowing;
      }

      for (auto interval_not_overflowing :
           ComplementsOf(*interval_overflowing)) {
        if (interval_not_overflowing.has_value()) {
          REPEAT(500) {
            EXPECT_PRED2(Not(IsOverflowing_Add<TypeParam>), lhs,
                         this->MakeRndValue(*interval_not_overflowing))
                << "Interval: " << *interval_not_overflowing;
          }
        }
      }

    } else {
      // We can't overflow add with lhs
      REPEAT(1000) {
        EXPECT_PRED2(Not(IsOverflowing_Add<TypeParam>), lhs,
                     this->MakeRndValue());
      }
    }
  }
}

template <class T>
constexpr auto IsUnderflowing_Add(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_IsUnderflowing_Add_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_IsUnderflowing_Add_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_IsUnderflowing_Add_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_IsUnderflowing_Add_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_IsUnderflowing_Add_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_IsUnderflowing_Add_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_IsUnderflowing_Add_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_IsUnderflowing_Add_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_IsUnderflowing_Add_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_IsUnderflowing_Add_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(AtbArithmetic, IsUnderflowing_Add) {
  REPEAT(1000) {
    EXPECT_PRED2(Not(IsUnderflowing_Add<TypeParam>), static_cast<TypeParam>(0),
                 this->MakeRndValue());

    EXPECT_PRED2(Not(IsUnderflowing_Add<TypeParam>), this->MakeRndValue(),
                 static_cast<TypeParam>(0));
  }

  REPEAT(1000) {
    const auto lhs = this->MakeRndValue();
    const auto interval_underflowing = IntervalThatUnderflows(AddingTo{lhs});

    if (interval_underflowing.has_value()) {
      REPEAT(500) {
        EXPECT_PRED2(IsUnderflowing_Add<TypeParam>, lhs,
                     this->MakeRndValue(*interval_underflowing))
            << "Interval: " << *interval_underflowing;
      }

      for (auto interval_not_underflowing :
           ComplementsOf(*interval_underflowing)) {
        if (interval_not_underflowing.has_value()) {
          REPEAT(500) {
            EXPECT_PRED2(Not(IsUnderflowing_Add<TypeParam>), lhs,
                         this->MakeRndValue(*interval_not_underflowing))
                << "Interval: " << *interval_not_underflowing;
          }
        }
      }

    } else {
      // We can't underflow add with lhs
      REPEAT(1000) {
        EXPECT_PRED2(Not(IsUnderflowing_Add<TypeParam>), lhs,
                     this->MakeRndValue());
      }
    }
  }
}

template <class T>
constexpr auto IsOverflowing_Sub(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_IsOverflowing_Sub_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_IsOverflowing_Sub_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_IsOverflowing_Sub_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_IsOverflowing_Sub_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_IsOverflowing_Sub_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_IsOverflowing_Sub_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_IsOverflowing_Sub_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_IsOverflowing_Sub_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_IsOverflowing_Sub_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_IsOverflowing_Sub_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(AtbArithmetic, IsOverflowing_Sub) {
  REPEAT(1000) {
    EXPECT_PRED2(Not(IsOverflowing_Sub<TypeParam>), this->MakeRndValue(),
                 static_cast<TypeParam>(0));
  }

  REPEAT(1000) {
    const auto lhs = this->MakeRndValue();
    const auto interval_overflowing =
        IntervalThatOverflows(SubstractingTo{lhs});

    if (interval_overflowing.has_value()) {
      REPEAT(500) {
        EXPECT_PRED2(IsOverflowing_Sub<TypeParam>, lhs,
                     this->MakeRndValue(*interval_overflowing))
            << "Interval: " << *interval_overflowing;
      }

      for (auto interval_not_overflowing :
           ComplementsOf(*interval_overflowing)) {
        if (interval_not_overflowing.has_value()) {
          REPEAT(500) {
            EXPECT_PRED2(Not(IsOverflowing_Sub<TypeParam>), lhs,
                         this->MakeRndValue(*interval_not_overflowing))
                << "Interval: " << *interval_not_overflowing;
          }
        }
      }

    } else {
      // We can't overflow sub with lhs
      REPEAT(1000) {
        EXPECT_PRED2(Not(IsOverflowing_Sub<TypeParam>), lhs,
                     this->MakeRndValue());
      }
    }
  }
}

template <class T>
constexpr auto IsUnderflowing_Sub(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_IsUnderflowing_Sub_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_IsUnderflowing_Sub_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_IsUnderflowing_Sub_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_IsUnderflowing_Sub_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_IsUnderflowing_Sub_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_IsUnderflowing_Sub_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_IsUnderflowing_Sub_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_IsUnderflowing_Sub_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_IsUnderflowing_Sub_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_IsUnderflowing_Sub_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(AtbArithmetic, IsUnderflowing_Sub) {
  REPEAT(1000) {
    EXPECT_PRED2(Not(IsUnderflowing_Sub<TypeParam>), this->MakeRndValue(),
                 static_cast<TypeParam>(0));
  }

  REPEAT(1000) {
    const auto lhs = this->MakeRndValue();
    const auto interval_underflowing =
        IntervalThatUnderflows(SubstractingTo{lhs});

    if (interval_underflowing.has_value()) {
      REPEAT(500) {
        EXPECT_PRED2(IsUnderflowing_Sub<TypeParam>, lhs,
                     this->MakeRndValue(*interval_underflowing))
            << "Interval: " << *interval_underflowing;
      }

      for (auto interval_not_underflowing :
           ComplementsOf(*interval_underflowing)) {
        if (interval_not_underflowing.has_value()) {
          REPEAT(500) {
            EXPECT_PRED2(Not(IsUnderflowing_Sub<TypeParam>), lhs,
                         this->MakeRndValue(*interval_not_underflowing))
                << "Interval: " << *interval_not_underflowing;
          }
        }
      }

    } else {
      // We can't underflow sub with lhs
      REPEAT(1000) {
        EXPECT_PRED2(Not(IsUnderflowing_Sub<TypeParam>), lhs,
                     this->MakeRndValue());
      }
    }
  }
}

template <class T>
constexpr auto IsOverflowing_Mul(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_IsOverflowing_Mul_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_IsOverflowing_Mul_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_IsOverflowing_Mul_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_IsOverflowing_Mul_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_IsOverflowing_Mul_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_IsOverflowing_Mul_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_IsOverflowing_Mul_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_IsOverflowing_Mul_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_IsOverflowing_Mul_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_IsOverflowing_Mul_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(AtbArithmetic, IsOverflowing_Mul) {
  REPEAT(1000) {
    for (TypeParam v : {0, 1}) {
      EXPECT_PRED2(Not(IsOverflowing_Mul<TypeParam>), this->MakeRndValue(), v);
      EXPECT_PRED2(Not(IsOverflowing_Mul<TypeParam>), v, this->MakeRndValue());
    }
  }

  REPEAT(1000) {
    const auto lhs = this->MakeRndValue();
    const auto interval_overflowing = IntervalThatOverflows(MultiplyingTo{lhs});

    if (interval_overflowing.has_value()) {
      REPEAT(500) {
        EXPECT_PRED2(IsOverflowing_Mul<TypeParam>, lhs,
                     this->MakeRndValue(*interval_overflowing))
            << "Interval: " << *interval_overflowing;
      }

      for (auto interval_not_overflowing :
           ComplementsOf(*interval_overflowing)) {
        if (interval_not_overflowing.has_value()) {
          REPEAT(500) {
            EXPECT_PRED2(Not(IsOverflowing_Mul<TypeParam>), lhs,
                         this->MakeRndValue(*interval_not_overflowing))
                << "Interval: " << *interval_not_overflowing;
          }
        }
      }

    } else {
      // We can't overflow mul with lhs
      REPEAT(1000) {
        EXPECT_PRED2(Not(IsOverflowing_Mul<TypeParam>), lhs,
                     this->MakeRndValue());
      }
    }
  }
}

template <class T>
constexpr auto IsUnderflowing_Mul(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_IsUnderflowing_Mul_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_IsUnderflowing_Mul_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_IsUnderflowing_Mul_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_IsUnderflowing_Mul_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_IsUnderflowing_Mul_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_IsUnderflowing_Mul_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_IsUnderflowing_Mul_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_IsUnderflowing_Mul_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_IsUnderflowing_Mul_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_IsUnderflowing_Mul_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(AtbArithmetic, IsUnderflowing_Mul) {
  REPEAT(1000) {
    for (TypeParam v : {0, 1}) {
      EXPECT_PRED2(Not(IsUnderflowing_Mul<TypeParam>), this->MakeRndValue(), v);
      EXPECT_PRED2(Not(IsUnderflowing_Mul<TypeParam>), v, this->MakeRndValue());
    }
  }

  REPEAT(1000) {
    const auto lhs = this->MakeRndValue();
    const auto interval_underflowing =
        IntervalThatUnderflows(MultiplyingTo{lhs});

    if (interval_underflowing.has_value()) {
      REPEAT(500) {
        EXPECT_PRED2(IsUnderflowing_Mul<TypeParam>, lhs,
                     this->MakeRndValue(*interval_underflowing))
            << "Interval: " << *interval_underflowing;
      }

      for (auto interval_not_underflowing :
           ComplementsOf(*interval_underflowing)) {
        if (interval_not_underflowing.has_value()) {
          REPEAT(500) {
            EXPECT_PRED2(Not(IsUnderflowing_Mul<TypeParam>), lhs,
                         this->MakeRndValue(*interval_not_underflowing))
                << "Interval: " << *interval_not_underflowing;
          }
        }
      }

    } else {
      // We can't underflow mul with lhs
      REPEAT(1000) {
        EXPECT_PRED2(Not(IsUnderflowing_Mul<TypeParam>), lhs,
                     this->MakeRndValue());
      }
    }
  }
}

} // namespace
