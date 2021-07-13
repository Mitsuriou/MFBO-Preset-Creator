﻿#pragma once

enum class BodyNameVersion
{
  INVALID_VALUE = -1,
  CBBE_3BBB_3BA_1_50,
  CBBE_3BBB_3BA_1_51_TO_1_55,
  CBBE_SMP_3BBB_1_2_0,
  BHUNP_3BBB_2_13,
  BHUNP_3BBB_ADVANCED_2_13,
  BHUNP_3BBB_ADVANCED_VER_2_2_13,
  BHUNP_BBP_2_13,
  BHUNP_BBP_ADVANCED_2_13,
  BHUNP_TBBP_2_13,
  BHUNP_TBBP_ADVANCED_2_13,
  BHUNP_3BBB_2_15,
  BHUNP_3BBB_ADVANCED_2_15,
  BHUNP_3BBB_ADVANCED_VER_2_2_15,
  BHUNP_BBP_2_15,
  BHUNP_BBP_ADVANCED_2_15,
  BHUNP_TBBP_2_15,
  BHUNP_TBBP_ADVANCED_2_15,
  BHUNP_3BBB_2_20,
  BHUNP_3BBB_ADVANCED_2_20,
  BHUNP_3BBB_ADVANCED_VER_2_2_20,
  BHUNP_BBP_2_20,
  BHUNP_BBP_ADVANCED_2_20,
  BHUNP_TBBP_2_20,
  BHUNP_TBBP_ADVANCED_2_20,
  BHUNP_3BBB_2_25,
  BHUNP_3BBB_ADVANCED_2_25,
  BHUNP_3BBB_ADVANCED_VER_2_2_25,
  BHUNP_BBP_2_25,
  BHUNP_BBP_ADVANCED_2_25,
  BHUNP_TBBP_2_25,
  BHUNP_TBBP_ADVANCED_2_25,
  CBBE_3BBB_3BA_2_00_TO_2_04,
  CBBE_3BBB_3BA_2_05_TO_2_06,
  BHUNP_3BBB_2_30,
  BHUNP_3BBB_ADVANCED_2_30,
  BHUNP_3BBB_ADVANCED_VER_2_2_30,
  BHUNP_BBP_2_30,
  BHUNP_BBP_ADVANCED_2_30,
  BHUNP_TBBP_2_30,
  BHUNP_TBBP_ADVANCED_2_30,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30,
  BHUNP_3BBB_2_31,
  BHUNP_3BBB_ADVANCED_2_31,
  BHUNP_3BBB_ADVANCED_VER_2_2_31,
  BHUNP_BBP_2_31,
  BHUNP_BBP_ADVANCED_2_31,
  BHUNP_TBBP_2_31,
  BHUNP_TBBP_ADVANCED_2_31,
  BHUNP_3BBB_2_35,
  BHUNP_3BBB_ADVANCED_2_35,
  BHUNP_3BBB_ADVANCED_VER_2_2_35,
  BHUNP_BBP_2_35,
  BHUNP_BBP_ADVANCED_2_35,
  BHUNP_TBBP_2_35,
  BHUNP_TBBP_ADVANCED_2_35,
  MIMIR_EBONIC_BODY_1_2
};

enum class BodyName
{
  INVALID_VALUE = -1,
  CBBE_3BBB_3BA,
  CBBE_SMP_3BBB,
  BHUNP_3BBB,
  BHUNP_3BBB_ADVANCED,
  BHUNP_3BBB_ADVANCED_VER_2,
  BHUNP_BBP,
  BHUNP_BBP_ADVANCED,
  BHUNP_TBBP,
  BHUNP_TBBP_ADVANCED,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE,
  MIMIR_EBONIC_BODY
};

enum class ApplicationLanguage
{
  ENGLISH = 0,
  FRENCH = 1,
  CHINESE_TRADITIONAL = 2
};

enum class GUITheme
{
  WINDOWS_VISTA = 0,
  PAPER_LIGHT = 1,
  PAPER_DARK = 2,
  PAPER_WHITE_MONO = 3,
  PAPER_BLACK_MONO = 4,
  ALEXHUSZAGH_BREEZE_LIGHT = 5,
  ALEXHUSZAGH_BREEZE_DARK = 6,
  QUASAR_APP_DARK_STYLE = 7,
  QUASAR_APP_VISUAL_STUDIO_DARK = 8,
  MITSURIOU_LIGHT_THEME = 9,
  MITSURIOU_DARK_THEME = 10
};

enum class WindowOpeningMode
{
  MINIMIZED = 0,
  WINDOWED = 1,
  MAXIMIZED = 2
};

enum class AssistedConversionRole
{
  BODY = 0,
  FEET = 1,
  HANDS = 2,
  SKELETON = 3
};

enum class ButtonClicked
{
  YES = 0,
  NO = 1,
  CLOSE_WINDOW = 2,
  OTHER = 3
};

enum class BodyPartType
{
  BODY = 0,
  FEET = 1,
  HANDS = 2,
  BEAST_HANDS = 3
};
