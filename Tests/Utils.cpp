// Unit test framework
#include "CppUnitTest.h"

// Files to test
#include "DataLists.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
  class UtilsTest final : public TestClass<UtilsTest>
  {
  public:
    TEST_METHOD(bool__QRCResourceExists__QString)
    {
      //------//
      // BODY //
      //------//
      // CBBE_3BA_3BBB
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_1_50, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // CBBE_3BA_3BBB_NEVERNUDE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_18_AND_2_30_AND_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // CBBE_3BA_3BBB_UNDERWEAR
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_18_AND_2_30_AND_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_3BBB
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_3BBB_ADVANCED
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_3BBB_ADVANCED_VER_2
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_3BBB_ADVANCED_VER_3
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_BBP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_2_20, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_BBP_ADVANCED
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_2_20, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_BBP_ADVANCED_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_TBBP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_2_20, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_TBBP_ADVANCED
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_TBBP_ADVANCED_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_35_TO_2_42, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_00_TO_3_02, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_03, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_04, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_10, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_11, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // CBBE_SMP_3BBB
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_SMP_3BBB_1_2_0, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // COCO_BODY_CBBE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V4, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6_8, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6_9_AND_V6_95, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // COCO_BODY_UUNP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V3, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V4, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6_8, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6_9_AND_V6_95, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // MIMIR_EBONIC_BODY
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::MIMIR_EBONIC_BODY_1_2, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_3BBB
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_3BBB_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_3BBB_NO_THIGH_JIGGLE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_BBP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_BBP_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_BBP_NO_THIGH_JIGGLE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_BBP_NO_THIGH_JIGGLE_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_TBBP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_TBBP_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_TBBP_NO_THIGH_JIGGLE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_ALTERBODY_3BBB
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_ALTERBODY_BBP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_ALTERBODY_BBP_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_ALTERBODY_TBBP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));
      // ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13, FeetNameVersion::_INVALID_VALUE, MeshPartType::BODY)));

      //------//
      // FEET //
      //------//
      //
      // Warning: not exhaustive!

      // CBBE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_SMP_3BBB_1_2_0, FeetNameVersion::CBBE_1_6_1, MeshPartType::FEET)));
      // BHUNP 3BBB Advanced Feet
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::BHUNP_3BBB_ADVANCED_2_20, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_25, FeetNameVersion::BHUNP_3BBB_ADVANCED_2_25, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_30, FeetNameVersion::BHUNP_3BBB_ADVANCED_2_30, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_31, FeetNameVersion::BHUNP_3BBB_ADVANCED_2_31, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42, FeetNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_00_TO_3_02, FeetNameVersion::BHUNP_3BBB_ADVANCED_3_00_TO_3_02, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_03, FeetNameVersion::BHUNP_3BBB_ADVANCED_3_03, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_04, FeetNameVersion::BHUNP_3BBB_ADVANCED_3_04, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_10, FeetNameVersion::BHUNP_3BBB_ADVANCED_3_10, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_3_11, FeetNameVersion::BHUNP_3BBB_ADVANCED_3_11, MeshPartType::FEET)));
      // CBBE_3BA_3BBB
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_1_50, FeetNameVersion::CBBE_3BA_3BBB_1_50, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55, FeetNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04, FeetNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12, FeetNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31, FeetNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31, MeshPartType::FEET)));
      // COCO_BODY_CBBE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V4, FeetNameVersion::COCO_BODY_NORMAL_CBBE_V4, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6, FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6_8, FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6_8, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6_9_AND_V6_95, FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6_9_AND_V6_95, MeshPartType::FEET)));
      // COCO_BODY_UUNP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V3, FeetNameVersion::COCO_BODY_NORMAL_UUNP_V3, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V4, FeetNameVersion::COCO_BODY_NORMAL_UUNP_V4, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6, FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6_8, FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6_8, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6_9_AND_V6_95, FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6_9_AND_V6_95, MeshPartType::FEET)));
      // COCO_BODY_CBBE
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6, FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6_8, FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6_8, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_CBBE_V6_9_AND_V6_95, FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6_9_AND_V6_95, MeshPartType::FEET)));
      // COCO_BODY_UUNP
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6, FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6_8, FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6_8, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::COCO_BODY_UUNP_V6_9_AND_V6_95, FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6_9_AND_V6_95, MeshPartType::FEET)));
      // MIMIR_EBONIC_BODY
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::MIMIR_EBONIC_BODY_1_2, FeetNameVersion::MIMIR_EBONIC_BODY_1_2, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX, FeetNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX, MeshPartType::FEET)));
      // ASDASDF
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::ASDASDF_3BBB_1_13, FeetNameVersion::ASDASDF_1_13, MeshPartType::FEET)));
      // MORE_SLIDERS_FOR_FEET
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_1_50, FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::CBBE_3BA_3BBB_1_50, FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1, MeshPartType::FEET)));
      // HG_FEET
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::HG_FEET_1, MeshPartType::FEET)));
      // KHRYSAMERE_HG_FEET
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::KHRYSAMERE_HG_FEET_1_0, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::KHRYSAMERE_HG_FEET_1_3_3, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::KHRYSAMERE_HG_FEET_1_4, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::KHRYSAMERE_HG_FEET_1_5, MeshPartType::FEET)));
      Assert::IsTrue(Utils::QRCResourceExists(DataLists::GetQRCPathForResource(BodyNameVersion::BHUNP_3BBB_2_20, FeetNameVersion::KHRYSAMERE_HG_FEET_1_6, MeshPartType::FEET)));
    }
  };
}
