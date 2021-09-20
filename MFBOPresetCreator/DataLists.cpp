#include "DataLists.h"
#include "Utils.h"

QStringList DataLists::GetBodiesNames()
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

QStringList DataLists::GetVersionsFromBodyName(const BodyName& aBody)
{
  QStringList lBodies;

  switch (aBody)
  {
    case BodyName::CBBE_3BBB_3BA:
      lBodies = QStringList({QString("1.50"), QString("1.51 to 1.55"), QString("2.02 to 2.04"), QString("2.06")});
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
      lBodies = QStringList({QString("2.20"), QString("2.25"), QString("2.30"), QString("2.31"), QString("2.35 to 2.38")});
      break;
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      lBodies = QStringList({QString("2.25"), QString("2.30")});
      break;
    case BodyName::MIMIR_EBONIC_BODY:
      lBodies = QStringList({QString("1.2"), QString("1.2 foot fix")});
      break;
  }

  return lBodies;
}

BodyNameVersion DataLists::GetBodyNameVersion(const BodyName& aBody, const int aRelativeVersion)
{
  // Ignore the cases where the checkboxes have no index selected
  if (aRelativeVersion == -1)
  {
    return BodyNameVersion::INVALID_VALUE;
  }

  // Calculate an offset since the bodies versions are not sorted
  auto lOffset{0};

  auto lBody{static_cast<int>(aBody)};

  // "2.02 to 2.04" & "2.06"
  if (lBody == 0 && aRelativeVersion > 1)
  {
    lOffset = 15;
  }
  // BHUNP v2.30
  else if ((lBody >= 2 && lBody <= 8) && aRelativeVersion == 2)
  {
    lOffset = 2;
  }
  else if ((lBody >= 2 && lBody <= 8) && aRelativeVersion > 2)
  {
    lOffset = 4;
  }

  switch (aBody)
  {
    case BodyName::CBBE_3BBB_3BA:
      return static_cast<BodyNameVersion>(aRelativeVersion + lOffset);
    case BodyName::CBBE_SMP_3BBB:
      return static_cast<BodyNameVersion>(2);
    case BodyName::BHUNP_3BBB:
      return static_cast<BodyNameVersion>(3 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_3BBB_ADVANCED:
      return static_cast<BodyNameVersion>(4 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2:
      return static_cast<BodyNameVersion>(5 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_BBP:
      return static_cast<BodyNameVersion>(6 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_BBP_ADVANCED:
      return static_cast<BodyNameVersion>(7 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_TBBP:
      return static_cast<BodyNameVersion>(8 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_TBBP_ADVANCED:
      return static_cast<BodyNameVersion>(9 + 7 * aRelativeVersion + lOffset);
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return static_cast<BodyNameVersion>(26 + aRelativeVersion);
    case BodyName::MIMIR_EBONIC_BODY:
      return static_cast<BodyNameVersion>(42 + aRelativeVersion);
  }

  return BodyNameVersion::INVALID_VALUE;
}

std::pair<int, int> DataLists::GetSplittedNameVersionFromBodyVersion(BodyNameVersion aBodyVersion)
{
  switch (aBodyVersion)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
      return std::pair<int, int>(static_cast<int>(BodyName::CBBE_3BBB_3BA), static_cast<int>(aBodyVersion));
    case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
      return std::pair<int, int>(static_cast<int>(BodyName::CBBE_3BBB_3BA), 2);
    case BodyNameVersion::CBBE_3BBB_3BA_2_06:
      return std::pair<int, int>(static_cast<int>(BodyName::CBBE_3BBB_3BA), 3);
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return std::pair<int, int>(static_cast<int>(BodyName::CBBE_SMP_3BBB), 0);
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 1, 0);
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 8, 1);
    case BodyNameVersion::BHUNP_3BBB_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case BodyNameVersion::BHUNP_BBP_2_30:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_TBBP_2_30:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 17, 2);
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
      return std::pair<int, int>(static_cast<int>(BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE), static_cast<int>(aBodyVersion) - 26);
    case BodyNameVersion::BHUNP_3BBB_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case BodyNameVersion::BHUNP_BBP_2_31:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_TBBP_2_31:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 26, 3);
    case BodyNameVersion::BHUNP_3BBB_2_35:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35:
    case BodyNameVersion::BHUNP_BBP_2_35:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35:
    case BodyNameVersion::BHUNP_TBBP_2_35:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 33, 4);
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return std::pair<int, int>(static_cast<int>(BodyName::MIMIR_EBONIC_BODY), static_cast<int>(aBodyVersion) - 42);
  }

  return std::pair<int, int>(-1, -1);
}

QString DataLists::GetQRCPathFromBodyName(const BodyNameVersion& aBody, const int aFeetModIndex, const BodyPartType& aRessourceType)
{
  // Ressource type
  auto lRessource{QString()};
  switch (aRessourceType)
  {
    case BodyPartType::BODY:
      lRessource = "body";
      break;
    case BodyPartType::FEET:
      lRessource = "feet";
      break;
    case BodyPartType::HANDS:
      lRessource = "hands";
      break;
  }

  // Body mod name
  auto lBodyVersion{DataLists::GetSplittedNameVersionFromBodyVersion(aBody)};
  auto lCastedBodyName{static_cast<BodyName>(lBodyVersion.first)};
  auto lResolvedModName{QString()};

  // CBBE-based bodies
  if (Utils::IsCBBEBasedBody(aBody))
  {
    // For the beast hands, use the default CBBE v.1.6.1 beast hands
    if (aRessourceType == BodyPartType::BEAST_HANDS)
    {
      return ":/presets/beast_hands/cbbe 1.6.1";
    }

    if (aRessourceType == BodyPartType::FEET)
    {
      // Do not handle case 0 below (default feet, it is following the normal workflow)
      switch (aFeetModIndex)
      {
        case 1:
          // More Sliders for Feet - Normal
          return ":/presets/feet/msf/normal/cbbe/1.0"; // TODO: Handle the feet mods version numbers
        case 2:
          // More Sliders for Feet - High Heels
          return ":/presets/feet/msf/high heels/cbbe/1.0"; // TODO: Handle the feet mods version numbers
      }
    }

    switch (lCastedBodyName)
    {
      case BodyName::CBBE_3BBB_3BA:
        lResolvedModName = "cbbe 3bbb 3ba";
        break;
      case BodyName::CBBE_SMP_3BBB:
        lResolvedModName = "cbbe smp 3bbb";
        break;
      case BodyName::MIMIR_EBONIC_BODY:
        lResolvedModName = "mimir ebonic body";
        break;
    }
  }
  // UNP-based bodies
  else
  {
    switch (aRessourceType)
    {
      case BodyPartType::BODY:
        switch (lCastedBodyName)
        {
          case BodyName::BHUNP_3BBB:
            lResolvedModName = "bhunp 3bbb";
            break;
          case BodyName::BHUNP_3BBB_ADVANCED:
            lResolvedModName = "bhunp 3bbb advanced";
            break;
          case BodyName::BHUNP_3BBB_ADVANCED_VER_2:
            lResolvedModName = "bhunp 3bbb advanced ver 2";
            break;
          case BodyName::BHUNP_BBP:
            lResolvedModName = "bhunp bbp";
            break;
          case BodyName::BHUNP_BBP_ADVANCED:
            lResolvedModName = "bhunp bbp advanced";
            break;
          case BodyName::BHUNP_TBBP:
            lResolvedModName = "bhunp tbbp";
            break;
          case BodyName::BHUNP_TBBP_ADVANCED:
            lResolvedModName = "bhunp tbbp advanced";
            break;
          case BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
            lResolvedModName = "bhunp 3bbb advanced ver 2 nevernude";
            break;
        }
        break;
      case BodyPartType::FEET:
        if (aRessourceType == BodyPartType::FEET)
        {
          switch (aFeetModIndex)
          {
            case 0:
              // Default: follow the normal workflow to use the default BHUNP feet
              lResolvedModName = "bhunp";
              break;
            case 1:
              // More Sliders for Feet - Normal
              return ":/presets/feet/msf/normal/bhunp/1.1"; // TODO: Handle the feet mods version numbers
            case 2:
              // More Sliders for Feet - High Heels
              return ":/presets/feet/msf/high heels/bhunp/1.1"; // TODO: Handle the feet mods version numbers
            case 3:
              // HG Feet and Toes BHUNP SE - HGFeet UUNP
              return ":/presets/feet/hg feet/bhunp"; // TODO: Handle the feet mods version numbers
            case 4:
              // Khrysamere HG Feet (BHUNP)
              return ":/presets/feet/khrysamere hg feet/normal/1.6.1"; // TODO: Handle the feet mods version numbers
            case 5:
              // Khrysamere HG Feet (Claws) (BHUNP)
              return ":/presets/feet/khrysamere hg feet/claws/1.6.1"; // TODO: Handle the feet mods version numbers
          }
        }
#if __cplusplus >= 201703L // C++17 and newer
        [[fallthrough]];
#endif
      case BodyPartType::HANDS:
        // Use the default BHUNP hands
        lResolvedModName = "bhunp";
        break;
      default:
        return "";
    }
  }

  return QString(":/presets/%1/%2/%3").arg(lRessource).arg(lResolvedModName).arg(DataLists::GetVersionsFromBodyName(lCastedBodyName).at(lBodyVersion.second));
}

QStringList DataLists::GetFeetModsFromBodyName(const BodyName& aBody)
{
  // Common entries
  auto lEntries{QStringList{
    QString("Default feet sliders"),
    QString("More Sliders for Feet - Normal - by Balveric"),
    QString("More Sliders for Feet - High Heels - by Balveric")}};

  // CBBE-based bodies
  if (Utils::IsCBBEBasedBody(aBody))
  {
    // Fill this block if any feet mod has to be here
  }
  // UNP-based bodies
  else
  {
    lEntries.append("HG Feet and Toes - by SunJeong");
    lEntries.append("HG Feet - by Khrysamere");
    lEntries.append("HG Feet (Claws) - by Khrysamere");
  }

  return lEntries;
}

QStringList DataLists::GetWindowOpeningModes()
{
  return QStringList{
    tr("Minimized"),
    tr("Windowed"),
    tr("Maximized")};
}

QStringList DataLists::GetDialogOpeningModes()
{
  return QStringList{
    tr("Windowed"),
    tr("Maximized")};
}

QStringList DataLists::GetAppThemes()
{
  return QStringList{
    QString("Windows Vista"),
    QString("Mitsuriou's Black Theme"),
    QString("Mitsuriou's Dark Theme"),
    QString("Mitsuriou's Light Theme"),
    QString("Alexhuszagh's Breeze Dark"),
    QString("Alexhuszagh's Breeze Light"),
    QString("Paper Dark by 6788"),
    QString("Paper Light by 6788"),
    QString("Paper Black Mono by 6788"),
    QString("Paper White Mono by 6788")};
}

QStringList DataLists::GetLanguages()
{
  QStringList lLanguages{
    QString("English"),
    QString("Français"),
    QString("繁体中文(台湾) - translation by Sephirothryan")};

  return lLanguages;
}

QStringList DataLists::GetAssistedConversionActions()
{
  return QStringList{
    tr("[ignore]"),
    tr("Body mesh"),
    tr("Feet mesh"),
    tr("Hands mesh"),
    tr("Skeleton mesh")};
}

QStringList DataLists::GetLastPathsKeys()
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
