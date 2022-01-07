// Unit test framework
#include "CppUnitTest.h"

// Files to test
#include "DataLists.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
  namespace VisualStudio
  {
    namespace CppUnitTestFramework
    {
      // Body enumerations
      template<>
      inline std::wstring __cdecl ToString<BodyName>(const BodyName& t)
      {
        return std::to_wstring(static_cast<int>(t));
      }

      template<>
      inline std::wstring __cdecl ToString<BodyVariant>(const BodyVariant& t)
      {
        return std::to_wstring(static_cast<int>(t));
      }

      template<>
      inline std::wstring __cdecl ToString<BodyNameVersion>(const BodyNameVersion& t)
      {
        return std::to_wstring(static_cast<int>(t));
      }

      // Feet enumerations
      template<>
      inline std::wstring __cdecl ToString<FeetName>(const FeetName& t)
      {
        return std::to_wstring(static_cast<int>(t));
      }

      template<>
      inline std::wstring __cdecl ToString<FeetVariant>(const FeetVariant& t)
      {
        return std::to_wstring(static_cast<int>(t));
      }

      template<>
      inline std::wstring __cdecl ToString<FeetNameVersion>(const FeetNameVersion& t)
      {
        return std::to_wstring(static_cast<int>(t));
      }
    }
  }
}

namespace Tests
{
  class DataListsTest : public TestClass<DataListsTest>
  {
  public:
    TEST_METHOD(BodyName__GetName__BodyVariant)
    {
      Assert::AreEqual(BodyName::_INVALID_VALUE, DataLists::GetName(BodyVariant::_INVALID_VALUE));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_3BBB));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_BBP));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_BBP_ADVANCED));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_TBBP));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_TBBP_ADVANCED));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE));
      Assert::AreEqual(BodyName::CBBE_SMP_3BBB, DataLists::GetName(BodyVariant::CBBE_SMP_3BBB));
      Assert::AreEqual(BodyName::COCO_BODY_CBBE, DataLists::GetName(BodyVariant::COCO_BODY_CBBE));
      Assert::AreEqual(BodyName::COCO_BODY_UUNP, DataLists::GetName(BodyVariant::COCO_BODY_UUNP));
      Assert::AreEqual(BodyName::MIMIR_EBONIC_BODY, DataLists::GetName(BodyVariant::MIMIR_EBONIC_BODY));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_3BBB));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_BBP));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_TBBP));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP));
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE));
    }

    TEST_METHOD(FeetName__GetName__FeetVariant)
    {
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(FeetVariant::_INVALID_VALUE));
      Assert::AreEqual(FeetName::CBBE, DataLists::GetName(FeetVariant::CBBE));
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(FeetVariant::BHUNP));
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(FeetVariant::CBBE_3BA_3BBB));
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(FeetVariant::COCO_BODY_CBBE));
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(FeetVariant::COCO_BODY_UUNP));
      Assert::AreEqual(FeetName::MIMIR_EBONIC_BODY, DataLists::GetName(FeetVariant::MIMIR_EBONIC_BODY));
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(FeetVariant::ASDASDF));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP));
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(FeetVariant::HG_FEET));
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(FeetVariant::KHRYSAMERE_HG_FEET));
    }

    TEST_METHOD(BodyName__GetName__BodyNameVersion)
    {
      Assert::AreEqual(BodyName::_INVALID_VALUE, DataLists::GetName(BodyNameVersion::_INVALID_VALUE));

      Assert::AreEqual(BodyName::_INVALID_VALUE, DataLists::GetName(BodyNameVersion::_INVALID_VALUE));
      // CBBE_3BA_3BBB
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_1_50));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_2_06));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_2_11_AND_2_12));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_17));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_17));
      Assert::AreEqual(BodyName::CBBE_3BA_3BBB, DataLists::GetName(BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_17));
      // BHUNP_3BBB
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_2_20));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_2_30));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_2_31));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42));
      // BHUNP_3BBB_ADVANCED
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42));
      // BHUNP_3BBB_ADVANCED_VER_2
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42));
      // BHUNP_BBP
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_2_20));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_2_30));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_2_31));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_2_35_TO_2_42));
      // BHUNP_BBP_ADVANCED
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_ADVANCED_2_20));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_ADVANCED_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_ADVANCED_2_30));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_ADVANCED_2_31));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_42));
      // BHUNP_TBBP
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_2_20));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_2_30));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_2_31));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_2_35_TO_2_42));
      // BHUNP_TBBP_ADVANCED
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_42));
      // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25));
      Assert::AreEqual(BodyName::BHUNP_UUNP_NEXT_GENERATION, DataLists::GetName(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30));
      // CBBE_SMP_3BBB
      Assert::AreEqual(BodyName::CBBE_SMP_3BBB, DataLists::GetName(BodyNameVersion::CBBE_SMP_3BBB_1_2_0));
      // COCO_BODY_CBBE
      Assert::AreEqual(BodyName::COCO_BODY_CBBE, DataLists::GetName(BodyNameVersion::COCO_BODY_CBBE_V4));
      Assert::AreEqual(BodyName::COCO_BODY_CBBE, DataLists::GetName(BodyNameVersion::COCO_BODY_CBBE_V6));
      // COCO_BODY_UUNP
      Assert::AreEqual(BodyName::COCO_BODY_UUNP, DataLists::GetName(BodyNameVersion::COCO_BODY_UUNP_V3));
      Assert::AreEqual(BodyName::COCO_BODY_UUNP, DataLists::GetName(BodyNameVersion::COCO_BODY_UUNP_V4));
      Assert::AreEqual(BodyName::COCO_BODY_UUNP, DataLists::GetName(BodyNameVersion::COCO_BODY_UUNP_V6));
      // MIMIR_EBONIC_BODY
      Assert::AreEqual(BodyName::MIMIR_EBONIC_BODY, DataLists::GetName(BodyNameVersion::MIMIR_EBONIC_BODY_1_2));
      Assert::AreEqual(BodyName::MIMIR_EBONIC_BODY, DataLists::GetName(BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX));
      // ASDASDF_3BBB
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_3BBB_1_13));
      // ASDASDF_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13));
      // ASDASDF_BBP
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_BBP_1_13));
      // ASDASDF_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_BBP_NO_THIGH_JIGGLE_1_13));
      // ASDASDF_TBBP
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_TBBP_1_13));
      // ASDASDF_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13));
      // ASDASDF_ALTERBODY_3BBB
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_1_13));
      // ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13));
      // ASDASDF_ALTERBODY_BBP
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_ALTERBODY_BBP_1_13));
      // ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13));
      // ASDASDF_ALTERBODY_TBBP
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_1_13));
      // ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyName::BHUNP_LITE_ASDASDF, DataLists::GetName(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13));
    }

    TEST_METHOD(FeetName__GetName__FeetNameVersion)
    {
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(FeetNameVersion::_INVALID_VALUE));
      // CBBE
      Assert::AreEqual(FeetName::CBBE, DataLists::GetName(FeetNameVersion::CBBE_1_6_1));
      // BHUNP
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(FeetNameVersion::BHUNP_3BBB_2_20));
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(FeetNameVersion::BHUNP_3BBB_2_25));
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(FeetNameVersion::BHUNP_3BBB_2_30));
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(FeetNameVersion::BHUNP_3BBB_2_31));
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(FeetNameVersion::BHUNP_3BBB_2_35_TO_2_42));
      // CBBE_3BA_3BBB
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(FeetNameVersion::CBBE_3BA_3BBB_1_50));
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(FeetNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55));
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(FeetNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04));
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(FeetNameVersion::CBBE_3BA_3BBB_2_06));
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(FeetNameVersion::CBBE_3BA_3BBB_2_11_AND_2_12));
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(FeetNameVersion::CBBE_3BA_3BBB_2_13_TO_2_17));
      // COCO_BODY_CBBE
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(FeetNameVersion::COCO_BODY_CBBE_V4));
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(FeetNameVersion::COCO_BODY_CBBE_V6));
      // COCO_BODY_UUNP
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(FeetNameVersion::COCO_BODY_UUNP_V3));
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(FeetNameVersion::COCO_BODY_UUNP_V4));
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(FeetNameVersion::COCO_BODY_UUNP_V6));
      // MIMIR_EBONIC_BODY
      Assert::AreEqual(FeetName::MIMIR_EBONIC_BODY, DataLists::GetName(FeetNameVersion::MIMIR_EBONIC_BODY_1_2));
      Assert::AreEqual(FeetName::MIMIR_EBONIC_BODY, DataLists::GetName(FeetNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX));
      // ASDASDF
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(FeetNameVersion::ASDASDF_1_13));
      // MORE_SLIDERS_FOR_FEET
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1));
      // HG_FEET
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(FeetNameVersion::HG_FEET_1));
      // KHRYSAMERE_HG_FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(FeetNameVersion::KHRYSAMERE_HG_FEET_1_0));
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(FeetNameVersion::KHRYSAMERE_HG_FEET_1_3_3));
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(FeetNameVersion::KHRYSAMERE_HG_FEET_1_4));
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(FeetNameVersion::KHRYSAMERE_HG_FEET_1_5));
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(FeetNameVersion::KHRYSAMERE_HG_FEET_1_6));
    }

    TEST_METHOD(FeetName__GetName__BodyVariant_int)
    {
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::_INVALID_VALUE, -1)); // invalid
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::_INVALID_VALUE, 0));  // invalid
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::_INVALID_VALUE, 5));  // invalid

      // BodyVariant::CBBE_3BA_3BBB
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB, -1));       // invalid
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB, 0));         // CBBE 3BA 3BBB
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB, 1)); // MSF
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB, 2));        // invalid

      // BodyVariant::CBBE_3BA_3BBB_NEVERNUDE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, -1));       // invalid
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, 0));         // CBBE 3BA 3BBB
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, 1)); // MSF
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, 2));        // invalid

      // BodyVariant::CBBE_3BA_3BBB_UNDERWEAR
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, -1));       // invalid
      Assert::AreEqual(FeetName::CBBE_3BA_3BBB, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, 0));         // CBBE 3BA 3BBB
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, 1)); // MSF
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, 2));        // invalid

      // BodyVariant::BHUNP_3BBB
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_3BBB, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB, 4));        // invalid

      // BodyVariant::BHUNP_3BBB_ADVANCED
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED, 4));        // invalid

      // BodyVariant::BHUNP_3BBB_ADVANCED_VER_2
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 4));        // invalid

      // BodyVariant::BHUNP_BBP
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_BBP, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_BBP, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_BBP, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_BBP, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_BBP, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_BBP, 4));        // invalid

      // BodyVariant::BHUNP_BBP_ADVANCED
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_BBP_ADVANCED, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_BBP_ADVANCED, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_BBP_ADVANCED, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_BBP_ADVANCED, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_BBP_ADVANCED, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_BBP_ADVANCED, 4));        // invalid

      // BodyVariant::BHUNP_TBBP
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_TBBP, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_TBBP, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_TBBP, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_TBBP, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_TBBP, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_TBBP, 4));        // invalid

      // BodyVariant::BHUNP_TBBP_ADVANCED
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_TBBP_ADVANCED, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_TBBP_ADVANCED, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_TBBP_ADVANCED, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_TBBP_ADVANCED, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_TBBP_ADVANCED, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_TBBP_ADVANCED, 4));        // invalid

      // BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, -1));       // invalid
      Assert::AreEqual(FeetName::BHUNP, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 0));                 // BHUNP
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 4));        // invalid

      // BodyVariant::CBBE_SMP_3BBB
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_SMP_3BBB, -1));       // invalid
      Assert::AreEqual(FeetName::CBBE, DataLists::GetName(BodyVariant::CBBE_SMP_3BBB, 0));                  // CBBE
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::CBBE_SMP_3BBB, 1)); // MSF
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::CBBE_SMP_3BBB, 2));        // invalid

      // BodyVariant::COCO_BODY_CBBE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::COCO_BODY_CBBE, -1));       // invalid
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(BodyVariant::COCO_BODY_CBBE, 0));             // COCO
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::COCO_BODY_CBBE, 1)); // MSF
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::COCO_BODY_CBBE, 2));        // invalid

      // BodyVariant::COCO_BODY_UUNP
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::COCO_BODY_UUNP, -1));       // invalid
      Assert::AreEqual(FeetName::COCO_BODY, DataLists::GetName(BodyVariant::COCO_BODY_UUNP, 0));             // COCO
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::COCO_BODY_UUNP, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::COCO_BODY_UUNP, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::COCO_BODY_UUNP, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::COCO_BODY_UUNP, 4));        // invalid

      // BodyVariant::MIMIR_EBONIC_BODY
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::MIMIR_EBONIC_BODY, -1));       // invalid
      Assert::AreEqual(FeetName::MIMIR_EBONIC_BODY, DataLists::GetName(BodyVariant::MIMIR_EBONIC_BODY, 0));     // MIMIR
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::MIMIR_EBONIC_BODY, 1)); // MSF
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::MIMIR_EBONIC_BODY, 2));        // invalid

      // BodyVariant::ASDASDF_3BBB
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_3BBB, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_3BBB, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_3BBB, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_3BBB, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_3BBB, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_3BBB, 4));        // invalid

      // BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, 4));        // invalid

      // BodyVariant::ASDASDF_BBP
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_BBP, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_BBP, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_BBP, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_BBP, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_BBP, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_BBP, 4));        // invalid

      // BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, 4));        // invalid

      // BodyVariant::ASDASDF_TBBP
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_TBBP, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_TBBP, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_TBBP, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_TBBP, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_TBBP, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_TBBP, 4));        // invalid

      // BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, 4));        // invalid

      // BodyVariant::ASDASDF_ALTERBODY_3BBB
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB, 4));        // invalid

      // BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, 4));        // invalid

      // BodyVariant::ASDASDF_ALTERBODY_BBP
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP, 4));        // invalid

      // BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, 4));        // invalid

      // BodyVariant::ASDASDF_ALTERBODY_TBBP
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP, 4));        // invalid

      // BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, -1));       // invalid
      Assert::AreEqual(FeetName::ASDASDF, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, 0));               // ASDASDF
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, 1)); // MSF
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, 2));               // HG FEET
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, 3));    // KHRYSAMERE HG FEET
      Assert::AreEqual(FeetName::_INVALID_VALUE, DataLists::GetName(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, 4));        // invalid
    }

    TEST_METHOD(BodyVariant__GetVariant__BodyNameVersion)
    {
      Assert::AreEqual(BodyVariant::_INVALID_VALUE, DataLists::GetVariant(BodyNameVersion::_INVALID_VALUE));

      // CBBE_3BA_3BBB
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_1_50));
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55));
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04));
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_2_06));
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_2_11_AND_2_12));
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_17));

      // CBBE_3BA_3BBB_NEVERNUDE
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_17));

      // CBBE_3BA_3BBB_UNDERWEAR
      Assert::AreEqual(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, DataLists::GetVariant(BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_17));

      // BHUNP_3BBB
      Assert::AreEqual(BodyVariant::BHUNP_3BBB, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_2_20));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_2_30));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_2_31));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42));

      // BHUNP_3BBB_ADVANCED
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42));

      // BHUNP_3BBB_ADVANCED_VER_2
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42));

      // BHUNP_BBP
      Assert::AreEqual(BodyVariant::BHUNP_BBP, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_2_20));
      Assert::AreEqual(BodyVariant::BHUNP_BBP, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_BBP, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_2_30));
      Assert::AreEqual(BodyVariant::BHUNP_BBP, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_2_31));
      Assert::AreEqual(BodyVariant::BHUNP_BBP, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_2_35_TO_2_42));

      // BHUNP_BBP_ADVANCED
      Assert::AreEqual(BodyVariant::BHUNP_BBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_ADVANCED_2_20));
      Assert::AreEqual(BodyVariant::BHUNP_BBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_ADVANCED_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_BBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_ADVANCED_2_30));
      Assert::AreEqual(BodyVariant::BHUNP_BBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_ADVANCED_2_31));
      Assert::AreEqual(BodyVariant::BHUNP_BBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_42));

      // BHUNP_TBBP
      Assert::AreEqual(BodyVariant::BHUNP_TBBP, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_2_20));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_2_30));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_2_31));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_2_35_TO_2_42));

      // BHUNP_TBBP_ADVANCED
      Assert::AreEqual(BodyVariant::BHUNP_TBBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31));
      Assert::AreEqual(BodyVariant::BHUNP_TBBP_ADVANCED, DataLists::GetVariant(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_42));

      // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25));
      Assert::AreEqual(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, DataLists::GetVariant(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30));

      // CBBE_SMP_3BBB
      Assert::AreEqual(BodyVariant::CBBE_SMP_3BBB, DataLists::GetVariant(BodyNameVersion::CBBE_SMP_3BBB_1_2_0));

      // COCO_BODY_CBBE
      Assert::AreEqual(BodyVariant::COCO_BODY_CBBE, DataLists::GetVariant(BodyNameVersion::COCO_BODY_CBBE_V4));
      Assert::AreEqual(BodyVariant::COCO_BODY_CBBE, DataLists::GetVariant(BodyNameVersion::COCO_BODY_CBBE_V6));

      // COCO_BODY_UUNP
      Assert::AreEqual(BodyVariant::COCO_BODY_UUNP, DataLists::GetVariant(BodyNameVersion::COCO_BODY_UUNP_V3));
      Assert::AreEqual(BodyVariant::COCO_BODY_UUNP, DataLists::GetVariant(BodyNameVersion::COCO_BODY_UUNP_V4));
      Assert::AreEqual(BodyVariant::COCO_BODY_UUNP, DataLists::GetVariant(BodyNameVersion::COCO_BODY_UUNP_V6));

      // MIMIR_EBONIC_BODY
      Assert::AreEqual(BodyVariant::MIMIR_EBONIC_BODY, DataLists::GetVariant(BodyNameVersion::MIMIR_EBONIC_BODY_1_2));
      Assert::AreEqual(BodyVariant::MIMIR_EBONIC_BODY, DataLists::GetVariant(BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX));

      // ASDASDF_3BBB
      Assert::AreEqual(BodyVariant::ASDASDF_3BBB, DataLists::GetVariant(BodyNameVersion::ASDASDF_3BBB_1_13));

      // ASDASDF_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, DataLists::GetVariant(BodyNameVersion::ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13));

      // ASDASDF_BBP
      Assert::AreEqual(BodyVariant::ASDASDF_BBP, DataLists::GetVariant(BodyNameVersion::ASDASDF_BBP_1_13));

      // ASDASDF_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, DataLists::GetVariant(BodyNameVersion::ASDASDF_BBP_NO_THIGH_JIGGLE_1_13));

      // ASDASDF_TBBP
      Assert::AreEqual(BodyVariant::ASDASDF_TBBP, DataLists::GetVariant(BodyNameVersion::ASDASDF_TBBP_1_13));

      // ASDASDF_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, DataLists::GetVariant(BodyNameVersion::ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13));

      // ASDASDF_ALTERBODY_3BBB
      Assert::AreEqual(BodyVariant::ASDASDF_ALTERBODY_3BBB, DataLists::GetVariant(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_1_13));

      // ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, DataLists::GetVariant(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13));

      // ASDASDF_ALTERBODY_BBP
      Assert::AreEqual(BodyVariant::ASDASDF_ALTERBODY_BBP, DataLists::GetVariant(BodyNameVersion::ASDASDF_ALTERBODY_BBP_1_13));

      // ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, DataLists::GetVariant(BodyNameVersion::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13));

      // ASDASDF_ALTERBODY_TBBP
      Assert::AreEqual(BodyVariant::ASDASDF_ALTERBODY_TBBP, DataLists::GetVariant(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_1_13));

      // ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, DataLists::GetVariant(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13));
    }

    TEST_METHOD(BodyVariant__GetVariant__BodyName_int_int)
    {
      // TODO
    }

    TEST_METHOD(FeetVariant__GetVariant__FeetNameVersion)
    {
      // TODO
    }

    TEST_METHOD(FeetVariant__GetVariant__FeetName_int_int_bool)
    {
      // TODO
    }

    TEST_METHOD(BodyNameVersion__GetBodyNameVersion__BodyVariant__int)
    {
      // CBBE_3BA_3BBB
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, -1));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_1_50, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, 0));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, 1));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, 2));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_2_06, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, 3));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_2_11_AND_2_12, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, 4));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_17, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, 5));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB, 6));

      // CBBE_3BA_3BBB_NEVERNUDE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, -1));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, 4));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_17, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, 5));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_NEVERNUDE, 6));

      // CBBE_3BA_3BBB_UNDERWEAR
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, -1));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, 4));
      Assert::AreEqual(BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_17, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, 5));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_3BA_3BBB_UNDERWEAR, 6));

      // BHUNP_3BBB
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB, -1));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_2_20, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB, 2));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_2_31, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB, 3));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB, 4));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB, 5));

      // BHUNP_3BBB_ADVANCED
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED, -1));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED, 2));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED, 3));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED, 4));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED, 5));

      // BHUNP_3BBB_ADVANCED_VER_2
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, -1));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 2));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 3));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 4));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2, 5));

      // BHUNP_BBP
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP, -1));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_2_20, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP, 2));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_2_31, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP, 3));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_2_35_TO_2_42, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP, 4));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP, 5));

      // BHUNP_BBP_ADVANCED
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP_ADVANCED, -1));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_ADVANCED_2_20, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP_ADVANCED, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_ADVANCED_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP_ADVANCED, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_ADVANCED_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP_ADVANCED, 2));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_ADVANCED_2_31, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP_ADVANCED, 3));
      Assert::AreEqual(BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_42, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP_ADVANCED, 4));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_BBP_ADVANCED, 5));

      // BHUNP_TBBP
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP, -1));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_2_20, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP, 2));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_2_31, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP, 3));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_2_35_TO_2_42, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP, 4));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP, 5));

      // BHUNP_TBBP_ADVANCED
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP_ADVANCED, -1));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP_ADVANCED, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP_ADVANCED, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP_ADVANCED, 2));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP_ADVANCED, 3));
      Assert::AreEqual(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_42, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP_ADVANCED, 4));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_TBBP_ADVANCED, 5));

      // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, -1));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 0));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 1));
      Assert::AreEqual(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 2));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 3));

      // CBBE_SMP_3BBB
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_SMP_3BBB, -1));
      Assert::AreEqual(BodyNameVersion::CBBE_SMP_3BBB_1_2_0, DataLists::GetBodyNameVersion(BodyVariant::CBBE_SMP_3BBB, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::CBBE_SMP_3BBB, 1));

      // COCO_BODY_CBBE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_CBBE, -1));
      Assert::AreEqual(BodyNameVersion::COCO_BODY_CBBE_V4, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_CBBE, 0));
      Assert::AreEqual(BodyNameVersion::COCO_BODY_CBBE_V6, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_CBBE, 1));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_CBBE, 2));

      // COCO_BODY_UUNP
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_UUNP, -1));
      Assert::AreEqual(BodyNameVersion::COCO_BODY_UUNP_V3, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_UUNP, 0));
      Assert::AreEqual(BodyNameVersion::COCO_BODY_UUNP_V4, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_UUNP, 1));
      Assert::AreEqual(BodyNameVersion::COCO_BODY_UUNP_V6, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_UUNP, 2));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::COCO_BODY_UUNP, 3));

      // MIMIR_EBONIC_BODY
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::MIMIR_EBONIC_BODY, -1));
      Assert::AreEqual(BodyNameVersion::MIMIR_EBONIC_BODY_1_2, DataLists::GetBodyNameVersion(BodyVariant::MIMIR_EBONIC_BODY, 0));
      Assert::AreEqual(BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX, DataLists::GetBodyNameVersion(BodyVariant::MIMIR_EBONIC_BODY, 1));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::MIMIR_EBONIC_BODY, 2));

      // ASDASDF_3BBB
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_3BBB, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_3BBB_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_3BBB, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_3BBB, 1));

      // ASDASDF_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE, 1));

      // ASDASDF_BBP
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_BBP, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_BBP_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_BBP, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_BBP, 1));

      // ASDASDF_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_BBP_NO_THIGH_JIGGLE_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE, 1));

      // ASDASDF_TBBP
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_TBBP, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_TBBP_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_TBBP, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_TBBP, 1));

      // ASDASDF_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE, 1));

      // ASDASDF_ALTERBODY_3BBB
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_3BBB, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_3BBB, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_3BBB, 1));

      // ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE, 1));

      // ASDASDF_ALTERBODY_BBP
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_BBP, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_ALTERBODY_BBP_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_BBP, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_BBP, 1));

      // ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE, 1));

      // ASDASDF_ALTERBODY_TBBP
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_TBBP, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_TBBP, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_TBBP, 1));

      // ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, -1));
      Assert::AreEqual(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, 0));
      Assert::AreEqual(BodyNameVersion::_INVALID_VALUE, DataLists::GetBodyNameVersion(BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE, 1));
    }
  };
}
