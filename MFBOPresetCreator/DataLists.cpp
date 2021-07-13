#include "DataLists.h"

QStringList DataLists::getBodiesNames()
{
  return QStringList{
    QString("CBBE 3BBB 3BA - by Acro"),
    QString("CBBE SMP (3BBB) - by Ousnius"),
    QString("BHUNP 3BBB - by Bakafactory and Haeun"),
    QString("BHUNP 3BBB Advanced - by Bakafactory and Haeun"),
    QString("BHUNP 3BBB Advanced Ver 2 - by Bakafactory and Haeun"),
    QString("BHUNP BBP - by Bakafactory and Haeun"),
    QString("BHUNP BBP Advanced - by Bakafactory and Haeun"),
    QString("BHUNP TBBP - by Bakafactory and Haeun"),
    QString("BHUNP TBBP Advanced - by Bakafactory and Haeun"),
    QString("BHUNP 3BBB Advanced Ver 2 (Nevernude) - by Bakafactory and Haeun"),
    QString("Mimir Ebonic Body - by Medley")};
}

QStringList DataLists::getVersionsFromBodyName(const BodyName& aBody)
{
  QStringList lBodies;

  switch (aBody)
  {
    case BodyName::CBBE_3BBB_3BA:
      lBodies = QStringList({QString("1.50"), QString("1.51 to 1.55"), QString("2.00 to 2.04"), QString("2.05 to 2.06")});
      break;
    case BodyName::CBBE_SMP_3BBB:
      lBodies.append(QString("1.2.0"));
      break;
    case BodyName::BHUNP_3BBB:
    case BodyName::BHUNP_3BBB_ADVANCED:
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyName::BHUNP_BBP:
    case BodyName::BHUNP_BBP_ADVANCED:
    case BodyName::BHUNP_TBBP:
    case BodyName::BHUNP_TBBP_ADVANCED:
      lBodies = QStringList({QString("2.13"), QString("2.15"), QString("2.20"), QString("2.25"), QString("2.30"), QString("2.31"), QString("2.35 & 2.36")});
      break;
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      lBodies = QStringList({QString("2.25"), QString("2.30")});
      break;
    case BodyName::MIMIR_EBONIC_BODY:
      lBodies.append(QString("1.2"));
      break;
  }

  return lBodies;
}

BodyNameVersion DataLists::getBodyNameVersion(const BodyName& aBody, const int aRelativeVersion)
{
  // Calculate an offset since the bodies versions are not sorted
  auto lOffset{0};

  auto lBody{static_cast<int>(aBody)};

  // "CBBE 3BBB 3BA 2.00 -> 2.04" == 32, "CBBE 3BBB 3BA 2.05 -> 2.06" == 33
  if (lBody == 0 && (aRelativeVersion == 3 || aRelativeVersion == 4))
  {
    lOffset = 29;
  }
  // BHUNP v2.30
  else if ((lBody >= 2 && lBody <= 8) && (aRelativeVersion == 4))
  {
    lOffset = 2;
  }
  else if ((lBody >= 2 && lBody <= 8) && (aRelativeVersion == 5 || aRelativeVersion == 6))
  {
    lOffset = 4;
  }

  switch (aBody)
  {
    case BodyName::CBBE_3BBB_3BA:
      return static_cast<BodyNameVersion>(aRelativeVersion + lOffset);
    case BodyName::CBBE_SMP_3BBB:
      return static_cast<BodyNameVersion>(3);
    case BodyName::BHUNP_3BBB:
      return static_cast<BodyNameVersion>(4 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_3BBB_ADVANCED:
      return static_cast<BodyNameVersion>(5 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2:
      return static_cast<BodyNameVersion>(6 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_BBP:
      return static_cast<BodyNameVersion>(7 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_BBP_ADVANCED:
      return static_cast<BodyNameVersion>(8 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_TBBP:
      return static_cast<BodyNameVersion>(9 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_TBBP_ADVANCED:
      return static_cast<BodyNameVersion>(10 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return static_cast<BodyNameVersion>(41 + aRelativeVersion);
    case BodyName::MIMIR_EBONIC_BODY:
      return static_cast<BodyNameVersion>(57 + aRelativeVersion);
  }

  return BodyNameVersion::CBBE_3BBB_3BA_1_50;
}

std::pair<int, int> DataLists::getSplittedNameVersionFromBodyVersion(BodyNameVersion aBodyVersion)
{
  switch (aBodyVersion)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
      return std::pair<int, int>(0, static_cast<int>(aBodyVersion));
    case BodyNameVersion::CBBE_3BBB_3BA_2_00_TO_2_04:
      return std::pair<int, int>(0, 3);
    case BodyNameVersion::CBBE_3BBB_3BA_2_05_TO_2_06:
      return std::pair<int, int>(0, 4);
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return std::pair<int, int>(1, 0);
    case BodyNameVersion::BHUNP_3BBB_2_13:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_13:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_13:
    case BodyNameVersion::BHUNP_BBP_2_13:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_13:
    case BodyNameVersion::BHUNP_TBBP_2_13:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_13:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 2, 0);
    case BodyNameVersion::BHUNP_3BBB_2_15:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_15:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_15:
    case BodyNameVersion::BHUNP_BBP_2_15:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_15:
    case BodyNameVersion::BHUNP_TBBP_2_15:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_15:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 9, 1);
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 16, 2);
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 23, 3);
    case BodyNameVersion::BHUNP_3BBB_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case BodyNameVersion::BHUNP_BBP_2_30:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_TBBP_2_30:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 32, 4);
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
      return std::pair<int, int>(9, static_cast<int>(aBodyVersion) - 41);
    case BodyNameVersion::BHUNP_3BBB_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case BodyNameVersion::BHUNP_BBP_2_31:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_TBBP_2_31:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 41, 5);
    case BodyNameVersion::BHUNP_3BBB_2_35:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35:
    case BodyNameVersion::BHUNP_BBP_2_35:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35:
    case BodyNameVersion::BHUNP_TBBP_2_35:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 48, 6);
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
      return std::pair<int, int>(10, static_cast<int>(aBodyVersion));
  }

  return std::pair<int, int>(-1, -1);
}

QString DataLists::getQRCPathFromBodyName(const BodyNameVersion& aBody, const QString& aRessourceType)
{
  auto lPath{QString(":/presets/")};
  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
      lPath.append("cbbe 3bbb 3ba/1.50");
      break;
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
      lPath.append("cbbe 3bbb 3ba/1.51_to_1.55");
      break;
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      lPath.append("cbbe smp 3bbb/1.2.0");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_13:
      lPath.append("bhunp 3bbb/2.13");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_13:
      lPath.append("bhunp 3bbb advanced/2.13");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_13:
      lPath.append("bhunp 3bbb advanced ver 2/2.13");
      break;
    case BodyNameVersion::BHUNP_BBP_2_13:
      lPath.append("bhunp bbp/2.13");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_13:
      lPath.append("bhunp bbp advanced/2.13");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_13:
      lPath.append("bhunp tbbp/2.13");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_13:
      lPath.append("bhunp tbbp advanced/2.13");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_15:
      lPath.append("bhunp 3bbb/2.15");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_15:
      lPath.append("bhunp 3bbb advanced/2.15");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_15:
      lPath.append("bhunp 3bbb advanced ver 2/2.15");
      break;
    case BodyNameVersion::BHUNP_BBP_2_15:
      lPath.append("bhunp bbp/2.15");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_15:
      lPath.append("bhunp bbp advanced/2.15");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_15:
      lPath.append("bhunp tbbp/2.15");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_15:
      lPath.append("bhunp tbbp advanced/2.15");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_20:
      lPath.append("bhunp 3bbb/2.20");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
      lPath.append("bhunp 3bbb advanced/2.20");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
      lPath.append("bhunp 3bbb advanced ver 2/2.20");
      break;
    case BodyNameVersion::BHUNP_BBP_2_20:
      lPath.append("bhunp bbp/2.20");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
      lPath.append("bhunp bbp advanced/2.20");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_20:
      lPath.append("bhunp tbbp/2.20");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
      lPath.append("bhunp tbbp advanced/2.20");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_25:
      lPath.append("bhunp 3bbb/2.25");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
      lPath.append("bhunp 3bbb advanced/2.25");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
      lPath.append("bhunp 3bbb advanced ver 2/2.25");
      break;
    case BodyNameVersion::BHUNP_BBP_2_25:
      lPath.append("bhunp bbp/2.25");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
      lPath.append("bhunp bbp advanced/2.25");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_25:
      lPath.append("bhunp tbbp/2.25");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
      lPath.append("bhunp tbbp advanced/2.25");
      break;
    case BodyNameVersion::CBBE_3BBB_3BA_2_00_TO_2_04:
      lPath.append("cbbe 3bbb 3ba/2.00_to_2.04");
      break;
    case BodyNameVersion::CBBE_3BBB_3BA_2_05_TO_2_06:
      lPath.append("cbbe 3bbb 3ba/2.05_to_2.06");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_30:
      lPath.append("bhunp 3bbb/2.30");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
      lPath.append("bhunp 3bbb advanced/2.30");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
      lPath.append("bhunp 3bbb advanced ver 2/2.30");
      break;
    case BodyNameVersion::BHUNP_BBP_2_30:
      lPath.append("bhunp bbp/2.30");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
      lPath.append("bhunp bbp advanced/2.30");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_30:
      lPath.append("bhunp tbbp/2.30");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
      lPath.append("bhunp tbbp advanced/2.30");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
      lPath.append("bhunp 3bbb advanced ver 2 nevernude/2.25");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
      lPath.append("bhunp 3bbb advanced ver 2 nevernude/2.30");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_31:
      lPath.append("bhunp 3bbb/2.31");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
      lPath.append("bhunp 3bbb advanced/2.31");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
      lPath.append("bhunp 3bbb advanced ver 2/2.31");
      break;
    case BodyNameVersion::BHUNP_BBP_2_31:
      lPath.append("bhunp bbp/2.31");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
      lPath.append("bhunp bbp advanced/2.31");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_31:
      lPath.append("bhunp tbbp/2.31");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
      lPath.append("bhunp tbbp advanced/2.31");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_35:
      lPath.append("bhunp 3bbb/2.35");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35:
      lPath.append("bhunp 3bbb advanced/2.35");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35:
      lPath.append("bhunp 3bbb advanced ver 2/2.35");
      break;
    case BodyNameVersion::BHUNP_BBP_2_35:
      lPath.append("bhunp bbp/2.35");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35:
      lPath.append("bhunp bbp advanced/2.35");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_35:
      lPath.append("bhunp tbbp/2.35");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35:
      lPath.append("bhunp tbbp advanced/2.35");
      break;
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
      lPath.append("mimir ebonic body/1.2");
      break;
  }

  lPath.append("/");
  lPath.append(aRessourceType);

  return lPath;
}

QStringList DataLists::getFeetModsEntries()
{
  return QStringList{
    QString("Default feet sliders"),
    QString("More Sliders for Feet - Normal"),
    QString("More Sliders for Feet - High Heels")};
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
  QStringList lLanguages{
    QString("English"),
    QString("Français"),
    QString("Traditional Chinese (Taiwan) - translation by Sephirothryan")};

  return lLanguages;
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
    "batchConversionInput",
    "batchConversionOutput",
    "assistedConversionInput",
    "retargetingToolInput",
    "retargetingToolOutput",
    "texturesAssistantInput",
    "lastLoadedProject",
    "lastSavedProject"};
}
