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
    lBuiltContent.append("\n").append(Utils::ReadQRCFileContent(DataLists::GetQRCPathForResource(aBody, aFeet, BodyPartType::BODY)).arg(aLineName));

  // Feet
  if (aTargetBlocks % 100 / 10 == 1)
    lBuiltContent.append("\n").append(
      Utils::ReadQRCFileContent(
        DataLists::GetQRCPathForResource(aBody, aFeet, BodyPartType::FEET))
        .arg(aLineName));

  // Hands
  if (aTargetBlocks % 10 == 1)
  {
    lBuiltContent.append("\n").append(
      Utils::ReadQRCFileContent(
        DataLists::GetQRCPathForResource(aBody, aFeet, aMustUseBeastHands ? BodyPartType::BEAST_HANDS : BodyPartType::HANDS))
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
  return QString("%1%1<Member name=\"%2 - ").append(DataLists::GetBodySliderValue(aBodyNameVersion)).append(QString("\"/>\n"));
}

QString SliderFileBuilder::GetFeetLine(const FeetNameVersion& aFeetNameVersion)
{
  return QString("%1%1<Member name=\"%2 - ").append(DataLists::GetFeetSliderValue(aFeetNameVersion)).append(QString("\"/>\n"));
}

QString SliderFileBuilder::GetHandsLine(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands)
{
  return QString("%1%1<Member name=\"%2 - ").append(DataLists::GetHandsSliderValue(aBodyNameVersion, aMustUseBeastHands)).append(QString("\"/>\n"));
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
