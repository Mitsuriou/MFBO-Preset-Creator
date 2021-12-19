﻿#pragma once

enum class BodyName
{
  _INVALID_VALUE = -1,
  CBBE_3BA_3BBB,
  BHUNP_UUNP_NEXT_GENERATION,
  CBBE_SMP_3BBB,
  COCO_BODY_CBBE,
  COCO_BODY_UUNP,
  MIMIR_EBONIC_BODY
};

enum class BodyVariant
{
  _INVALID_VALUE = -1,
  CBBE_3BA_3BBB,
  BHUNP_3BBB,
  BHUNP_3BBB_ADVANCED,
  BHUNP_3BBB_ADVANCED_VER_2,
  BHUNP_BBP,
  BHUNP_BBP_ADVANCED,
  BHUNP_TBBP,
  BHUNP_TBBP_ADVANCED,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE,
  CBBE_SMP_3BBB,
  COCO_BODY_CBBE,
  COCO_BODY_UUNP,
  MIMIR_EBONIC_BODY
};

enum class BodyNameVersion
{
  _INVALID_VALUE = -1,
  // CBBE_3BA_3BBB
  CBBE_3BA_3BBB_1_50,
  CBBE_3BA_3BBB_1_51_TO_1_55,
  CBBE_3BA_3BBB_2_02_TO_2_04,
  CBBE_3BA_3BBB_2_06,
  CBBE_3BA_3BBB_2_11_AND_2_12,
  CBBE_3BA_3BBB_2_13_AND_2_14,
  // BHUNP_3BBB
  BHUNP_3BBB_2_20,
  BHUNP_3BBB_2_25,
  BHUNP_3BBB_2_30,
  BHUNP_3BBB_2_31,
  BHUNP_3BBB_2_35_TO_2_42,
  // BHUNP_3BBB_ADVANCED
  BHUNP_3BBB_ADVANCED_2_20,
  BHUNP_3BBB_ADVANCED_2_25,
  BHUNP_3BBB_ADVANCED_2_30,
  BHUNP_3BBB_ADVANCED_2_31,
  BHUNP_3BBB_ADVANCED_2_35_TO_2_42,
  // BHUNP_3BBB_ADVANCED_VER_2
  BHUNP_3BBB_ADVANCED_VER_2_2_20,
  BHUNP_3BBB_ADVANCED_VER_2_2_25,
  BHUNP_3BBB_ADVANCED_VER_2_2_30,
  BHUNP_3BBB_ADVANCED_VER_2_2_31,
  BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42,
  // BHUNP_BBP
  BHUNP_BBP_2_20,
  BHUNP_BBP_2_25,
  BHUNP_BBP_2_30,
  BHUNP_BBP_2_31,
  BHUNP_BBP_2_35_TO_2_42,
  // BHUNP_BBP_ADVANCED
  BHUNP_BBP_ADVANCED_2_20,
  BHUNP_BBP_ADVANCED_2_25,
  BHUNP_BBP_ADVANCED_2_30,
  BHUNP_BBP_ADVANCED_2_31,
  BHUNP_BBP_ADVANCED_2_35_TO_2_42,
  // BHUNP_TBBP
  BHUNP_TBBP_2_20,
  BHUNP_TBBP_2_25,
  BHUNP_TBBP_2_30,
  BHUNP_TBBP_2_31,
  BHUNP_TBBP_2_35_TO_2_42,
  // BHUNP_TBBP_ADVANCED
  BHUNP_TBBP_ADVANCED_2_20,
  BHUNP_TBBP_ADVANCED_2_25,
  BHUNP_TBBP_ADVANCED_2_30,
  BHUNP_TBBP_ADVANCED_2_31,
  BHUNP_TBBP_ADVANCED_2_35_TO_2_42,
  // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30,
  // CBBE_SMP_3BBB
  CBBE_SMP_3BBB_1_2_0,
  // COCO_BODY_CBBE
  COCO_BODY_CBBE_V4,
  COCO_BODY_CBBE_V6,
  // COCO_BODY_UUNP
  COCO_BODY_UUNP_V3,
  COCO_BODY_UUNP_V4,
  COCO_BODY_UUNP_V6,
  // MIMIR_EBONIC_BODY
  MIMIR_EBONIC_BODY_1_2,
  MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX
};

enum class FeetName
{
  _INVALID_VALUE = -1,
  CBBE,
  BHUNP,
  CBBE_3BA_3BBB,
  COCO_BODY,
  MIMIR_EBONIC_BODY,
  MORE_SLIDERS_FOR_FEET,
  HG_FEET,
  KHRYSAMERE_HG_FEET
};

enum class FeetVariant
{
  _INVALID_VALUE = -1,
  CBBE,
  BHUNP,
  CBBE_3BA_3BBB,
  COCO_BODY_CBBE,
  COCO_BODY_UUNP,
  MIMIR_EBONIC_BODY,
  MORE_SLIDERS_FOR_FEET_NORMAL_CBBE,
  MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP,
  MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE,
  MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP,
  HG_FEET,
  KHRYSAMERE_HG_FEET
};

enum class FeetNameVersion
{
  _INVALID_VALUE = -1,
  // CBBE
  CBBE_1_6_1,
  // BHUNP
  BHUNP_3BBB_2_20,
  BHUNP_3BBB_2_25,
  BHUNP_3BBB_2_30,
  BHUNP_3BBB_2_31,
  BHUNP_3BBB_2_35_TO_2_42,
  // CBBE_3BA_3BBB
  CBBE_3BA_3BBB_1_50,
  CBBE_3BA_3BBB_1_51_TO_1_55,
  CBBE_3BA_3BBB_2_02_TO_2_04,
  CBBE_3BA_3BBB_2_06,
  CBBE_3BA_3BBB_2_11_AND_2_12,
  CBBE_3BA_3BBB_2_13_AND_2_14,
  // COCO_BODY_CBBE
  COCO_BODY_CBBE_V4,
  COCO_BODY_CBBE_V6,
  // COCO_BODY_UUNP
  COCO_BODY_UUNP_V3,
  COCO_BODY_UUNP_V4,
  COCO_BODY_UUNP_V6,
  // MIMIR_EBONIC_BODY
  MIMIR_EBONIC_BODY_1_2,
  MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX,
  // MORE_SLIDERS_FOR_FEET
  MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0,
  MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1,
  MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0,
  MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1,
  // HG_FEET
  HG_FEET_1,
  // KHRYSAMERE_HG_FEET
  KHRYSAMERE_HG_FEET_1_0,
  KHRYSAMERE_HG_FEET_1_3_3,
  KHRYSAMERE_HG_FEET_1_4,
  KHRYSAMERE_HG_FEET_1_5,
  KHRYSAMERE_HG_FEET_1_6
};

enum class ApplicationLanguage
{
  ENGLISH,
  FRENCH,
  CHINESE_TRADITIONAL
};

enum class GUITheme
{
  WINDOWS_VISTA,
  MITSURIOU_BLACK_THEME,
  MITSURIOU_DARK_THEME,
  MITSURIOU_LIGHT_THEME,
  ALEXHUSZAGH_BREEZE_DARK,
  ALEXHUSZAGH_BREEZE_LIGHT,
  PAPER_DARK,
  PAPER_LIGHT,
  PAPER_BLACK_MONO,
  PAPER_WHITE_MONO
};

enum class WindowOpeningMode
{
  MINIMIZED,
  WINDOWED,
  MAXIMIZED
};

enum class DialogOpeningMode
{
  WINDOWED,
  MAXIMIZED
};

enum class StartupAction
{
  OPEN_WELCOME_SCREEN,
  CHECK_FOR_UPDATES,
  SKIP_UPDATE_CHECKS
};

enum class AssistedConversionRole
{
  IGNORED,
  BODY,
  FEET,
  HANDS,
  SKELETON
};

enum class ButtonClicked
{
  YES,
  NO,
  CLOSE_WINDOW,
  OTHER
};

enum class BodyPartType
{
  BODY,
  FEET,
  HANDS,
  BEAST_HANDS
};

enum class ApplicationVersionRelative
{
  OLDER,
  EQUIVALENT,
  NEWER
};

enum class BCGroupWidgetCallContext
{
  UNDEFINED,
  BODY,
  FEET,
  HANDS,
  HEAD,
  EYES,
  SKELETON
};
