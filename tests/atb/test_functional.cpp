#include "test_functional.hpp"

#include "gtest/gtest.h"

namespace atb {

namespace {

TEST(AtbFunctionalTest, Bind) {
  using ::testing::Return;
  {
    MockCallable<void> mock;
    EXPECT_CALL(mock, Call()).WillRepeatedly(Return()).RetiresOnSaturation();
    mock();
  }

  {
    MockCallable<void, int> mock;
    EXPECT_CALL(mock, Call(3))
        .Times(2)
        .WillRepeatedly(Return())
        .RetiresOnSaturation();
    mock(3);
    mock(3);
  }
}

TEST(AtbFunctionalTest, IsValid) {
  using mock_t = MockCallable<void>;

  mock_t mock;

  mock_t::callable_t callable = *mock.Itf();
  EXPECT_TRUE(ATB_CALLABLE_IS_VALID(callable));

  callable.data = nullptr;
  EXPECT_TRUE(ATB_CALLABLE_IS_VALID(callable));

  callable = *mock.Itf();
  callable.fn = nullptr;
  EXPECT_FALSE(ATB_CALLABLE_IS_VALID(callable));

  callable = *mock.Itf();
  callable.fn = nullptr;
  callable.data = nullptr;
  EXPECT_FALSE(ATB_CALLABLE_IS_VALID(callable));
}

TEST(AtbFunctionalTest, InvokeUnsafely) {
  using ::testing::Return;

  using mock_t = MockCallable<void, int>;

  {
    mock_t::callable_t invalid = K_ATB_INIT_BIND_NULL;
    EXPECT_DEATH({ ATB_INVOKE_UNSAFELY(invalid, 5); }, "");
  }

  mock_t mock;
  EXPECT_CALL(mock, Call(3)).WillRepeatedly(Return()).RetiresOnSaturation();
  ATB_INVOKE_UNSAFELY(*mock.Itf(), 3);
}

TEST(AtbFunctionalTest, Invoke) {
  using ::testing::Return;

  using mock_t = MockCallable<void, double, int>;

  {
    mock_t::callable_t invalid = K_ATB_INIT_BIND_NULL;
    ATB_INVOKE(invalid, 5., 4);
  }

  mock_t mock;
  EXPECT_CALL(mock, Call(mock_t::arg_t{3.14, 278}))
      .WillRepeatedly(Return())
      .RetiresOnSaturation();

  ATB_INVOKE(*mock.Itf(), 3.14, 278);
}

TEST(AtbFunctionalTest, InvokeR) {
  using ::testing::Return;

  using mock_t = MockCallable<int, double, int>;
  int res;

  {
    mock_t::callable_t invalid = K_ATB_INIT_BIND_NULL;

    res = -1;
    ATB_INVOKE_R(res, invalid, 5., 4);
    EXPECT_EQ(res, -1);
  }

  mock_t mock;
  EXPECT_CALL(mock, Call(mock_t::arg_t{3.14, 278}))
      .WillRepeatedly(Return(42))
      .RetiresOnSaturation();

  res = 0;
  ATB_INVOKE_R(res, *mock.Itf(), 3.14, 278);
  EXPECT_EQ(res, 42);
}

TEST(AtbFunctionalTest, InvokeDefault) {
  using ::testing::Return;

  using mock_t = MockCallable<int, double, int>;

  {
    mock_t::callable_t invalid = K_ATB_INIT_BIND_NULL;
    EXPECT_EQ(ATB_INVOKE_DEFAULT(42, invalid, 5., 4), 42);
  }

  mock_t mock;
  EXPECT_CALL(mock, Call(mock_t::arg_t{5., 4}))
      .WillRepeatedly(Return(33))
      .RetiresOnSaturation();

  EXPECT_EQ(ATB_INVOKE_DEFAULT(100, *mock.Itf(), 5., 4), 33);
}

} // namespace

} // namespace atb
