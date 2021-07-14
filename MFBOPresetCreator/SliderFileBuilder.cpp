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
    lBuiltContent.append("\n").append(SliderFileBuilder::getBodyBlock(aLineName, aBody));

  // Feet
  if (aTargetBlocks % 100 / 10 == 1)
    lBuiltContent.append("\n").append(SliderFileBuilder::getFeetBlock(aLineName, aBody, aFeetModIndex));

  // Hands
  if (aTargetBlocks % 10 == 1)
    lBuiltContent.append("\n").append(SliderFileBuilder::getHandsBlock(aLineName, aBody, aMustUseBeastHands));

  lBuiltContent.append("\n</SliderSetInfo>\n");
  return lBuiltContent;
}

QString SliderFileBuilder::getHandsBlock(const QString& aLineName, const BodyNameVersion& aBody, const bool aMustUseBeastHands)
{
  if (Utils::isCBBEBasedBody(aBody) && aMustUseBeastHands)
  {
    // TODO: Add an option to choose the installed CBBE version to match the beast hands
    // Take the default CBBE v.1.6.1 beast hands
    return Utils::readQRCFileContent(":/presets/beast_hands/cbbe 1.6.1").arg(aLineName);
  }

  // Take the normal hands
  return Utils::readQRCFileContent(DataLists::getQRCPathFromBodyName(aBody, "hands")).arg(aLineName);
}

QString SliderFileBuilder::getFeetBlock(const QString& aLineName, const BodyNameVersion& aBody, const int aFeetModIndex)
{
  auto lSliderSet{QString()};

  switch (aFeetModIndex)
  {
    case 0:
      // Default mod's feet sliders
      return Utils::readQRCFileContent(DataLists::getQRCPathFromBodyName(aBody, "feet")).arg(aLineName);
    case 1:
      // More Sliders for Feet - Normal
      if (Utils::isCBBEBasedBody(aBody))
      {
        lSliderSet = QString("%1<SliderSet name=\"%2 - Feet (MSF - normal)\">\n%1%1<DataFolder>More Sliders for Feet CBBE</DataFolder>\n%1%1<SourceFile>CBBE MSF Feet.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\" DataFolder=\"CBBE\">Feet</Shape>\n%1%1<Slider name=\"AnkleSize\" invert=\"false\" small=\"100\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkleSize\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetAnkleSize</Data>\n%1%1</Slider>\n%1%1<Slider name=\"FeetFeminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeetFeminine\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetFeetFeminine</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Fix\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFix\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetFix</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Soles\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSoles\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetSoles</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Padding\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPadding\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetPadding</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Achilles Tendon\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAchilles Tendon\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetAchilles Tendon</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ankles Bones\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkles Bones\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetAnkles Bones</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Arch\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetArch\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetArch</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Heel\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetHeel\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetHeel</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dome\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDome\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetDome</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Intrinsic\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetIntrinsic\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetIntrinsic</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ball\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBall\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetBall</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Wakfu\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetWakfu\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetWakfu</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Longer\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetToes Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLonger\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetLonger</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt BigToe Nail\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt BigToe Nail\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetAlt BigToe Nail</Data>\n%1%1</Slider>\n%1%1<Slider name=\"BigToe Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigToe Longer\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetBigToe Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt 2ndToe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt 2ndToe\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetAlt 2ndToe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Alignement\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Alignement\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetToes Alignement</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Slim Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSlim Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetSlim Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Chubby Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetChubby Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetChubby Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Nails\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt Nails\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetAlt Nails</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Nails Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetNails Longer\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetNails Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt BigToe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt BigToe\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetAlt BigToe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"BigToe Spreading\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigToe Spreading\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetBigToe Spreading</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Plated BigToe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPlated BigToe\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetPlated BigToe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Before Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBefore Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetBefore Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Tight Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTight Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetTight Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"High Heel\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetHigh Heel\" target=\"Feet\" local=\"true\">CBBE MSF Feet.osd\\FeetHigh Heel</Data>\n%1%1</Slider>\n%1</SliderSet>");
      }
      else
      {
        lSliderSet = QString("%1<SliderSet name=\"%2 - Feet (MSF - normal)\">\n%1%1<DataFolder>More Sliders for Feet BHUNP</DataFolder>\n%1%1<SourceFile>BHUNP Feet MSF.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\" DataFolder=\"UUNP 3BBB Vagina SMP\">Feet</Shape>\n%1%1<Slider name=\"7B Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Bombshell Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Bombshell Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Bombshell Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Bombshell High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Bombshell High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Bombshell High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Natural Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Natural Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Natural Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Natural High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Natural High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Natural High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Cleavage Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Cleavage Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Cleavage Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Cleavage High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Cleavage High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Cleavage High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B BCup Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B BCup Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B BCup Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B BCup High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B BCup High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B BCup High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Oppai Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Oppai Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Oppai Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Oppai High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Oppai High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B Oppai High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B CH Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B CH Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B CH Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B CH High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B CH High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\Feet7B CH High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Pushup Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Pushup Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP Pushup Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Pushup High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Pushup High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP Pushup High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Skinny Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Skinny Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP Skinny Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Skinny High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Skinny High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP Skinny High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Perky Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Perky Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP Perky Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Perky High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Perky High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNP Perky High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPB Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPB High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Chapi\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Chapi\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPB Chapi</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v1\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v1\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPB Oppai v1</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v3.2 Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v3.2 Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPB Oppai v3.2 Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v3.2 High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v3.2 High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPB Oppai v3.2 High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPetite Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPetite Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPetite Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPetite High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPetite High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPetite High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPC Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPC Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPC Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPC High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPC High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPC High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPCM Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPCM Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPCM Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPCM High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPCM High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPCM High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPSH Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPSH Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPSH Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPSH High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPSH High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPSH High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPK Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPK High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Bonus Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Bonus Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPK Bonus Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Bonus High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Bonus High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPK Bonus High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UN7B Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUN7B Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUN7B Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UN7B High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUN7B High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUN7B High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPBB Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPBB Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPBB Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPBB High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPBB High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPBB High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Seraphim Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSeraphim Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetSeraphim Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Seraphim High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSeraphim High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetSeraphim High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Demonfet Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDemonfet Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetDemonfet Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Demonfet High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDemonfet High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetDemonfet High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dream Girl Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDream Girl Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetDream Girl Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dream Girl High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDream Girl High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetDream Girl High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Top Model Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTop Model Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetTop Model Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Top Model High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTop Model High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetTop Model High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Leito Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLeito Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetLeito Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Leito High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLeito High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetLeito High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPF Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPF Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPF Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPF High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPF High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPF High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPFx Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPFx Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPFx Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPFx High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPFx High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetUNPFx High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"MCBM Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetMCBM Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetMCBM Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"MCBM High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetMCBM High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetMCBM High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Venus Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetVenus Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetVenus Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Venus High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetVenus High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetVenus High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Manga Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetManga Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetManga Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Manga High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetManga High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetManga High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CHSBHC Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCHSBHC Low\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetCHSBHC Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CHSBHC High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCHSBHC High\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetCHSBHC High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Achilles Tendon\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAchilles Tendon\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetAchilles Tendon</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Arch\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetArch\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetArch</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ball\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBall\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetBall</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Before Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBefore Toes\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetBefore Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dome\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDome\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetDome</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Feminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeminine\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetFeminine</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Heel\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetHeel\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetHeel</Data>\n%1%1</Slider>\n%1%1<Slider name=\"High Heel\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetHigh Heel\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetHigh Heel</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Intrinsic\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetIntrinsic\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetIntrinsic</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLonger\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetLonger</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Soles\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSoles\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetSoles</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Wakfu\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetWakfu\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetWakfu</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt 2nd Toe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt 2nd Toe\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetAlt 2nd Toe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Bigtoe Nail\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt Bigtoe Nail\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetAlt Bigtoe Nail</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Bigtoe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt Bigtoe\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetAlt Bigtoe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Nails\" invert=\"false\" small=\"0\" big=\"0\">\n"
                             "%1%1%1<Data name=\"FeetAlt Nails\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetAlt Nails</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ankles Bones\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkles Bones\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetAnkles Bones</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Bigtoe Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigtoe Longer\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetBigtoe Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Bigtoe Spreading\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigtoe Spreading\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetBigtoe Spreading</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Chubby Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetChubby Toes\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetChubby Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Fix\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFix\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetFix</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Nails Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetNails Longer\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetNails Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Padding\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPadding\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetPadding</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Plated Bigtoe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPlated Bigtoe\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetPlated Bigtoe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Slim Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSlim Toes\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetSlim Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Tight Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTight Toes\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetTight Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Alignement\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Alignement\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetToes Alignement</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Longer\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetToes Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"TTNL\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTTNL\" target=\"Feet\" local=\"true\">BHUNP Feet MSF.osd\\FeetTTNL</Data>\n%1%1</Slider>\n%1</SliderSet>");
      }

      break;
    case 2:
      // More Sliders for Feet - High Heels
      if (Utils::isCBBEBasedBody(aBody))
      {
        lSliderSet = QString("%1<SliderSet name=\"%2 - Feet (MSF - HH)\">\n%1%1<DataFolder>More Sliders for Feet CBBE</DataFolder>\n%1%1<SourceFile>CBBE MSF Feet High Heel.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\" DataFolder=\"CBBE\">Feet</Shape>\n%1%1<Slider name=\"AnkleSize\" invert=\"false\" small=\"100\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkleSize\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetAnkleSize</Data>\n%1%1</Slider>\n%1%1<Slider name=\"FeetFeminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeetFeminine\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetFeetFeminine</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Fix\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFix\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetFix</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Soles\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSoles\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetSoles</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Padding\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPadding\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetPadding</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Achilles Tendon\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAchilles Tendon\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetAchilles Tendon</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ankles Bones\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkles Bones\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetAnkles Bones</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Arch\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetArch\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetArch</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Heel\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetHeel\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetHeel</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dome\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDome\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetDome</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Intrinsic\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetIntrinsic\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetIntrinsic</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ball\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBall\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetBall</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Wakfu\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetWakfu\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetWakfu</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Longer\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetToes Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLonger\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetLonger</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt BigToe Nail\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt BigToe Nail\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetAlt BigToe Nail</Data>\n%1%1</Slider>\n%1%1<Slider name=\"BigToe Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigToe Longer\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetBigToe Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt 2ndToe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt 2ndToe\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetAlt 2ndToe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Alignement\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Alignement\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetToes Alignement</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Slim Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSlim Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetSlim Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Chubby Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetChubby Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetChubby Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Nails\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt Nails\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetAlt Nails</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Nails Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetNails Longer\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetNails Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt BigToe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt BigToe\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetAlt BigToe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"BigToe Spreading\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigToe Spreading\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetBigToe Spreading</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Plated BigToe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPlated BigToe\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetPlated BigToe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Before Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBefore Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetBefore Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Tight Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTight Toes\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetTight Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Flat\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFlat\" target=\"Feet\" local=\"true\">CBBE MSF Feet High Heel.osd\\FeetFlat</Data>\n%1%1</Slider>\n%1</SliderSet>");
      }
      else
      {
        lSliderSet = QString("%1<SliderSet name=\"%2 - Feet (MSF - HH)\">\n%1%1<DataFolder>More Sliders for Feet BHUNP</DataFolder>\n%1%1<SourceFile>BHUNP Feet  High Heel MSF.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\" DataFolder=\"UUNP 3BBB Vagina SMP\">Feet</Shape>\n%1%1<Slider name=\"7B Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Bombshell Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Bombshell Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Bombshell Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Bombshell High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Bombshell High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Bombshell High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Natural Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Natural Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Natural Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Natural High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Natural High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Natural High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Cleavage Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Cleavage Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Cleavage Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Cleavage High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Cleavage High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Cleavage High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B BCup Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B BCup Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B BCup Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B BCup High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B BCup High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B BCup High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Oppai Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Oppai Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Oppai Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Oppai High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Oppai High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B Oppai High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B CH Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B CH Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B CH Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B CH High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B CH High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\Feet7B CH High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Pushup Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Pushup Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP Pushup Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Pushup High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Pushup High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP Pushup High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Skinny Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Skinny Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP Skinny Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Skinny High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Skinny High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP Skinny High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Perky Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Perky Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP Perky Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Perky High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Perky High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNP Perky High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPB Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPB High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Chapi\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Chapi\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPB Chapi</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v1\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v1\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPB Oppai v1</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v3.2 Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v3.2 Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPB Oppai v3.2 Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v3.2 High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v3.2 High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPB Oppai v3.2 High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPetite Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPetite Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPetite Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPetite High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPetite High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPetite High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPC Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPC Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPC Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPC High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPC High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPC High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPCM Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPCM Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPCM Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPCM High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPCM High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPCM High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPSH Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPSH Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPSH Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPSH High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPSH High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPSH High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPK Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPK High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Bonus Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Bonus Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPK Bonus Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Bonus High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Bonus High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPK Bonus High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UN7B Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUN7B Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUN7B Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UN7B High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUN7B High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUN7B High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPBB Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPBB Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPBB Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPBB High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPBB High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPBB High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Seraphim Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSeraphim Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetSeraphim Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Seraphim High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSeraphim High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetSeraphim High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Demonfet Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDemonfet Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetDemonfet Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Demonfet High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDemonfet High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetDemonfet High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dream Girl Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDream Girl Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetDream Girl Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dream Girl High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDream Girl High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetDream Girl High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Top Model Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTop Model Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetTop Model Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Top Model High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTop Model High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetTop Model High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Leito Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLeito Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetLeito Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Leito High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLeito High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetLeito High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPF Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPF Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPF Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPF High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPF High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPF High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPFx Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPFx Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPFx Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPFx High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPFx High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetUNPFx High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"MCBM Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetMCBM Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetMCBM Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"MCBM High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetMCBM High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetMCBM High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Venus Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetVenus Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetVenus Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Venus High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetVenus High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetVenus High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Manga Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetManga Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetManga Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Manga High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetManga High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetManga High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CHSBHC Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCHSBHC Low\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetCHSBHC Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CHSBHC High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCHSBHC High\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetCHSBHC High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Achilles Tendon\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAchilles Tendon\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetAchilles Tendon</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Arch\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetArch\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetArch</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ball\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBall\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetBall</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Before Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBefore Toes\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetBefore Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dome\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDome\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetDome</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Feminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeminine\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetFeminine</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Heel\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetHeel\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetHeel</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Intrinsic\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetIntrinsic\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetIntrinsic</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLonger\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetLonger</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Soles\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSoles\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetSoles</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Wakfu\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetWakfu\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetWakfu</Data>\n%1%1</Slider>\n"
                             "%1%1<Slider name=\"Alt 2nd Toe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt 2nd Toe\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetAlt 2nd Toe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Bigtoe Nail\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt Bigtoe Nail\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetAlt Bigtoe Nail</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Bigtoe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt Bigtoe\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetAlt Bigtoe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Alt Nails\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAlt Nails\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetAlt Nails</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Ankles Bones\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkles Bones\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetAnkles Bones</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Bigtoe Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigtoe Longer\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetBigtoe Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Bigtoe Spreading\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetBigtoe Spreading\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetBigtoe Spreading</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Chubby Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetChubby Toes\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetChubby Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Fix\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFix\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetFix</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Nails Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetNails Longer\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetNails Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Padding\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPadding\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetPadding</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Plated Bigtoe\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetPlated Bigtoe\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetPlated Bigtoe</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Slim Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSlim Toes\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetSlim Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Tight Toes\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTight Toes\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetTight Toes</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Alignement\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Alignement\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetToes Alignement</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Toes Longer\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetToes Longer\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetToes Longer</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Normal\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetNormal\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetNormal</Data>\n%1%1</Slider>\n%1%1<Slider name=\"TTNL\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTTNL\" target=\"Feet\" local=\"true\">BHUNP Feet  High Heel MSF.osd\\FeetTTNL</Data>\n%1%1</Slider>\n%1</SliderSet>");
      }

      break;
  }

  // Replace the "%1" string with identation spaces and "%2" with the name given to the line
  return lSliderSet.arg("    ").arg(aLineName);
}

QString SliderFileBuilder::getBodyBlock(const QString& aLineName, const BodyNameVersion& aBody)
{
  return Utils::readQRCFileContent(DataLists::getQRCPathFromBodyName(aBody, "body")).arg(aLineName);
}

QString SliderFileBuilder::buildXMLFileContent(const QString& aLineName, const std::vector<Struct::Filter>& aFiltersList, const BodyNameVersion& aBody, const bool aMustUseBeastHands, const int aFeetModIndex, const unsigned char& aTargetBlocks)
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
