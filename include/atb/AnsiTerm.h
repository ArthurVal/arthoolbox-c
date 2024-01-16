#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include "atb/StaticArray.h" /* atb_StaticArray_IsInRangeOf */
#include "atb/String.h"      /* atb_ConstStringView */

#include <assert.h>

/**
 *  \brief Holds ANSI terminal colors values
 */
typedef enum {
  ATB_ANSICOLOR_BLACK = 0,
  ATB_ANSICOLOR_RED,
  ATB_ANSICOLOR_GREEN,
  ATB_ANSICOLOR_YELLOW,
  ATB_ANSICOLOR_BLUE,
  ATB_ANSICOLOR_MAGENTA,
  ATB_ANSICOLOR_CYAN,
  ATB_ANSICOLOR_WHITE,

  ATB_ANSICOLOR_RESET,
} atb_AnsiColor_Value;

static inline struct atb_ConstStringView
atb_AnsiColor_Value_ToString(atb_AnsiColor_Value value) {
  switch (value) {
  case ATB_ANSICOLOR_BLACK:
    return atb_ConstStringView_FromStaticString("BLACK");
  case ATB_ANSICOLOR_RED:
    return atb_ConstStringView_FromStaticString("RED");
  case ATB_ANSICOLOR_GREEN:
    return atb_ConstStringView_FromStaticString("GREEN");
  case ATB_ANSICOLOR_YELLOW:
    return atb_ConstStringView_FromStaticString("YELLOW");
  case ATB_ANSICOLOR_BLUE:
    return atb_ConstStringView_FromStaticString("BLUE");
  case ATB_ANSICOLOR_MAGENTA:
    return atb_ConstStringView_FromStaticString("MAGENTA");
  case ATB_ANSICOLOR_CYAN:
    return atb_ConstStringView_FromStaticString("CYAN");
  case ATB_ANSICOLOR_WHITE:
    return atb_ConstStringView_FromStaticString("WHITE");
  case ATB_ANSICOLOR_RESET:
    return atb_ConstStringView_FromStaticString("RESET");
  }

  return atb_ConstStringView_FromStaticString("UNKNOWN");
}

/**
 *  \brief Holds properties associated to a color
 */
struct atb_AnsiColor_Property {
  unsigned bright : 1;
  unsigned background : 1;
};

#define atb_AnsiColor_Property_Fmt "{.bright=%d, .background=%d}"
#define atb_AnsiColor_Property_FmtArg(prop) (prop).bright, (prop).background

/**
 *  \brief Ansi terminal color representation
 */
struct atb_AnsiColor {
  atb_AnsiColor_Value value;
  struct atb_AnsiColor_Property property;
};

#define atb_AnsiColor_Fmt                                                      \
  "{.value=" atb_String_Fmt ", .property=" atb_AnsiColor_Property_Fmt "}"
#define atb_AnsiColor_FmtArg(color)                                            \
  atb_String_FmtArg(atb_AnsiColor_Value_ToString((color).value)),              \
      atb_AnsiColor_Property_FmtArg((color).property)

/**
 *  \return atb_ConstStringView Corresponding to the ANSI terminal string code
 *
 *  \param[in] color The color we wish to get the code from
 */
static inline struct atb_ConstStringView
atb_AnsiColor_GetCode(struct atb_AnsiColor const color) {

  if (color.value == ATB_ANSICOLOR_RESET) {
    return atb_ConstStringView_FromStaticString("0");
  }

  const struct atb_ConstStringView ANSI_COLOR_TABLE[][2][2] = {
      [ATB_ANSICOLOR_BLACK] =
          {
              /* NORMAL */
              {
                  atb_ConstStringView_FromStaticString("30"), /* = FG */
                  atb_ConstStringView_FromStaticString("40"), /* = BG */

              },
              /* BRIGHT */
              {
                  atb_ConstStringView_FromStaticString("90"),  /* = FG */
                  atb_ConstStringView_FromStaticString("100"), /* = BG */
              },
          },

      [ATB_ANSICOLOR_RED] =
          {
              {
                  atb_ConstStringView_FromStaticString("31"),
                  atb_ConstStringView_FromStaticString("41"),
              },
              {
                  atb_ConstStringView_FromStaticString("91"),
                  atb_ConstStringView_FromStaticString("101"),
              },
          },

      [ATB_ANSICOLOR_GREEN] =
          {
              {
                  atb_ConstStringView_FromStaticString("32"),
                  atb_ConstStringView_FromStaticString("42"),
              },
              {
                  atb_ConstStringView_FromStaticString("92"),
                  atb_ConstStringView_FromStaticString("102"),
              },
          },

      [ATB_ANSICOLOR_YELLOW] =
          {
              {
                  atb_ConstStringView_FromStaticString("33"),
                  atb_ConstStringView_FromStaticString("43"),
              },
              {
                  atb_ConstStringView_FromStaticString("93"),
                  atb_ConstStringView_FromStaticString("103"),
              },
          },

      [ATB_ANSICOLOR_BLUE] =
          {
              {
                  atb_ConstStringView_FromStaticString("34"),
                  atb_ConstStringView_FromStaticString("44"),
              },
              {
                  atb_ConstStringView_FromStaticString("94"),
                  atb_ConstStringView_FromStaticString("104"),
              },
          },

      [ATB_ANSICOLOR_MAGENTA] =
          {
              {
                  atb_ConstStringView_FromStaticString("35"),
                  atb_ConstStringView_FromStaticString("45"),
              },
              {
                  atb_ConstStringView_FromStaticString("95"),
                  atb_ConstStringView_FromStaticString("105"),
              },
          },

      [ATB_ANSICOLOR_CYAN] =
          {
              {
                  atb_ConstStringView_FromStaticString("36"),
                  atb_ConstStringView_FromStaticString("46"),
              },
              {
                  atb_ConstStringView_FromStaticString("96"),
                  atb_ConstStringView_FromStaticString("106"),
              },
          },

      [ATB_ANSICOLOR_WHITE] =
          {
              {
                  atb_ConstStringView_FromStaticString("37"),
                  atb_ConstStringView_FromStaticString("47"),
              },
              {
                  atb_ConstStringView_FromStaticString("97"),
                  atb_ConstStringView_FromStaticString("107"),
              },
          },
  };

  /* Checks for out of bound access */
  assert(atb_StaticArray_IsInRangeOf(ANSI_COLOR_TABLE, color.value));

  return ANSI_COLOR_TABLE[color.value][color.property.bright]
                         [color.property.background];
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
