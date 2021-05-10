#include "SliderSetsHelper.h"

QString SliderSetsHelper::getSliderSetFile(const BodyNameVersion& aBody, const bool& aMustUseBeastHands, const int& aFeetModIndex)
{
  return QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<SliderSetInfo version=\"1\">\n%1\n%2\n%3\n</SliderSetInfo>")
    .arg(SliderSetsHelper::getBodyBlock(aBody))
    .arg(SliderSetsHelper::getFeetBlock(aBody, aFeetModIndex))
    .arg(SliderSetsHelper::getHandsBlock(aBody, aMustUseBeastHands));
}

QString SliderSetsHelper::getHandsBlock(const BodyNameVersion& aBody, const bool& aMustUseBeastHands)
{
  return QString("azeaze");
}

QString SliderSetsHelper::getFeetBlock(const BodyNameVersion& aBody, const int& aFeetModIndex)
{
  auto lSliderSet{QString("")};

  switch (aFeetModIndex)
  {
    case 0:
      // Default
      switch (aBody)
      {
        case BodyNameVersion::CBBE_3BBB_3BA_1_40:
          lSliderSet = QString("%1<SliderSet name=\"{%%bodyslide_set_name%%} - Feet\">\n%1%1<DataFolder>CBBE</DataFolder>%1%1<SourceFile>CBBE Feet.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\">Feet</Shape>\n%1%1<Slider name=\"AnkleSize\" invert=\"false\" small=\"100\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkleSize\" target=\"Feet\" local=\"true\">CBBE Feet.osd\\FeetAnkleSize</Data>\n%1%1</Slider>\n%1%1<Slider name=\"FeetFeminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeetFeminine\" target=\"Feet\" local=\"true\">CBBE Feet.osd\\FeetFeetFeminine</Data>\n%1%1</Slider>\n%1</SliderSet>");
          break;
        case BodyNameVersion::CBBE_3BBB_3BA_1_50:
        case BodyNameVersion::CBBE_3BBB_3BA_1_51_to_1_55:
        case BodyNameVersion::CBBE_3BBB_3BA_2_00_to_2_04:
          lSliderSet = QString("%1<SliderSet name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Feet\">\n%1%1<DataFolder>CBBE</DataFolder>\n%1%1<SourceFile>CBBE Feet.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\" DataFolder=\"CBBE\">Feet</Shape>\n%1%1<Slider name=\"AnkleSize\" invert=\"false\" small=\"100\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkleSize\" target=\"Feet\">CBBE Feet.osd\\FeetAnkleSize</Data>\n%1%1</Slider>\n%1%1<Slider name=\"FeetFeminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeetFeminine\" target=\"Feet\">CBBE Feet.osd\\FeetFeetFeminine</Data>\n%1%1</Slider>\n%1</SliderSet>");
          break;
        case BodyNameVersion::CBBE_3BBB_3BA_2_05_to_2_06:
          lSliderSet = QString("%1<SliderSet name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Feet\">\n%1%1<DataFolder>CBBE</DataFolder>\n%1%1<SourceFile>CBBE Feet.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\" DataFolder=\"CBBE\">Feet</Shape>\n%1%1<Slider name=\"AnkleSize\" invert=\"false\" small=\"100\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkleSize\" target=\"Feet\" local=\"true\">CBBE Feet.osd\\FeetAnkleSize</Data>\n%1%1</Slider>\n%1%1<Slider name=\"FeetFeminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeetFeminine\" target=\"Feet\" local=\"true\">CBBE Feet.osd\\FeetFeetFeminine</Data>\n%1%1</Slider>\n%1</SliderSet>");
          break;
        case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
          lSliderSet = QString("%1<SliderSet name=\"{%%bodyslide_set_name%%} - CBBE Feet\">\n%1%1<DataFolder>CBBE</DataFolder>\n%1%1<SourceFile>CBBE Feet.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\">Feet</Shape>\n%1%1<Slider name=\"AnkleSize\" invert=\"false\" small=\"100\" big=\"0\">\n%1%1%1<Data name=\"FeetAnkleSize\" target=\"Feet\" local=\"true\">CBBE Feet.osd\\FeetAnkleSize</Data>\n%1%1</Slider>\n%1%1<Slider name=\"FeetFeminine\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetFeetFeminine\" target=\"Feet\" local=\"true\">CBBE Feet.osd\\FeetFeetFeminine</Data>\n%1%1</Slider>\n%1</SliderSet>");
          break;
        case BodyNameVersion::BHUNP_3BBB_2_13:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
        case BodyNameVersion::BHUNP_BBP_2_13:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
        case BodyNameVersion::BHUNP_TBBP_2_13:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
        case BodyNameVersion::BHUNP_3BBB_2_15:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_15:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_15:
        case BodyNameVersion::BHUNP_BBP_2_15:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_15:
        case BodyNameVersion::BHUNP_TBBP_2_15:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_15:
        case BodyNameVersion::BHUNP_3BBB_2_20:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_20:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_20:
        case BodyNameVersion::BHUNP_BBP_2_20:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_20:
        case BodyNameVersion::BHUNP_TBBP_2_20:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_20:
        case BodyNameVersion::BHUNP_3BBB_2_25:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_25:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_25:
        case BodyNameVersion::BHUNP_BBP_2_25:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_25:
        case BodyNameVersion::BHUNP_TBBP_2_25:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_25:
          lSliderSet = QString("%1<SliderSet name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\">\n%1%1<DataFolder>BHUNP 3BBB Advanced</DataFolder>\n%1%1<SourceFile>BHUNP 3BBB Advanced Feet.nif</SourceFile>\n%1%1<OutputPath>{%%feet_output_path%%}</OutputPath>\n%1%1<OutputFile GenWeights=\"true\">{%%feet_output_file%%}</OutputFile>\n%1%1<Shape target=\"Feet\">Feet</Shape>\n%1%1<Slider name=\"7B Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Bombshell Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Bombshell Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Bombshell Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Bombshell High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Bombshell High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Bombshell High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Natural Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Natural Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Natural Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Natural High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Natural High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Natural High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Cleavage Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Cleavage Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Cleavage Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Cleavage High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Cleavage High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Cleavage High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B BCup Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B BCup Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B BCup Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B BCup High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B BCup High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B BCup High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Oppai Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Oppai Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Oppai Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B Oppai High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B Oppai High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B Oppai High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B CH Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B CH Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B CH Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7B CH High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7B CH High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7B CH High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Pushup Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Pushup Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP Pushup Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Pushup High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Pushup High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP Pushup High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Skinny Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Skinny Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP Skinny Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Skinny High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Skinny High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP Skinny High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Perky Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Perky Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP Perky Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNP Perky High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNP Perky High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNP Perky High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPB Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPB High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Chapi\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Chapi\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPB Chapi</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v1\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v1\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPB Oppai v1</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v3.2 Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v3.2 Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPB Oppai v3.2 Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPB Oppai v3.2 High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPB Oppai v3.2 High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPB Oppai v3.2 High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPetite Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPetite Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPetite Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPetite High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPetite High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPetite High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPC Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPC Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPC Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPC High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPC High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPC High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPCM Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPCM Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPCM Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPCM High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPCM High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPCM High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPSH Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPSH Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPSH Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPSH High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPSH High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPSH High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPK Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPK High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Bonus Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Bonus Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPK Bonus Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPK Bonus High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPK Bonus High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPK Bonus High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UN7B Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUN7B Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUN7B Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UN7B High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUN7B High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUN7B High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPBB Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPBB Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPBB Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPBB High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPBB High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPBB High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Seraphim Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSeraphim Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetSeraphim Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Seraphim High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetSeraphim High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetSeraphim High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Demonfet Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDemonfet Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetDemonfet Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Demonfet High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDemonfet High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetDemonfet High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dream Girl Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDream Girl Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetDream Girl Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Dream Girl High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetDream Girl High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetDream Girl High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Top Model Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTop Model Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetTop Model Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Top Model High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetTop Model High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetTop Model High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Leito Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLeito Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetLeito Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Leito High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetLeito High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetLeito High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPF Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPF Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPF Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPF High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPF High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPF High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPFx Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPFx Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPFx Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"UNPFx High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetUNPFx High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetUNPFx High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"MCBM Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetMCBM Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetMCBM Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"MCBM High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetMCBM High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetMCBM High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Venus Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetVenus Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetVenus Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Venus High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetVenus High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetVenus High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Manga Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetManga Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetManga Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"Manga High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetManga High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetManga High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CHSBHC Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCHSBHC Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetCHSBHC Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CHSBHC High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCHSBHC High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetCHSBHC High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"7BUNP Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"Feet7BUNP Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\Feet7BUNP Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CNHF Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCNHF Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetCNHF Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CNHF High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCNHF High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetCNHF High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CNHF Bonus Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCNHF Bonus Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetCNHF Bonus Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"CNHF Bonus High\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetCNHF Bonus High\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetCNHF Bonus High</Data>\n%1%1</Slider>\n%1%1<Slider name=\"ZGGB-R2 Low\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetZGGB-R2 Low\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetZGGB-R2 Low</Data>\n%1%1</Slider>\n%1%1<Slider name=\"HighHeel\" invert=\"false\" small=\"0\" big=\"0\">\n%1%1%1<Data name=\"FeetHighHeel\" target=\"Feet\" local=\"true\">BHUNP 3BBB Advanced Feet.osd\\FeetHighHeel</Data>\n%1%1</Slider>\n%1</SliderSet>");
          break;
      }
      break;
    case 1:
      // More Sliders for Feet - Normal
      switch (aBody)
      {
        case BodyNameVersion::CBBE_3BBB_3BA_1_40:
        case BodyNameVersion::CBBE_3BBB_3BA_1_50:
        case BodyNameVersion::CBBE_3BBB_3BA_1_51_to_1_55:
        case BodyNameVersion::CBBE_3BBB_3BA_2_00_to_2_04:
        case BodyNameVersion::CBBE_3BBB_3BA_2_05_to_2_06:
        case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
        case BodyNameVersion::BHUNP_3BBB_2_13:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
        case BodyNameVersion::BHUNP_BBP_2_13:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
        case BodyNameVersion::BHUNP_TBBP_2_13:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
        case BodyNameVersion::BHUNP_3BBB_2_15:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_15:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_15:
        case BodyNameVersion::BHUNP_BBP_2_15:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_15:
        case BodyNameVersion::BHUNP_TBBP_2_15:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_15:
        case BodyNameVersion::BHUNP_3BBB_2_20:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_20:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_20:
        case BodyNameVersion::BHUNP_BBP_2_20:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_20:
        case BodyNameVersion::BHUNP_TBBP_2_20:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_20:
        case BodyNameVersion::BHUNP_3BBB_2_25:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_25:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_25:
        case BodyNameVersion::BHUNP_BBP_2_25:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_25:
        case BodyNameVersion::BHUNP_TBBP_2_25:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_25:
          lSliderSet = QString("");
          break;
      }
      break;
    case 2:
      // More Sliders for Feet - High Heels
      switch (aBody)
      {
        case BodyNameVersion::CBBE_3BBB_3BA_1_40:
        case BodyNameVersion::CBBE_3BBB_3BA_1_50:
        case BodyNameVersion::CBBE_3BBB_3BA_1_51_to_1_55:
        case BodyNameVersion::CBBE_3BBB_3BA_2_00_to_2_04:
        case BodyNameVersion::CBBE_3BBB_3BA_2_05_to_2_06:
        case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
        case BodyNameVersion::BHUNP_3BBB_2_13:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
        case BodyNameVersion::BHUNP_BBP_2_13:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
        case BodyNameVersion::BHUNP_TBBP_2_13:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
        case BodyNameVersion::BHUNP_3BBB_2_15:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_15:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_15:
        case BodyNameVersion::BHUNP_BBP_2_15:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_15:
        case BodyNameVersion::BHUNP_TBBP_2_15:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_15:
        case BodyNameVersion::BHUNP_3BBB_2_20:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_20:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_20:
        case BodyNameVersion::BHUNP_BBP_2_20:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_20:
        case BodyNameVersion::BHUNP_TBBP_2_20:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_20:
        case BodyNameVersion::BHUNP_3BBB_2_25:
        case BodyNameVersion::BHUNP_3BBB_Advanced_2_25:
        case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_25:
        case BodyNameVersion::BHUNP_BBP_2_25:
        case BodyNameVersion::BHUNP_BBP_Advanced_2_25:
        case BodyNameVersion::BHUNP_TBBP_2_25:
        case BodyNameVersion::BHUNP_TBBP_Advanced_2_25:
          lSliderSet = QString("");
          break;
      }
      break;
  }

  // Replace the "%1" string with identation spaces
  return lSliderSet.arg("    ");
}

QString SliderSetsHelper::getBodyBlock(const BodyNameVersion& aBody)
{
  return QString("wxcwxc");
}
