#include "SliderFileBuilder.h"
#include "DataLists.h"
#include "Utils.h"
#include <QFile>
#include <QTextStream>

QString SliderFileBuilder::BuildOSPFileContent(const QString& aLineName, const BodyNameVersion& aBody, const FeetNameVersion& aFeet, const bool aMustUseBeastHands, const unsigned char& aTargetBlocks)
{
  auto lBuiltContent{QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<SliderSetInfo version=\"1\">")};

  // Body
  if (aTargetBlocks / 100 == 1)
    lBuiltContent.append("\n").append(Utils::ReadQRCFileContent(DataLists::GetQRCPathForRessource(aBody, aFeet, BodyPartType::BODY)).arg(aLineName));

  // Feet
  if (aTargetBlocks % 100 / 10 == 1)
    lBuiltContent.append("\n").append(
      Utils::ReadQRCFileContent(
        DataLists::GetQRCPathForRessource(aBody, aFeet, BodyPartType::FEET))
        .arg(aLineName));

  // Hands
  if (aTargetBlocks % 10 == 1)
  {
    lBuiltContent.append("\n").append(
      Utils::ReadQRCFileContent(
        DataLists::GetQRCPathForRessource(aBody, aFeet, aMustUseBeastHands ? BodyPartType::BEAST_HANDS : BodyPartType::HANDS))
        .arg(aLineName));
  }

  lBuiltContent.append("\n</SliderSetInfo>\n");
  return lBuiltContent;
}

QString SliderFileBuilder::BuildXMLFileContent(const QString& aLineName,
                                               const std::vector<Struct::Filter>& aFiltersList,
                                               const BodyNameVersion& aBodyNameVersion,
                                               const FeetNameVersion& aFeetNameVersion,
                                               const bool aMustUseBeastHands,
                                               const unsigned char& aTargetBlocks)
{
  auto lBuiltContent{QString("<SliderGroups>\n")};

  const auto& lFilters{aFiltersList.size() > 0 ? aFiltersList : SliderFileBuilder::GetXMLDefaultFiltersFromBody(aBodyNameVersion)};

  for (const auto& lFilter : lFilters)
  {
    if (lFilter.getFilter().isEmpty())
    {
      continue;
    }

    // <Group>
    lBuiltContent.append(QString("    <Group name=\"%1\">\n").arg(lFilter.getFilter()));

    // Body
    if (aTargetBlocks / 100 == 1 && lFilter.isBodyCompatible())
    {
      lBuiltContent.append(SliderFileBuilder::GetBodyLine(aBodyNameVersion));
    }

    // Feet
    if (aTargetBlocks % 100 / 10 == 1 && lFilter.isFeetCompatible())
    {
      lBuiltContent.append(SliderFileBuilder::GetFeetLine(aFeetNameVersion));
    }

    // Hands
    if (aTargetBlocks % 10 == 1 && lFilter.isHandsCompatible())
    {
      lBuiltContent.append(SliderFileBuilder::GetHandsLine(aBodyNameVersion, aMustUseBeastHands));
    }

    // </Group>
    lBuiltContent.append("%1</Group>\n");
  }

  lBuiltContent.append("</SliderGroups>");

  // Replace the "%1" string with identation spaces and "%2" with the name given to the line
  return lBuiltContent.arg("    ", aLineName);
}

QString SliderFileBuilder::GetBodyLine(const BodyNameVersion& aBodyNameVersion)
{
  const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
  const auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};

  auto lSliderSetName{QString()};

  switch (lBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::MIMIR_EBONIC_BODY:
      lSliderSetName = QString("CBBE 3BBB Body Amazing");
      break;
    case BodyVariant::BHUNP_3BBB:
      lSliderSetName = QString("BHUNP 3BBB");
      break;
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      lSliderSetName = QString("BHUNP 3BBB Advanced");
      break;
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      lSliderSetName = QString("BHUNP 3BBB Advanced Ver 2");
      break;
    case BodyVariant::BHUNP_BBP:
      lSliderSetName = QString("BHUNP BBP");
      break;
    case BodyVariant::BHUNP_BBP_ADVANCED:
      lSliderSetName = QString("BHUNP BBP Advanced");
      break;
    case BodyVariant::BHUNP_TBBP:
      lSliderSetName = QString("BHUNP TBBP");
      break;
    case BodyVariant::BHUNP_TBBP_ADVANCED:
      lSliderSetName = QString("BHUNP TBBP Advanced");
      break;
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      lSliderSetName = QString("BHUNP 3BBB Advanced Ver 2 Nevernude");
      break;
    case BodyVariant::CBBE_SMP_3BBB:
      lSliderSetName = QString("CBBE Body SMP (3BBB)");
      break;
    case BodyVariant::COCO_BODY_CBBE:
      switch (lVersionIndex)
      {
        case 0:
          lSliderSetName = QString("[COCO]3Bsmp_BodyV4_A");
          break;
        case 1:
          lSliderSetName = QString("[COCO 3BBB V6]Body_A");
          break;
        default:
          // Keep the default empty text
          break;
      }
      break;
    case BodyVariant::COCO_BODY_UUNP:
      switch (lVersionIndex)
      {
        case 0:
          lSliderSetName = QString("[COCO]body_B_v3");
          break;
        case 1:
          lSliderSetName = QString("[COCO]3Bsmp_BodyV4_B");
          break;
        case 2:
          lSliderSetName = QString("[COCO 3BBB V6]Body_B");
          break;
        default:
          // Keep the default empty text
          break;
      }
      break;
    case BodyVariant::_INVALID_VALUE:
    default:
      // Keep the default empty text
      break;
  }

  return QString("%1%1<Member name=\"%2 - ").append(lSliderSetName).append(QString("\"/>\n"));
}

QString SliderFileBuilder::GetFeetLine(const FeetNameVersion& aFeetNameVersion)
{
  const auto lVariant{DataLists::GetVariant(aFeetNameVersion)};
  const auto lVersionIndex{DataLists::GetVersionIndex(aFeetNameVersion)};

  auto lSliderSetName{QString()};

  switch (lVariant)
  {
    case FeetVariant::CBBE:
      lSliderSetName = QString("CBBE Feet");
      break;
    case FeetVariant::BHUNP:
      lSliderSetName = QString("BHUNP 3BBB Advanced Feet");
      break;
    case FeetVariant::CBBE_3BA_3BBB:
      lSliderSetName = QString("CBBE 3BBB Feet");
      break;
    case FeetVariant::COCO_BODY_CBBE:
      switch (lVersionIndex)
      {
        case 0:
          lSliderSetName = QString("[COCO]3Bsmp_FeetV4_A");
          break;
        case 1:
          lSliderSetName = QString("[COCO 3BBB V6]Feet_A");
          break;
        default:
          // Keep the default empty text
          break;
      }
      break;
    case FeetVariant::COCO_BODY_UUNP:
      switch (lVersionIndex)
      {
        case 0:
          lSliderSetName = QString("[COCO]bodyFeets_B_v3");
          break;
        case 1:
          lSliderSetName = QString("[COCO]3Bsmp_FeetV4_B");
          break;
        case 2:
          lSliderSetName = QString("[COCO 3BBB V6]Feet_B");
          break;
        default:
          // Keep the default empty text
          break;
      }
      break;
    case FeetVariant::MIMIR_EBONIC_BODY:
      lSliderSetName = QString("CBBE 3BBB Feet");
      break;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
      lSliderSetName = QString("CBBE MSF Feet");
      break;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
      lSliderSetName = QString("BHUNP Feet MSF");
      break;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
      lSliderSetName = QString("CBBE MSF Feet High Heel");
      break;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
      lSliderSetName = QString("BHUNP Feet High Heel MSF");
      break;
    case FeetVariant::HG_FEET:
      lSliderSetName = QString("HGFeet UUNP");
      break;
    case FeetVariant::KHRYSAMERE_HG_FEET:
      switch (lVersionIndex)
      {
        case 0:
          lSliderSetName = QString("[Khrysamere] HG Feet (BHUNP)");
          break;
        case 1:
          lSliderSetName = QString("[Khrysamere] K Feet");
          break;
        case 2:
          lSliderSetName = QString("[Khrysamere] K Feet");
          break;
        case 3:
          lSliderSetName = QString("[Khrysamere] K Feet v5.0");
          break;
        case 4:
          lSliderSetName = QString("[Khrysamere] K Feet");
          break;
        default:
          // Keep the default empty text
          break;
      }
      // Keep the default empty text
      break;
  }

  return QString("%1%1<Member name=\"%2 - ").append(lSliderSetName).append(QString("\"/>\n"));
}

QString SliderFileBuilder::GetHandsLine(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands)
{
  // Beast hands
  if (Utils::IsCBBEBasedBody(aBodyNameVersion) && aMustUseBeastHands)
  {
    return QString("CBBE Hands Beast");
  }

  auto lBodyName{DataLists::GetName(aBodyNameVersion)};
  auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};

  auto lSliderSetName{QString()};

  switch (lBodyName)
  {
    case BodyName::CBBE_3BA_3BBB:
    case BodyName::MIMIR_EBONIC_BODY:
      lSliderSetName = QString("CBBE 3BBB Hands");
      break;
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
      lSliderSetName = QString("BHUNP 3BBB Advanced Hands");
      break;
    case BodyName::CBBE_SMP_3BBB:
      lSliderSetName = QString("CBBE Hands");
      break;
    case BodyName::COCO_BODY_CBBE:
      switch (lVersionIndex)
      {
        case 0:
          lSliderSetName = QString("[COCO]3Bsmp_HandV4_A");
          break;
        case 1:
          lSliderSetName = QString("[COCO 3BBB V6]Hands_A");
          break;
        default:
          // Keep the default empty text
          break;
      }
      break;
    case BodyName::COCO_BODY_UUNP:
      switch (lVersionIndex)
      {
        case 0:
          lSliderSetName = QString("[COCO]bodyHands_B_v3");
          break;
        case 1:
          lSliderSetName = QString("[COCO]3Bsmp_HandV4_B");
          break;
        case 2:
          lSliderSetName = QString("[COCO 3BBB V6]Hands_B");
          break;
        default:
          // Keep the default empty text
          break;
      }
      break;
    case BodyName::_INVALID_VALUE:
    default:
      // Keep the default empty text
      break;
  }

  return QString("%1%1<Member name=\"%2 - ").append(lSliderSetName).append(QString("\"/>\n"));
}

std::vector<Struct::Filter> SliderFileBuilder::GetXMLDefaultFiltersFromBody(const BodyNameVersion& aBody)
{
  auto lDefaultFilters{std::vector<Struct::Filter>()};

  // Push the MFBO filter for any body base
  lDefaultFilters.push_back(Struct::Filter("MFBO", true, true, true));

  if (Utils::IsCBBEBasedBody(aBody))
  {
    // Common filters
    lDefaultFilters.push_back(Struct::Filter("CBBE", true, true, true));
    lDefaultFilters.push_back(Struct::Filter("CBBE Bodies", true, true, true));

    const auto lBodyName{DataLists::GetName(aBody)};

    // For Mimir only
    if (lBodyName == BodyName::MIMIR_EBONIC_BODY)
    {
      lDefaultFilters.push_back(Struct::Filter("3BA", true, true, true));
    }

    // For Mimir or CBBE 3BBB 3BA
    if (lBodyName == BodyName::MIMIR_EBONIC_BODY || lBodyName == BodyName::CBBE_3BA_3BBB)
    {
      lDefaultFilters.push_back(Struct::Filter("3BBB", true, true, true));
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
