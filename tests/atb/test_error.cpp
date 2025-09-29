#include <algorithm>
#include <array>
#include <cstring>
#include <optional>
#include <string_view>
using namespace std::literals::string_view_literals;

#include "atb/error.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "helper/Core.hpp"
using helper::DoNot;

#include "helper/Error.hpp"

namespace {

struct ErrorFormatterMock {
  MOCK_METHOD(size_t, Describe, (atb_ErrorCode_t, char *, size_t, size_t *));
};

auto Describe_Mock(void *data, atb_ErrorCode_t code, char *d_first,
                   size_t d_size, size_t *written) -> bool {
  return reinterpret_cast<ErrorFormatterMock *>(data)->Describe(
      code, d_first, d_size, written);
}

auto BindFormatterTo(ErrorFormatterMock &mock) -> atb_ErrorFormatter {
  atb_ErrorFormatter fmt;
  fmt.data = &(mock);
  fmt.Describe = Describe_Mock;
  return fmt;
}

auto FindAvailableCategory() -> std::optional<atb_ErrorCategory_t> {
  atb_ErrorCategory_t cat = K_ATB_ERROR_GENERIC;
  while ((++cat != K_ATB_ERROR_GENERIC) &&
         (atb_ErrorCategory_HasFormatter(cat)));

  if (cat != K_ATB_ERROR_GENERIC) {
    return cat;
  } else {
    return std::nullopt;
  }
}

struct AtbErrorCategoryTest : testing::Test {
  static void SetUpTestSuite() {}
  AtbErrorCategoryTest() {}
  void SetUp() override {
    using testing::_;
    using testing::Return;
    ON_CALL(mock, Describe(_, _, _, _)).WillByDefault(Return(false));
  }

  void TearDown() override {}
  virtual ~AtbErrorCategoryTest() {}
  static void TearDownTestSuite() {}

  ErrorFormatterMock mock;
};

using AtbErrorCategoryDeathTest = AtbErrorCategoryTest;

TEST_F(AtbErrorCategoryTest, HasFormatter) {
  for (atb_ErrorCategory_t pre_defined_cat : {
           K_ATB_ERROR_RAW,
           K_ATB_ERROR_GENERIC,
       }) {
    EXPECT_PRED1(atb_ErrorCategory_HasFormatter, pre_defined_cat);
  }
}

TEST_F(AtbErrorCategoryDeathTest, AddAndRemoveFormatter) {
  const auto str = "Coucou";
  const auto itf = BindFormatterTo(mock);

  EXPECT_DEBUG_DEATH(
      atb_ErrorCategory_AddFormatter(2, NULL, itf, K_ATB_ERROR_IGNORED),
      "name != NULL");

  auto wrong_itf = itf;
  wrong_itf.Describe = nullptr;
  EXPECT_DEBUG_DEATH(
      atb_ErrorCategory_AddFormatter(2, str, wrong_itf, K_ATB_ERROR_IGNORED),
      "fmt.Describe != NULL");
}

TEST_F(AtbErrorCategoryTest, AddAndRemoveFormatter) {
  constexpr size_t max_name_size = (1 << 6);
  auto name_too_long =
      std::array<char,
                 max_name_size + 1 /* 'over sized by 1' */ + 1 /* '\0' */>{};

  std::fill(name_too_long.begin(), name_too_long.end(), 'a');
  name_too_long.back() = '\0';

  // We look for any available category
  atb_Error err;
  auto mock_itf = BindFormatterTo(mock);

  EXPECT_PRED4(DoNot(atb_ErrorCategory_AddFormatter), K_ATB_ERROR_GENERIC,
               "TOTO", mock_itf, &err);
  EXPECT_EQ(err.category, K_ATB_ERROR_GENERIC);
  EXPECT_EQ(err.code, K_ATB_ERROR_GENERIC_INVALID_ARGUMENT);

  EXPECT_PRED4(DoNot(atb_ErrorCategory_AddFormatter), K_ATB_ERROR_RAW, "TOTO",
               mock_itf, &err);
  EXPECT_EQ(err.category, K_ATB_ERROR_GENERIC);
  EXPECT_EQ(err.code, K_ATB_ERROR_GENERIC_INVALID_ARGUMENT);

  auto available_cat = FindAvailableCategory();
  ASSERT_TRUE(available_cat.has_value());

  EXPECT_PRED4(DoNot(atb_ErrorCategory_AddFormatter), *available_cat,
               name_too_long.data(), mock_itf, &err);
  EXPECT_EQ(err.category, K_ATB_ERROR_GENERIC);
  EXPECT_EQ(err.code, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);

  EXPECT_PRED1(DoNot(atb_ErrorCategory_HasFormatter), *available_cat);
  EXPECT_PRED1(DoNot(atb_ErrorCategory_RemoveFormatter), *available_cat);
  EXPECT_PRED4(atb_ErrorCategory_AddFormatter, *available_cat, "TOTO", mock_itf,
               &err);
  EXPECT_PRED1(atb_ErrorCategory_HasFormatter, *available_cat);

  EXPECT_PRED1(atb_ErrorCategory_RemoveFormatter, *available_cat);
  EXPECT_PRED1(DoNot(atb_ErrorCategory_HasFormatter), *available_cat);
}

struct AtbErrorTest : AtbErrorCategoryTest {
  static constexpr char mock_fmt_name[] = "MOCK";

  static void SetUpTestSuite() {}

  AtbErrorTest() {}
  void SetUp() override {
    atb_Error err;

    auto available_cat = FindAvailableCategory();
    ASSERT_TRUE(available_cat.has_value());

    mock_category = std::move(*available_cat);
    ASSERT_PRED4(atb_ErrorCategory_AddFormatter, mock_category, mock_fmt_name,
                 BindFormatterTo(mock), &err)
        << err;
  }

  void MockError_Set(atb_Error *const err, atb_ErrorCode_t code) {
    atb_Error_Set(err, mock_category, code);
  }

  void TearDown() override {
    EXPECT_PRED1(atb_ErrorCategory_RemoveFormatter, mock_category);
  }
  virtual ~AtbErrorTest() {}
  static void TearDownTestSuite() {}

 private:
  atb_ErrorCategory_t mock_category;
};

TEST_F(AtbErrorTest, Ignored) {
  EXPECT_PRED1(atb_Error_IsIgnored, (atb_Error *)K_ATB_ERROR_IGNORED);

  atb_Error err;
  EXPECT_PRED1(DoNot(atb_Error_IsIgnored), &err);
}

TEST_F(AtbErrorTest, Set) {
  // No crashs
  atb_Error_Set(K_ATB_ERROR_IGNORED, 0, 0);

  atb_Error err;
  err.category = 0;
  err.code = 0;

  atb_Error_Set(&err, 123, -1);
  EXPECT_EQ(err.category, 123);
  EXPECT_EQ(err.code, -1);

  atb_RawError_Set(&err, -2);
  EXPECT_EQ(err.category, K_ATB_ERROR_RAW);
  EXPECT_EQ(err.code, -2);

  atb_GenericError_Set(&err, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);
  EXPECT_EQ(err.category, K_ATB_ERROR_GENERIC);
  EXPECT_EQ(err.code, K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE);
}

TEST(AtbErrorDeathTest, Describe) {
  char str[20];
  std::size_t written;
  atb_Error err;

  EXPECT_DEBUG_DEATH(atb_Error_Describe(NULL, str, std::size(str), &written),
                     "self != NULL");

  EXPECT_DEBUG_DEATH(atb_Error_Describe(NULL, str, std::size(str), NULL),
                     "self != NULL");

  EXPECT_DEBUG_DEATH(atb_Error_Describe(&err, str, std::size(str), NULL),
                     "written != NULL");
}

TEST_F(AtbErrorTest, DescribeUnknownCat) {
  atb_Error err;

  char str[256];
  std::fill(std::begin(str), std::end(str), '\0');

  std::size_t written = 0;

  auto unknown_cat = FindAvailableCategory();
  ASSERT_TRUE(unknown_cat.has_value());

  err.category = *unknown_cat;

  for (auto code : std::array{
           0,
           10,
           123,
           135345,
           675467,
           -14,
           -5623,
           -135345,
           -675467,
           65354636,
           std::numeric_limits<atb_ErrorCode_t>::max(),
           std::numeric_limits<atb_ErrorCode_t>::min(),
       }) {
    err.code = code;

    SCOPED_TRACE(SCOPE_LOOP_MSG_1(err));

    auto expected_str =
        helper::MakeStringFromFmt("0x%.2x: %i", err.category, err.code);

    written = 0;
    EXPECT_TRUE(atb_Error_Describe(&err, nullptr, 0, &written));
    EXPECT_EQ(written, expected_str.size());

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_FALSE(
        atb_Error_Describe(&err, str, expected_str.size() - 1, &written));
    EXPECT_EQ(written, 0);
    EXPECT_NE(std::string_view(str, written), expected_str);

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_TRUE(atb_Error_Describe(&err, str, expected_str.size(), &written));
    EXPECT_EQ(written, expected_str.size());
    EXPECT_EQ(std::string_view(str, written), expected_str);

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_TRUE(
        atb_Error_Describe(&err, str, expected_str.size() + 3, &written));
    EXPECT_EQ(written, expected_str.size());
    EXPECT_EQ(std::string_view(str, written), expected_str);
  }
}

TEST_F(AtbErrorTest, DescribeRawError) {
  atb_Error err;

  char str[256];
  std::fill(std::begin(str), std::end(str), '\0');

  std::size_t written = 0;

  err.category = K_ATB_ERROR_RAW;

  for (auto code : std::array{
           0,
           10,
           123,
           135345,
           675467,
           -14,
           -5623,
           -135345,
           -675467,
           65354636,
           std::numeric_limits<atb_ErrorCode_t>::max(),
           std::numeric_limits<atb_ErrorCode_t>::min(),
       }) {
    err.code = code;

    SCOPED_TRACE(SCOPE_LOOP_MSG_1(err));

    auto expected_str = helper::MakeStringFromFmt("raw: %i", err.code);

    written = 0;
    EXPECT_TRUE(atb_Error_Describe(&err, nullptr, 0, &written));
    EXPECT_EQ(written, expected_str.size());

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_FALSE(
        atb_Error_Describe(&err, str, expected_str.size() - 1, &written));
    EXPECT_EQ(written, 0);
    EXPECT_NE(std::string_view(str, written), expected_str);

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_TRUE(atb_Error_Describe(&err, str, expected_str.size(), &written));
    EXPECT_EQ(written, expected_str.size());
    EXPECT_EQ(std::string_view(str, written), expected_str);

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_TRUE(
        atb_Error_Describe(&err, str, expected_str.size() + 3, &written));
    EXPECT_EQ(written, expected_str.size());
    EXPECT_EQ(std::string_view(str, written), expected_str);
  }
}

TEST_F(AtbErrorTest, DescribeGeneric) {
  atb_Error err;

  char str[256];
  std::fill(std::begin(str), std::end(str), '\0');

  std::size_t written = 0;

  err.category = K_ATB_ERROR_GENERIC;

  for (auto code : std::array{
           K_ATB_ERROR_GENERIC_INVALID_ARGUMENT,
           K_ATB_ERROR_GENERIC_BAD_MESSAGE,
           K_ATB_ERROR_GENERIC_NOT_A_DIRECTORY,
       }) {
    err.code = code;

    SCOPED_TRACE(SCOPE_LOOP_MSG_1(err));

    auto expected_str = std::string{"generic: "} + std::strerror(code);

    written = 0;
    EXPECT_TRUE(atb_Error_Describe(&err, nullptr, 0, &written));
    EXPECT_EQ(written, expected_str.size());

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_FALSE(
        atb_Error_Describe(&err, str, expected_str.size() - 1, &written));
    EXPECT_EQ(written, 0);
    EXPECT_NE(std::string_view(str, written), expected_str);

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_TRUE(atb_Error_Describe(&err, str, expected_str.size(), &written));
    EXPECT_EQ(written, expected_str.size());
    EXPECT_EQ(std::string_view(str, written), expected_str);

    written = 0;
    std::fill(std::begin(str), std::end(str), '\0');
    EXPECT_TRUE(
        atb_Error_Describe(&err, str, expected_str.size() + 3, &written));
    EXPECT_EQ(written, expected_str.size());
    EXPECT_EQ(std::string_view(str, written), expected_str);
  }
}

TEST_F(AtbErrorTest, Describe) {
  using testing::DoAll;
  using testing::Ne;
  using testing::Return;
  using testing::SetArgPointee;
  using testing::SetArrayArgument;

  char str[256];
  std::fill(std::begin(str), std::end(str), '\0');

  atb_Error err;
  std::size_t written = 0;

  const std::string expected_mock_header =
      std::string{mock_fmt_name} + std::string{": "};

  constexpr auto str_written = "ABC"sv;

  MockError_Set(&err, 10);

  EXPECT_CALL(mock, Describe(err.code, nullptr, 0, Ne(nullptr)))
      .WillOnce(DoAll(SetArgPointee<3>(20), Return(true)))
      .WillOnce(DoAll(SetArgPointee<3>(str_written.size()), Return(true)))
      .WillRepeatedly(DoAll(SetArgPointee<3>(std::size(str) + 1), Return(true)))
      .RetiresOnSaturation();

  EXPECT_CALL(
      mock, Describe(err.code, std::next(str, expected_mock_header.size()),
                     std::size(str) - expected_mock_header.size(), Ne(nullptr)))
      .Times(1)
      .WillOnce(
          DoAll(SetArrayArgument<1>(str_written.cbegin(), str_written.cend()),
                SetArgPointee<3>(str_written.size()), Return(true)))
      .RetiresOnSaturation();

  EXPECT_TRUE(atb_Error_Describe(&err, nullptr, 0, &written));
  EXPECT_EQ(written, expected_mock_header.size() + 20);

  written = 0;
  EXPECT_TRUE(atb_Error_Describe(&err, str, std::size(str), &written));
  EXPECT_EQ(written, expected_mock_header.size() + str_written.size());
  EXPECT_EQ(std::string_view(str, written),
            expected_mock_header + std::string{str_written});

  written = 0;
  std::fill(std::begin(str), std::end(str), '\0');

  // Mock Describe too large
  EXPECT_FALSE(atb_Error_Describe(&err, str, std::size(str), &written));
  EXPECT_EQ(str[0], '\0');
}

} // namespace
