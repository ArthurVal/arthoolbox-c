#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define ATB_LOG_LEVEL ATB_LOG_LEVEL_DEBUG
#include "atb/Logging.h"

#include <cstdlib>

namespace {
TEST(AtbLogging, Debug) {
  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(DEBUG, "Test");

  EXPECT_THAT(testing::internal::GetCapturedStdout(),
              testing::ContainsRegex("\\[[0-9]+\\]\\[DEBUG  \\] Test"));
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "");

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(DEBUG, "Test %d %s", 42, "Coucou");

  EXPECT_THAT(
      testing::internal::GetCapturedStdout(),
      testing::ContainsRegex("\\[[0-9]+\\]\\[DEBUG  \\] Test 42 Coucou"));
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "");
}

TEST(AtbLogging, Info) {
  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(INFO, "Test");

  EXPECT_THAT(testing::internal::GetCapturedStdout(),
              testing::ContainsRegex("\\[[0-9]+\\]\\[INFO   \\] Test"));
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "");

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(INFO, "Test %d %s", 42, "Coucou");

  EXPECT_THAT(
      testing::internal::GetCapturedStdout(),
      testing::ContainsRegex("\\[[0-9]+\\]\\[INFO   \\] Test 42 Coucou"));
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "");
}

TEST(AtbLogging, Warning) {
  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(WARNING, "Test");

  EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
  EXPECT_THAT(testing::internal::GetCapturedStderr(),
              testing::ContainsRegex("\\[[0-9]+\\]\\[WARNING\\] Test"));

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(WARNING, "Test %d %s", 42, "Coucou");

  EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
  EXPECT_THAT(
      testing::internal::GetCapturedStderr(),
      testing::ContainsRegex("\\[[0-9]+\\]\\[WARNING\\] Test 42 Coucou"));
}

TEST(AtbLogging, Error) {
  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(ERROR, "Test");

  EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
  EXPECT_THAT(testing::internal::GetCapturedStderr(),
              testing::ContainsRegex("\\[[0-9]+\\]\\[ERROR  \\] Test"));

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  ATB_LOG(ERROR, "Test %d %s", 42, "Coucou");

  EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
  EXPECT_THAT(
      testing::internal::GetCapturedStderr(),
      testing::ContainsRegex("\\[[0-9]+\\]\\[ERROR  \\] Test 42 Coucou"));
}

TEST(AtbLogging, Fatal) {
  EXPECT_EXIT(ATB_LOG(FATAL, "Test"), ::testing::ExitedWithCode(EXIT_FAILURE),
              testing::ContainsRegex("\\[[0-9]+\\]\\[FATAL  \\] Test"));

  EXPECT_EXIT(
      ATB_LOG(FATAL, "Test %d %s", 42, "Coucou"),
      ::testing::ExitedWithCode(EXIT_FAILURE),
      testing::ContainsRegex("\\[[0-9]+\\]\\[FATAL  \\] Test 42 Coucou"));
}

} // namespace
