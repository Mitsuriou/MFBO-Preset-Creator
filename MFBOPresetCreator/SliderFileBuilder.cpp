#include "SliderFileBuilder.h"
#include "DataLists.h"
#include "Utils.h"
#include <QFile>
#include <QTextStream>

QString SliderFileBuilder::buildOSPFileContent(const QString& aLineName, const BodyNameVersion& aBody, const bool aMustUseBeastHands, const int aFeetModIndex, const unsigned char& aTargetBlocks)
{
  auto lBuiltContent{QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<SliderSetInfo version=\"1\">")};

  // Body
  if (aTargetBlocks / 100 == 1)
    lBuiltContent.append("\n").append(Utils::readQRCFileContent(DataLists::getQRCPathFromBodyName(aBody, aFeetModIndex, BodyPartType::BODY)).arg(aLineName));

  // Feet
  if (aTargetBlocks % 100 / 10 == 1)
    lBuiltContent.append("\n").append(
      Utils::readQRCFileContent(
        DataLists::getQRCPathFromBodyName(aBody, aFeetModIndex, BodyPartType::FEET))
        .arg(aLineName));

  // Hands
  if (aTargetBlocks % 10 == 1)
  {
    lBuiltContent.append("\n").append(
      Utils::readQRCFileContent(
        DataLists::getQRCPathFromBodyName(aBody, aFeetModIndex, aMustUseBeastHands ? BodyPartType::BEAST_HANDS : BodyPartType::HANDS))
        .arg(aLineName));
  }

  lBuiltContent.append("\n</SliderSetInfo>\n");
  return lBuiltContent;
}

QString SliderFileBuilder::buildXMLFileContent(const QString& aLineName,
                                               const std::vector<Struct::Filter>& aFiltersList,
                                               const BodyNameVersion& aBody,
                                               const bool aMustUseBeastHands,
                                               const int aFeetModIndex,
                                               const unsigned char& aTargetBlocks)
{
  auto lBuiltContent{QString("<SliderGroups>\n")};

  const auto& lFilters{aFiltersList.size() > 0 ? aFiltersList : SliderFileBuilder::getXMLDefaultFiltersFromBody(aBody)};

  for (const auto& lFilter : lFilters)
  {
    if (lFilter.getFilter().length() == 0)
    {
      continue;
    }

    lBuiltContent.append(QString("    <Group name=\"%1\">\n").arg(lFilter.getFilter()));

    switch (aBody)
    {
      case BodyNameVersion::CBBE_3BBB_3BA_1_50:
      case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
      case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
      case BodyNameVersion::CBBE_3BBB_3BA_2_06:
      case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - CBBE 3BBB Body Amazing\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          switch (aFeetModIndex)
          {
            case 0:
              // Default
              lBuiltContent.append("%1%1<Member name=\"%2 - CBBE 3BBB Feet\"/>\n");
              break;
            case 1:
              // More Sliders for Feet - Normal
              lBuiltContent.append("%1%1<Member name=\"%2 - Feet (MSF - normal)\"/>\n");
              break;
            case 2:
              // More Sliders for Feet - High Heels
              lBuiltContent.append("%1%1<Member name=\"%2 - Feet (MSF - HH)\"/>\n");
              break;
          }
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          if (aMustUseBeastHands)
          {
            lBuiltContent.append("%1%1<Member name=\"%2 - CBBE Hands Beast\"/>\n");
          }
          else
          {
            lBuiltContent.append("%1%1<Member name=\"%2 - CBBE 3BBB Hands\"/>\n");
          }
        }
        break;
      case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - CBBE Body SMP (3BBB)\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          switch (aFeetModIndex)
          {
            case 0:
              // Default
              lBuiltContent.append("%1%1<Member name=\"%2 - CBBE Feet\"/>\n");
              break;
            case 1:
              // More Sliders for Feet - Normal
              lBuiltContent.append("%1%1<Member name=\"%2 - Feet (MSF - normal)\"/>\n");
              break;
            case 2:
              // More Sliders for Feet - High Heels
              lBuiltContent.append("%1%1<Member name=\"%2 - Feet (MSF - HH)\"/>\n");
              break;
          }
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          if (aMustUseBeastHands)
          {
            lBuiltContent.append("%1%1<Member name=\"%2 - CBBE Hands Beast\"/>\n");
          }
          else
          {
            lBuiltContent.append("%1%1<Member name=\"%2 - CBBE Hands\"/>\n");
          }
        }
        break;
      case BodyNameVersion::BHUNP_3BBB_2_20:
      case BodyNameVersion::BHUNP_3BBB_2_25:
      case BodyNameVersion::BHUNP_3BBB_2_30:
      case BodyNameVersion::BHUNP_3BBB_2_31:
      case BodyNameVersion::BHUNP_3BBB_2_35:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Ver 2\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
      case BodyNameVersion::BHUNP_BBP_2_20:
      case BodyNameVersion::BHUNP_BBP_2_25:
      case BodyNameVersion::BHUNP_BBP_2_30:
      case BodyNameVersion::BHUNP_BBP_2_31:
      case BodyNameVersion::BHUNP_BBP_2_35:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP BBP\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
      case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
      case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
      case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
      case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
      case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP BBP Advanced\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
      case BodyNameVersion::BHUNP_TBBP_2_20:
      case BodyNameVersion::BHUNP_TBBP_2_25:
      case BodyNameVersion::BHUNP_TBBP_2_30:
      case BodyNameVersion::BHUNP_TBBP_2_31:
      case BodyNameVersion::BHUNP_TBBP_2_35:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP TBBP\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
      case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
      case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
      case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
      case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
      case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP TBBP Advanced\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
      case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
        // Body
        if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Ver 2 Nevernude\"/>\n");
        }

        // Feet
        if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
        {
          lBuiltContent.append(SliderFileBuilder::getFeetLineForBHUNP(aFeetModIndex));
        }

        // Hands
        if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
        {
          lBuiltContent.append("%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Hands\"/>\n");
        }
        break;
    }

    lBuiltContent.append("%1</Group>\n");
  }

  lBuiltContent.append("</SliderGroups>");

  // Replace the "%1" string with identation spaces and "%2" with the name given to the line
  return lBuiltContent.arg("    ").arg(aLineName);
}

QString SliderFileBuilder::getFeetLineForBHUNP(const int aFeetModIndex)
{
  switch (aFeetModIndex)
  {
    case 0:
      // Default
      return "%1%1<Member name=\"%2 - BHUNP 3BBB Advanced Feet\"/>\n";
    case 1:
      // More Sliders for Feet - Normal
      return "%1%1<Member name=\"%2 - Feet (MSF - normal)\"/>\n";
    case 2:
      // More Sliders for Feet - High Heels
      return "%1%1<Member name=\"%2 - Feet (MSF - HH)\"/>\n";
    case 3:
      // HG Feet and Toes BHUNP SE - HGFeet UUNP
      return "%1%1<Member name=\"%2 - HGFeet UUNP\"/>\n";
    case 4:
      // Khrysamere HG Feet (BHUNP)
      return "%1%1<Member name=\"%2 - [Khrysamere] HG Feet (BHUNP)\"/>\n";
    case 5:
      // Khrysamere HG Feet (Claws) (BHUNP)
      return "%1%1<Member name=\"%2 - [Khrysamere] HG Feet (Claws) (BHUNP)\"/>\n";
  }

  return QString();
}

std::vector<Struct::Filter> SliderFileBuilder::getXMLDefaultFiltersFromBody(const BodyNameVersion& aBody)
{
  auto lDefaultFilters{std::vector<Struct::Filter>()};
  lDefaultFilters.push_back(Struct::Filter("MFBO", true, true, true));

  if (Utils::isCBBEBasedBody(aBody))
  {
    lDefaultFilters.push_back(Struct::Filter("CBBE", true, true, true));
    lDefaultFilters.push_back(Struct::Filter("CBBE Bodies", true, true, true));

    // More specific filters
    switch (aBody)
    {
      case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
        lDefaultFilters.push_back(Struct::Filter("3BA", true, true, true));
        [[fallthrough]];
      case BodyNameVersion::CBBE_3BBB_3BA_1_50:
      case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
      case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
      case BodyNameVersion::CBBE_3BBB_3BA_2_06:
        lDefaultFilters.push_back(Struct::Filter("3BBB", true, true, true));
        break;
    }
  }
  else
  {
    lDefaultFilters.push_back(Struct::Filter("BHUNP 3BBB", true, true, true));
    lDefaultFilters.push_back(Struct::Filter("UUNP 3BBB", true, true, true));
    lDefaultFilters.push_back(Struct::Filter("Unified UNP", true, true, true));
  }

  return lDefaultFilters;
}
