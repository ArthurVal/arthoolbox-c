#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "atb/AnsiTerm.h"

#include "test_StringUtils.hpp"

#include <array>

namespace {

TEST(AtbAnsiTerm, GetCode) {

  for (const auto &[color, expected_code] : std::array{
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RESET, {false, false}}, "0"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RESET, {false, true}}, "0"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RESET, {true, false}}, "0"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RESET, {true, true}}, "0"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLACK, {false, false}}, "30"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLACK, {false, true}}, "40"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLACK, {true, false}}, "90"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLACK, {true, true}}, "100"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RED, {false, false}}, "31"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RED, {false, true}}, "41"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RED, {true, false}}, "91"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_RED, {true, true}}, "101"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_GREEN, {false, false}}, "32"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_GREEN, {false, true}}, "42"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_GREEN, {true, false}}, "92"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_GREEN, {true, true}}, "102"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_YELLOW, {false, false}}, "33"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_YELLOW, {false, true}}, "43"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_YELLOW, {true, false}}, "93"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_YELLOW, {true, true}}, "103"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLUE, {false, false}}, "34"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLUE, {false, true}}, "44"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLUE, {true, false}}, "94"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_BLUE, {true, true}}, "104"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_MAGENTA, {false, false}},
                     "35"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_MAGENTA, {false, true}}, "45"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_MAGENTA, {true, false}}, "95"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_MAGENTA, {true, true}}, "105"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_CYAN, {false, false}}, "36"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_CYAN, {false, true}}, "46"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_CYAN, {true, false}}, "96"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_CYAN, {true, true}}, "106"},

           std::pair{atb_AnsiColor{ATB_ANSICOLOR_WHITE, {false, false}}, "37"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_WHITE, {false, true}}, "47"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_WHITE, {true, false}}, "97"},
           std::pair{atb_AnsiColor{ATB_ANSICOLOR_WHITE, {true, true}}, "107"},
       }) {
    EXPECT_EQ(atb_AnsiColor_GetCode(color), expected_code);
  }
}
} // namespace
