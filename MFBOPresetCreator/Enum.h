﻿#pragma once

enum class BodyName
{
  _INVALID_VALUE = -1,
  CBBE_3BA_3BBB,
  BHUNP_UUNP_NEXT_GENERATION,
  CBBE_SMP_3BBB,
  COCO_BODY_CBBE,
  COCO_BODY_UUNP,
  MIMIR_EBONIC_BODY,
  BHUNP_LITE_ASDASDF
};

enum class BodyVariant
{
  _INVALID_VALUE = -1,
  // CBBE 3BA 3BBB
  CBBE_3BA_3BBB,
  CBBE_3BA_3BBB_NEVERNUDE,
  CBBE_3BA_3BBB_UNDERWEAR,
  // BHUNP
  BHUNP_3BBB,
  BHUNP_3BBB_ADVANCED,
  BHUNP_3BBB_ADVANCED_VER_2,
  BHUNP_3BBB_ADVANCED_VER_3,
  BHUNP_BBP,
  BHUNP_BBP_ADVANCED,
  BHUNP_TBBP,
  BHUNP_TBBP_ADVANCED,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE,
  // CBBE SMP 3BBB
  CBBE_SMP_3BBB,
  // COCO BODY
  COCO_BODY_CBBE,
  COCO_BODY_UUNP,
  // MIMIR EBONIC BODY
  MIMIR_EBONIC_BODY,
  // ASDASDF
  ASDASDF_3BBB,
  ASDASDF_3BBB_NO_THIGH_JIGGLE,
  ASDASDF_BBP,
  ASDASDF_BBP_NO_THIGH_JIGGLE,
  ASDASDF_TBBP,
  ASDASDF_TBBP_NO_THIGH_JIGGLE,
  ASDASDF_ALTERBODY_3BBB,
  ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE,
  ASDASDF_ALTERBODY_BBP,
  ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE,
  ASDASDF_ALTERBODY_TBBP,
  ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE,
};

enum class BodyNameVersion
{
  _INVALID_VALUE = -1,
  // CBBE_3BA_3BBB
  CBBE_3BA_3BBB_1_50,
  CBBE_3BA_3BBB_1_51_TO_1_55,
  CBBE_3BA_3BBB_2_02_TO_2_04,
  CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12,
  CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31,
  CBBE_3BA_3BBB_2_40_TO_2_42,
  // CBBE_3BA_3BBB_NEVERNUDE
  CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_18_AND_2_30_AND_2_31,
  CBBE_3BA_3BBB_NEVERNUDE_2_40_TO_2_42,
  // CBBE_3BA_3BBB_UNDERWEAR
  CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_18_AND_2_30_AND_2_31,
  CBBE_3BA_3BBB_UNDERWEAR_2_40_TO_2_42,
  // BHUNP_3BBB
  BHUNP_3BBB_2_20,
  BHUNP_3BBB_2_25,
  BHUNP_3BBB_2_30,
  BHUNP_3BBB_2_31,
  BHUNP_3BBB_2_35_TO_2_42,
  BHUNP_3BBB_3_00_TO_3_02,
  BHUNP_3BBB_3_03,
  BHUNP_3BBB_3_04,
  BHUNP_3BBB_3_10,
  BHUNP_3BBB_3_11_TO_3_13,
  // BHUNP_3BBB_ADVANCED
  BHUNP_3BBB_ADVANCED_2_20,
  BHUNP_3BBB_ADVANCED_2_25,
  BHUNP_3BBB_ADVANCED_2_30,
  BHUNP_3BBB_ADVANCED_2_31,
  BHUNP_3BBB_ADVANCED_2_35_TO_2_42,
  BHUNP_3BBB_ADVANCED_3_00_TO_3_02,
  BHUNP_3BBB_ADVANCED_3_03,
  BHUNP_3BBB_ADVANCED_3_04,
  BHUNP_3BBB_ADVANCED_3_10,
  BHUNP_3BBB_ADVANCED_3_11_TO_3_13,
  // BHUNP_3BBB_ADVANCED_VER_2
  BHUNP_3BBB_ADVANCED_VER_2_2_20,
  BHUNP_3BBB_ADVANCED_VER_2_2_25,
  BHUNP_3BBB_ADVANCED_VER_2_2_30,
  BHUNP_3BBB_ADVANCED_VER_2_2_31,
  BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42,
  BHUNP_3BBB_ADVANCED_VER_2_3_00_TO_3_02,
  BHUNP_3BBB_ADVANCED_VER_2_3_03,
  BHUNP_3BBB_ADVANCED_VER_2_3_04,
  BHUNP_3BBB_ADVANCED_VER_2_3_10,
  BHUNP_3BBB_ADVANCED_VER_2_3_11_TO_3_13,
  // BHUNP_3BBB_ADVANCED_VER_3
  BHUNP_3BBB_ADVANCED_VER_3_3_00_TO_3_02,
  BHUNP_3BBB_ADVANCED_VER_3_3_03,
  BHUNP_3BBB_ADVANCED_VER_3_3_04,
  BHUNP_3BBB_ADVANCED_VER_3_3_10,
  BHUNP_3BBB_ADVANCED_VER_3_3_11_TO_3_13,
  // BHUNP_BBP
  BHUNP_BBP_2_20,
  BHUNP_BBP_2_25,
  BHUNP_BBP_2_30,
  BHUNP_BBP_2_31,
  BHUNP_BBP_2_35_TO_2_42,
  BHUNP_BBP_3_00_TO_3_02,
  BHUNP_BBP_3_03,
  BHUNP_BBP_3_04,
  BHUNP_BBP_3_10,
  BHUNP_BBP_3_11_TO_3_13,
  // BHUNP_BBP_ADVANCED
  BHUNP_BBP_ADVANCED_2_20,
  BHUNP_BBP_ADVANCED_2_25,
  BHUNP_BBP_ADVANCED_2_30,
  BHUNP_BBP_ADVANCED_2_31,
  BHUNP_BBP_ADVANCED_2_35_TO_2_42,
  BHUNP_BBP_ADVANCED_3_00_TO_3_02,
  BHUNP_BBP_ADVANCED_3_03,
  BHUNP_BBP_ADVANCED_3_04,
  BHUNP_BBP_ADVANCED_3_10,
  BHUNP_BBP_ADVANCED_3_11_TO_3_13,
  // BHUNP_TBBP
  BHUNP_TBBP_2_20,
  BHUNP_TBBP_2_25,
  BHUNP_TBBP_2_30,
  BHUNP_TBBP_2_31,
  BHUNP_TBBP_2_35_TO_2_42,
  BHUNP_TBBP_3_00_TO_3_02,
  BHUNP_TBBP_3_03,
  BHUNP_TBBP_3_04,
  BHUNP_TBBP_3_10,
  BHUNP_TBBP_3_11_TO_3_13,
  // BHUNP_TBBP_ADVANCED
  BHUNP_TBBP_ADVANCED_2_20,
  BHUNP_TBBP_ADVANCED_2_25,
  BHUNP_TBBP_ADVANCED_2_30,
  BHUNP_TBBP_ADVANCED_2_31,
  BHUNP_TBBP_ADVANCED_2_35_TO_2_42,
  BHUNP_TBBP_ADVANCED_3_00_TO_3_02,
  BHUNP_TBBP_ADVANCED_3_03,
  BHUNP_TBBP_ADVANCED_3_04,
  BHUNP_TBBP_ADVANCED_3_10,
  BHUNP_TBBP_ADVANCED_3_11_TO_3_13,
  // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_35_TO_2_42,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_00_TO_3_02,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_03,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_04,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_10,
  BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_11_TO_3_13,
  // CBBE_SMP_3BBB
  CBBE_SMP_3BBB_1_2_0,
  // COCO_BODY_CBBE
  COCO_BODY_CBBE_V4,
  COCO_BODY_CBBE_V6,
  COCO_BODY_CBBE_V6_8,
  COCO_BODY_CBBE_V6_9_AND_V6_95,
  // COCO_BODY_UUNP
  COCO_BODY_UUNP_V3,
  COCO_BODY_UUNP_V4,
  COCO_BODY_UUNP_V6,
  COCO_BODY_UUNP_V6_8,
  COCO_BODY_UUNP_V6_9_AND_V6_95,
  // MIMIR_EBONIC_BODY
  MIMIR_EBONIC_BODY_1_2,
  MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX,
  // ASDASDF_3BBB
  ASDASDF_3BBB_1_13,
  // ASDASDF_3BBB_NO_THIGH_JIGGLE
  ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13,
  // ASDASDF_BBP
  ASDASDF_BBP_1_13,
  // ASDASDF_BBP_NO_THIGH_JIGGLE
  ASDASDF_BBP_NO_THIGH_JIGGLE_1_13,
  // ASDASDF_TBBP
  ASDASDF_TBBP_1_13,
  // ASDASDF_TBBP_NO_THIGH_JIGGLE
  ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13,
  // ASDASDF_ALTERBODY_3BBB
  ASDASDF_ALTERBODY_3BBB_1_13,
  // ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE
  ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13,
  // ASDASDF_ALTERBODY_BBP
  ASDASDF_ALTERBODY_BBP_1_13,
  // ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE
  ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13,
  // ASDASDF_ALTERBODY_TBBP
  ASDASDF_ALTERBODY_TBBP_1_13,
  // ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE
  ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13,
};

enum class FeetName
{
  _INVALID_VALUE = -1,
  CBBE,
  BHUNP_3BBB_ADVANCED,
  CBBE_3BA_3BBB,
  COCO_BODY,
  MIMIR_EBONIC_BODY,
  ASDASDF,
  MORE_SLIDERS_FOR_FEET,
  HG_FEET,
  KHRYSAMERE_HG_FEET
};

enum class FeetVariant
{
  _INVALID_VALUE = -1,
  CBBE,
  BHUNP_3BBB_ADVANCED,
  CBBE_3BA_3BBB,
  COCO_BODY_NORMAL_CBBE,
  COCO_BODY_NORMAL_UUNP,
  COCO_BODY_HIGH_HEELS_CBBE,
  COCO_BODY_HIGH_HEELS_UUNP,
  MIMIR_EBONIC_BODY,
  ASDASDF,
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
  // BHUNP 3BBB Advanced Feet
  BHUNP_3BBB_ADVANCED_2_20,
  BHUNP_3BBB_ADVANCED_2_25,
  BHUNP_3BBB_ADVANCED_2_30,
  BHUNP_3BBB_ADVANCED_2_31,
  BHUNP_3BBB_ADVANCED_2_35_TO_2_42,
  BHUNP_3BBB_ADVANCED_3_00_TO_3_02,
  BHUNP_3BBB_ADVANCED_3_03,
  BHUNP_3BBB_ADVANCED_3_04,
  BHUNP_3BBB_ADVANCED_3_10,
  BHUNP_3BBB_ADVANCED_3_11_TO_3_13,
  // CBBE_3BA_3BBB
  CBBE_3BA_3BBB_1_50,
  CBBE_3BA_3BBB_1_51_TO_1_55,
  CBBE_3BA_3BBB_2_02_TO_2_04,
  CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12,
  CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31,
  CBBE_3BA_3BBB_2_40_TO_2_42,
  // COCO_BODY_CBBE
  COCO_BODY_NORMAL_CBBE_V4,
  COCO_BODY_NORMAL_CBBE_V6,
  COCO_BODY_NORMAL_CBBE_V6_8,
  COCO_BODY_NORMAL_CBBE_V6_9_AND_V6_95,
  // COCO_BODY_UUNP
  COCO_BODY_NORMAL_UUNP_V3,
  COCO_BODY_NORMAL_UUNP_V4,
  COCO_BODY_NORMAL_UUNP_V6,
  COCO_BODY_NORMAL_UUNP_V6_8,
  COCO_BODY_NORMAL_UUNP_V6_9_AND_V6_95,
  // COCO_BODY_CBBE
  COCO_BODY_HIGH_HEELS_CBBE_V6,
  COCO_BODY_HIGH_HEELS_CBBE_V6_8,
  COCO_BODY_HIGH_HEELS_CBBE_V6_9_AND_V6_95,
  // COCO_BODY_UUNP
  COCO_BODY_HIGH_HEELS_UUNP_V6,
  COCO_BODY_HIGH_HEELS_UUNP_V6_8,
  COCO_BODY_HIGH_HEELS_UUNP_V6_9_AND_V6_95,
  // MIMIR_EBONIC_BODY
  MIMIR_EBONIC_BODY_1_2,
  MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX,
  // ASDASDF
  ASDASDF_1_13,
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
  DEFAULT_OS_THEME,
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
