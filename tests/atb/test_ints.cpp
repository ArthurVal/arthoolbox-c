#include <limits>
#include <type_traits>

#include "atb/ints.h"
#include "gtest/gtest.h"

namespace {

template <class T>
struct AtbIntsTest : testing::Test {
  static void SetUpTestSuite() {}
  constexpr AtbIntsTest() {}
  void SetUp() override {}

  static constexpr auto Max() { return std::numeric_limits<T>::max(); }
  static constexpr auto Min() { return std::numeric_limits<T>::min(); }
  static constexpr auto IsSigned() { return std::is_signed_v<T>; }
  static constexpr auto IsUnsigned() { return !IsSigned(); }

  void TearDown() override {}
  virtual ~AtbIntsTest() {}
  static void TearDownTestSuite() {}
};

template <class T>
using AtbIntsDeathTest = AtbIntsTest<T>;

using AllInts =
    testing::Types<std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                   std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>;

TYPED_TEST_SUITE(AtbIntsTest, AllInts);
TYPED_TEST_SUITE(AtbIntsDeathTest, AllInts);

#define _DEF(TYPE, NAME, ...)                      \
  if constexpr (std::is_same_v<T, TYPE>) {         \
    return atb_Add_IsOverflowing_##NAME(lhs, rhs); \
  } else

template <class T>
constexpr auto Add_IsOverflowing(T lhs, T rhs) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Add_IsOverflowing) {
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

#define _DEF(TYPE, NAME, ...)                       \
  if constexpr (std::is_same_v<T, TYPE>) {          \
    return atb_Add_IsUnderflowing_##NAME(lhs, rhs); \
  } else

template <class T>
constexpr auto Add_IsUnderflowing(T lhs, T rhs) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Add_IsUnderflowing) {
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

#define _DEF(TYPE, NAME, ...)                      \
  if constexpr (std::is_same_v<T, TYPE>) {         \
    return atb_Sub_IsOverflowing_##NAME(lhs, rhs); \
  } else

template <class T>
constexpr auto Sub_IsOverflowing(T lhs, T rhs) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Sub_IsOverflowing) {
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

#define _DEF(TYPE, NAME, ...)                       \
  if constexpr (std::is_same_v<T, TYPE>) {          \
    return atb_Sub_IsUnderflowing_##NAME(lhs, rhs); \
  } else

template <class T>
constexpr auto Sub_IsUnderflowing(T lhs, T rhs) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Sub_IsUnderflowing) {
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

#define _DEF(TYPE, NAME, ...)                      \
  if constexpr (std::is_same_v<T, TYPE>) {         \
    return atb_Mul_IsOverflowing_##NAME(lhs, rhs); \
  } else

template <class T>
constexpr auto Mul_IsOverflowing(T lhs, T rhs) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Mul_IsOverflowing) {
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

#define _DEF(TYPE, NAME, ...)                       \
  if constexpr (std::is_same_v<T, TYPE>) {          \
    return atb_Mul_IsUnderflowing_##NAME(lhs, rhs); \
  } else

template <class T>
constexpr auto Mul_IsUnderflowing(T lhs, T rhs) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Mul_IsUnderflowing) {
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

#define _DEF(TYPE, NAME, ...)                     \
  if constexpr (std::is_same_v<T, TYPE>) {        \
    return atb_Add_Safely_##NAME(lhs, rhs, dest); \
  } else

template <class T>
constexpr auto Add_Safely(T lhs, T rhs, T *const dest) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Add_Safely) {
  TypeParam res = 0;
  EXPECT_TRUE(Add_Safely<TypeParam>(1, 2, &res));
  EXPECT_EQ(res, 1 + 2);

  res = 0;
  EXPECT_TRUE(Add_Safely<TypeParam>(this->Max(), 0, &res));
  EXPECT_EQ(res, this->Max());

  res = 0;
  EXPECT_TRUE(Add_Safely<TypeParam>(this->Min(), 0, &res));
  EXPECT_EQ(res, this->Min());

  // Overflows
  res = 0;
  EXPECT_FALSE(Add_Safely<TypeParam>(this->Max(), 20, &res));
  EXPECT_EQ(res, 0);

  if constexpr (this->IsSigned()) {
    res = 0;
    EXPECT_TRUE(Add_Safely<TypeParam>(this->Max(), -20, &res));
    EXPECT_EQ(res, this->Max() - 20);

    // Underflows
    res = 0;
    EXPECT_FALSE(Add_Safely<TypeParam>(this->Min(), -20, &res));
    EXPECT_EQ(res, 0);
  }
}

TYPED_TEST(AtbIntsDeathTest, Add_Safely) {
  EXPECT_DEBUG_DEATH(Add_Safely<TypeParam>(0, 0, nullptr), "dest != NULL");
}

#define _DEF(TYPE, NAME, ...)                     \
  if constexpr (std::is_same_v<T, TYPE>) {        \
    return atb_Sub_Safely_##NAME(lhs, rhs, dest); \
  } else

template <class T>
constexpr auto Sub_Safely(T lhs, T rhs, T *const dest) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Sub_Safely) {
  TypeParam res = 0;
  EXPECT_TRUE(Sub_Safely<TypeParam>(3, 2, &res));
  EXPECT_EQ(res, 3 - 2);

  res = 0;
  EXPECT_TRUE(Sub_Safely<TypeParam>(this->Max(), 0, &res));
  EXPECT_EQ(res, this->Max());

  res = 0;
  EXPECT_TRUE(Sub_Safely<TypeParam>(this->Min(), 0, &res));
  EXPECT_EQ(res, this->Min());

  // Underflows
  res = 0;
  EXPECT_FALSE(Sub_Safely<TypeParam>(this->Min(), 20, &res));
  EXPECT_EQ(res, 0);

  if constexpr (this->IsSigned()) {
    res = 0;
    EXPECT_TRUE(Sub_Safely<TypeParam>(this->Min(), -20, &res));
    EXPECT_EQ(res, this->Min() + 20);

    // Overflows
    res = 0;
    EXPECT_FALSE(Sub_Safely<TypeParam>(this->Max(), -20, &res));
    EXPECT_EQ(res, 0);
  }
}

TYPED_TEST(AtbIntsDeathTest, Sub_Safely) {
  EXPECT_DEBUG_DEATH(Sub_Safely<TypeParam>(0, 0, nullptr), "dest != NULL");
}

#define _DEF(TYPE, NAME, ...)                     \
  if constexpr (std::is_same_v<T, TYPE>) {        \
    return atb_Mul_Safely_##NAME(lhs, rhs, dest); \
  } else

template <class T>
constexpr auto Mul_Safely(T lhs, T rhs, T *const dest) -> bool {
  ATB_INTS_X_FOREACH(_DEF) {
    static_assert(sizeof(T) == 0, "No implementation for type T");
    return false;
  }
}

#undef _DEF

TYPED_TEST(AtbIntsTest, Mul_Safely) {
  TypeParam res = 0;
  EXPECT_TRUE(Mul_Safely<TypeParam>(3, 2, &res));
  EXPECT_EQ(res, 3 * 2);

  res = 0;
  EXPECT_TRUE(Mul_Safely<TypeParam>(this->Max(), 1, &res));
  EXPECT_EQ(res, this->Max());

  res = 0;
  EXPECT_TRUE(Mul_Safely<TypeParam>(this->Max(), 0, &res));
  EXPECT_EQ(res, 0);

  // Overflows
  res = 0;
  EXPECT_FALSE(Mul_Safely<TypeParam>(this->Max(), 2, &res));
  EXPECT_EQ(res, 0);

  if constexpr (this->IsSigned()) {
    res = 0;
    EXPECT_TRUE(Mul_Safely<TypeParam>(-3, -2, &res));
    EXPECT_EQ(res, 6);

    // Overflows
    res = 0;
    EXPECT_FALSE(Mul_Safely<TypeParam>(this->Min(), -20, &res));
    EXPECT_EQ(res, 0);

    // Underflows
    res = 0;
    EXPECT_FALSE(Mul_Safely<TypeParam>(this->Max(), -2, &res));
    EXPECT_EQ(res, 0);
  }
}

TYPED_TEST(AtbIntsDeathTest, Mul_Safely) {
  EXPECT_DEBUG_DEATH(Mul_Safely<TypeParam>(0, 0, nullptr), "dest != NULL");
}

} // namespace
