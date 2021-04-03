#include "DataLists.h"

QStringList DataLists::getBodiesNames()
{
  return QStringList{
    QString("CBBE 3BBB 3BA"),
    QString("CBBE SMP (3BBB)"),
    QString("BHUNP 3BBB"),
    QString("BHUNP 3BBB Advanced"),
    QString("BHUNP 3BBB Advanced ver 2"),
    QString("BHUNP BBP"),
    QString("BHUNP BBP Advanced"),
    QString("BHUNP TBBP"),
    QString("BHUNP TBBP Advanced")};
}

QStringList DataLists::getVersionsFromBodyName(BodyName aBody)
{
  QStringList lBodies;

  switch (aBody)
  {
    case BodyName::CBBE_3BBB_3BA:
      lBodies.append(QString("1.40"));
      lBodies.append(QString("1.50"));
      lBodies.append(QString("1.51 -> 1.55"));
      lBodies.append(QString("2.00 -> 2.04"));
      lBodies.append(QString("2.05"));
      break;
    case BodyName::CBBE_SMP_3BBB:
      lBodies.append(QString("1.2.0"));
      break;
    case BodyName::BHUNP_3BBB:
    case BodyName::BHUNP_3BBB_Advanced:
    case BodyName::BHUNP_3BBB_Advanced_ver_2:
    case BodyName::BHUNP_BBP:
    case BodyName::BHUNP_BBP_Advanced:
    case BodyName::BHUNP_TBBP:
    case BodyName::BHUNP_TBBP_Advanced:
      lBodies.append(QString("2.13"));
      lBodies.append(QString("2.15"));
      lBodies.append(QString("2.20"));
      lBodies.append(QString("2.25"));
  }

  return lBodies;
}

BodyNameVersion DataLists::getBodyNameVersion(BodyName aBody, int aRelativeVersion)
{
  switch (aBody)
  {
    case BodyName::CBBE_3BBB_3BA:
      return static_cast<BodyNameVersion>(aRelativeVersion <= 2 ? aRelativeVersion : aRelativeVersion + 29); // "2.00 -> 2.04" == 32, "2.05" == 33
    case BodyName::CBBE_SMP_3BBB:
      return static_cast<BodyNameVersion>(3);
    case BodyName::BHUNP_3BBB:
      return static_cast<BodyNameVersion>(4 + 7 * aRelativeVersion);
    case BodyName::BHUNP_3BBB_Advanced:
      return static_cast<BodyNameVersion>(5 + 7 * aRelativeVersion);
    case BodyName::BHUNP_3BBB_Advanced_ver_2:
      return static_cast<BodyNameVersion>(6 + 7 * aRelativeVersion);
    case BodyName::BHUNP_BBP:
      return static_cast<BodyNameVersion>(7 + 7 * aRelativeVersion);
    case BodyName::BHUNP_BBP_Advanced:
      return static_cast<BodyNameVersion>(8 + 7 * aRelativeVersion);
    case BodyName::BHUNP_TBBP:
      return static_cast<BodyNameVersion>(9 + 7 * aRelativeVersion);
    case BodyName::BHUNP_TBBP_Advanced:
      return static_cast<BodyNameVersion>(10 + 7 * aRelativeVersion);
  }

  return BodyNameVersion::CBBE_3BBB_3BA_1_40;
}

std::pair<int, int> DataLists::getSplittedNameVersionFromBodyVersion(BodyNameVersion aBodyVersion)
{
  switch (aBodyVersion)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_40:
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_to_1_55:
      return std::pair<int, int>(0, static_cast<int>(aBodyVersion));
    case BodyNameVersion::CBBE_3BBB_3BA_2_00_to_2_04:
      return std::pair<int, int>(0, 3);
    case BodyNameVersion::CBBE_3BBB_3BA_2_05:
      return std::pair<int, int>(0, 4);
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return std::pair<int, int>(1, 0);
    case BodyNameVersion::BHUNP_3BBB_2_13:
      return std::pair<int, int>(2, 0);
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
    case BodyNameVersion::BHUNP_BBP_2_13:
    case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
    case BodyNameVersion::BHUNP_TBBP_2_13:
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 2, 0);
    case BodyNameVersion::BHUNP_3BBB_2_15:
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_15:
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_15:
    case BodyNameVersion::BHUNP_BBP_2_15:
    case BodyNameVersion::BHUNP_BBP_Advanced_2_15:
    case BodyNameVersion::BHUNP_TBBP_2_15:
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_15:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 9, 1);
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_20:
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_20:
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_Advanced_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_20:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 16, 2);
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_25:
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_25:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_Advanced_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_25:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 23, 3);
  }

  return std::pair<int, int>(-1, -1);
}

QStringList DataLists::getWindowOpeningModes()
{
  return QStringList{
    tr("Minimized"),
    tr("Windowed"),
    tr("Maximized")};
}

QStringList DataLists::getAppThemes()
{
  return QStringList{
    QString("Windows Vista"),
    QString("Paper Light by 6788"),
    QString("Paper Dark by 6788"),
    QString("Paper White Mono"),
    QString("Paper Black Mono"),
    QString("Alexhuszagh's Breeze Light"),
    QString("Alexhuszagh's Breeze Dark"),
    QString("QuasarApp's Dark Style"),
    QString("QuasarApp's Visual Studio Dark"),
    QString("Mitsuriou's Light Theme"),
    QString("Mitsuriou's Dark Theme")};
}

QStringList DataLists::getLanguages()
{
  return QStringList{
    QString("English"),
    QString("Français")};
}

QStringList DataLists::getAssistedConversionActions()
{
  return QStringList{
    tr("[ignore]"),
    tr("Body mesh"),
    tr("Feet mesh"),
    tr("Hands mesh"),
    tr("Skeleton mesh")};
}

QStringList DataLists::getLastPathsKeys()
{
  return QStringList{
    "general",
    "mainWindowOutput",
    "assistedConversionInput",
    "retargetingToolInput",
    "retargetingToolOutput",
    "texturesAssistantInput"};
}
