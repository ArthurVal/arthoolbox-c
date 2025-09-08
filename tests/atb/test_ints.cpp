#include "atb/ints.h"

#include "gtest/gtest.h"

#include <limits>
#include <type_traits>

namespace {

template <class T> struct TestAtbInts : testing::Test {

  static void SetUpTestSuite() {}
  constexpr TestAtbInts() {}
  void SetUp() override {}

  static constexpr auto Max() { return std::numeric_limits<T>::max(); }
  static constexpr auto Min() { return std::numeric_limits<T>::min(); }
  static constexpr auto IsSigned() { return std::is_signed_v<T>; }
  static constexpr auto IsUnsigned() { return !IsSigned(); }

  void TearDown() override {}
  virtual ~TestAtbInts() {}
  static void TearDownTestSuite() {}
};

using AllInts =
    testing::Types<std::intmax_t, std::int8_t, std::int16_t, std::int32_t,
                   std::int64_t, std::uintmax_t, std::uint8_t, std::uint16_t,
                   std::uint32_t, std::uint64_t>;

TYPED_TEST_SUITE(TestAtbInts, AllInts);

template <class T> constexpr auto Add_IsOverflowing(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_Add_IsOverflowing_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_Add_IsOverflowing_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_Add_IsOverflowing_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_Add_IsOverflowing_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_Add_IsOverflowing_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_Add_IsOverflowing_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_Add_IsOverflowing_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_Add_IsOverflowing_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_Add_IsOverflowing_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_Add_IsOverflowing_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(TestAtbInts, Add_IsOverflowing) {
  EXPECT_FALSE(Add_IsOverflowing<TypeParam>(0, 0));
  EXPECT_TRUE(Add_IsOverflowing<TypeParam>(this->Max(), this->Max()));

  EXPECT_FALSE(Add_IsOverflowing<TypeParam>(0, this->Max()));
  EXPECT_FALSE(Add_IsOverflowing<TypeParam>(this->Max(), 0));

  EXPECT_TRUE(Add_IsOverflowing<TypeParam>(1, this->Max()));
  EXPECT_TRUE(Add_IsOverflowing<TypeParam>(this->Max(), 1));

  EXPECT_TRUE(Add_IsOverflowing<TypeParam>(100, this->Max()));
  EXPECT_TRUE(Add_IsOverflowing<TypeParam>(this->Max(), 100));

  EXPECT_FALSE(Add_IsOverflowing<TypeParam>(this->Min(), this->Max()));
  EXPECT_FALSE(Add_IsOverflowing<TypeParam>(this->Max(), this->Min()));

  if constexpr (this->IsSigned()) {
    EXPECT_FALSE(Add_IsOverflowing<TypeParam>(-1, this->Max()));
    EXPECT_FALSE(Add_IsOverflowing<TypeParam>(this->Max(), -1));

    EXPECT_FALSE(Add_IsOverflowing<TypeParam>(-100, this->Max()));
    EXPECT_FALSE(Add_IsOverflowing<TypeParam>(this->Max(), -100));
  }
}

template <class T> constexpr auto Add_IsUnderflowing(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_Add_IsUnderflowing_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_Add_IsUnderflowing_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_Add_IsUnderflowing_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_Add_IsUnderflowing_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_Add_IsUnderflowing_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_Add_IsUnderflowing_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_Add_IsUnderflowing_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_Add_IsUnderflowing_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_Add_IsUnderflowing_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_Add_IsUnderflowing_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(TestAtbInts, Add_IsUnderflowing) {
  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(0, 0));
  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(this->Max(), this->Max()));

  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(0, this->Max()));
  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(this->Max(), 0));

  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(1, this->Max()));
  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(this->Max(), 1));

  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(100, this->Max()));
  EXPECT_FALSE(Add_IsUnderflowing<TypeParam>(this->Max(), 100));

  if constexpr (this->IsSigned()) {
    EXPECT_TRUE(Add_IsUnderflowing<TypeParam>(this->Min(), this->Min()));

    EXPECT_TRUE(Add_IsUnderflowing<TypeParam>(this->Min(), -1));
    EXPECT_TRUE(Add_IsUnderflowing<TypeParam>(-1, this->Min()));

    EXPECT_TRUE(Add_IsUnderflowing<TypeParam>(this->Min(), -100));
    EXPECT_TRUE(Add_IsUnderflowing<TypeParam>(-100, this->Min()));
  }
}

template <class T> constexpr auto Sub_IsOverflowing(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_Sub_IsOverflowing_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_Sub_IsOverflowing_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_Sub_IsOverflowing_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_Sub_IsOverflowing_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_Sub_IsOverflowing_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_Sub_IsOverflowing_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_Sub_IsOverflowing_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_Sub_IsOverflowing_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_Sub_IsOverflowing_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_Sub_IsOverflowing_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(TestAtbInts, Sub_IsOverflowing) {
  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(0, 0));
  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(this->Max(), this->Max()));

  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(0, this->Max()));
  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(this->Max(), 0));

  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(1, this->Max()));
  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(this->Max(), 1));

  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(100, this->Max()));
  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(this->Max(), 100));

  EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(this->Min(), this->Max()));

  if constexpr (this->IsSigned()) {
    EXPECT_TRUE(Sub_IsOverflowing<TypeParam>(this->Max(), this->Min()));

    EXPECT_TRUE(Sub_IsOverflowing<TypeParam>(1, this->Min()));
    EXPECT_TRUE(Sub_IsOverflowing<TypeParam>(100, this->Min()));
    EXPECT_TRUE(Sub_IsOverflowing<TypeParam>(0, this->Min()));
    EXPECT_FALSE(Sub_IsOverflowing<TypeParam>(-1, this->Min()));
  }
}

template <class T> constexpr auto Sub_IsUnderflowing(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_Sub_IsUnderflowing_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_Sub_IsUnderflowing_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_Sub_IsUnderflowing_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_Sub_IsUnderflowing_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_Sub_IsUnderflowing_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_Sub_IsUnderflowing_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_Sub_IsUnderflowing_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_Sub_IsUnderflowing_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_Sub_IsUnderflowing_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_Sub_IsUnderflowing_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(TestAtbInts, Sub_IsUnderflowing) {
  EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(0, 0));

  EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(0, this->Min()));
  EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(this->Min(), 0));

  EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(this->Max(), 1));

  EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(this->Max(), 100));

  if constexpr (this->IsSigned()) {
    EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(0, this->Max()));
    EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(100, this->Max()));

    EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(this->Min(), this->Min()));
    EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(this->Min(), -1));
    EXPECT_FALSE(Sub_IsUnderflowing<TypeParam>(-1, this->Min()));

    EXPECT_TRUE(Sub_IsUnderflowing<TypeParam>(this->Min(), 1));
    EXPECT_TRUE(Sub_IsUnderflowing<TypeParam>(-2, this->Max()));
  } else {
    EXPECT_TRUE(Sub_IsUnderflowing<TypeParam>(0, this->Max()));
    EXPECT_TRUE(Sub_IsUnderflowing<TypeParam>(100, this->Max()));
  }
}

template <class T> constexpr auto Mul_IsOverflowing(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_Mul_IsOverflowing_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_Mul_IsOverflowing_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_Mul_IsOverflowing_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_Mul_IsOverflowing_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_Mul_IsOverflowing_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_Mul_IsOverflowing_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_Mul_IsOverflowing_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_Mul_IsOverflowing_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_Mul_IsOverflowing_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_Mul_IsOverflowing_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(TestAtbInts, Mul_IsOverflowing) {
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(0, 0));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(0, this->Max()));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(0, this->Min()));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(this->Max(), 0));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(this->Min(), 0));

  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(1, this->Max()));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(1, this->Min()));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(this->Max(), 1));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(this->Min(), 1));

  EXPECT_TRUE(Mul_IsOverflowing<TypeParam>(this->Max(), 2));
  EXPECT_TRUE(Mul_IsOverflowing<TypeParam>(2, this->Max()));

  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(this->Max() / 3, 2));
  EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(2, this->Max() / 3));

  if constexpr (this->IsSigned()) {
    EXPECT_TRUE(Mul_IsOverflowing<TypeParam>(this->Min(), -2));
    EXPECT_TRUE(Mul_IsOverflowing<TypeParam>(-2, this->Min()));
    EXPECT_FALSE(Mul_IsOverflowing<TypeParam>(this->Min(), 2));
  }
}

template <class T> constexpr auto Mul_IsUnderflowing(T lhs, T rhs) -> bool {
  if constexpr (std::is_same_v<T, std::int8_t>) {
    return atb_Mul_IsUnderflowing_i8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int16_t>) {
    return atb_Mul_IsUnderflowing_i16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int32_t>) {
    return atb_Mul_IsUnderflowing_i32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::int64_t>) {
    return atb_Mul_IsUnderflowing_i64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint8_t>) {
    return atb_Mul_IsUnderflowing_u8(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return atb_Mul_IsUnderflowing_u16(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return atb_Mul_IsUnderflowing_u32(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return atb_Mul_IsUnderflowing_u64(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::intmax_t>) {
    return atb_Mul_IsUnderflowing_imax(lhs, rhs);
  } else if constexpr (std::is_same_v<T, std::uintmax_t>) {
    return atb_Mul_IsUnderflowing_umax(lhs, rhs);
  } else {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

TYPED_TEST(TestAtbInts, Mul_IsUnderflowing) {
  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(0, 0));
  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(0, this->Max()));
  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(0, this->Min()));
  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(this->Max(), 0));
  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(this->Min(), 0));

  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(this->Max(), 1));
  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(this->Max(), 2));
  EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(this->Max(), 100));

  if constexpr (this->IsSigned()) {
    EXPECT_TRUE(Mul_IsUnderflowing<TypeParam>(this->Min(), 2));
    EXPECT_TRUE(Mul_IsUnderflowing<TypeParam>(2, this->Min()));

    EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(this->Min(), -2));
    EXPECT_FALSE(Mul_IsUnderflowing<TypeParam>(-2, this->Min()));

    EXPECT_TRUE(Mul_IsUnderflowing<TypeParam>(this->Min() / 3, 4));
    EXPECT_TRUE(Mul_IsUnderflowing<TypeParam>(4, this->Min() / 3));
  }
}

} // namespace
