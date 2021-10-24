#include "pch.h"

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
      std::wstring ToString(const BodyNameVersion& aBodyNameVersion)
      {
        return std::to_wstring(static_cast<int>(aBodyNameVersion));
      }
    }
  }
}

namespace Tests
{
  class DataListsTest : public TestClass<DataListsTest>
  {
  public:
    TEST_METHOD(GetBodyNameVersion)
    {
      // CBBE 3BBB 3BA
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 0), BodyNameVersion::CBBE_3BBB_3BA_1_50);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 1), BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 2), BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 3), BodyNameVersion::CBBE_3BBB_3BA_2_06);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 4), BodyNameVersion::INVALID_VALUE);

      // CBBE SMP 3BBB
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_SMP_3BBB, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_SMP_3BBB, 0), BodyNameVersion::CBBE_SMP_3BBB_1_2_0);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::CBBE_SMP_3BBB, 1), BodyNameVersion::INVALID_VALUE);

      // BHUNP 3BBB
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB, 0), BodyNameVersion::BHUNP_3BBB_2_20);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB, 1), BodyNameVersion::BHUNP_3BBB_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB, 2), BodyNameVersion::BHUNP_3BBB_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB, 3), BodyNameVersion::BHUNP_3BBB_2_31);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB, 4), BodyNameVersion::BHUNP_3BBB_2_35_TO_2_39);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB, 5), BodyNameVersion::INVALID_VALUE);

      // BHUNP 3BBB ADVANCED
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED, 0), BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED, 1), BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED, 2), BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED, 3), BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED, 4), BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_39);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED, 5), BodyNameVersion::INVALID_VALUE);

      // BHUNP 3BBB ADVANCED VER 2
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2, 0), BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2, 1), BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2, 2), BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2, 3), BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2, 4), BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_39);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2, 5), BodyNameVersion::INVALID_VALUE);

      // BHUNP BBP
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP, 0), BodyNameVersion::BHUNP_BBP_2_20);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP, 1), BodyNameVersion::BHUNP_BBP_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP, 2), BodyNameVersion::BHUNP_BBP_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP, 3), BodyNameVersion::BHUNP_BBP_2_31);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP, 4), BodyNameVersion::BHUNP_BBP_2_35_TO_2_39);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP, 5), BodyNameVersion::INVALID_VALUE);

      // BHUNP BBP ADVANCED
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP_ADVANCED, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP_ADVANCED, 0), BodyNameVersion::BHUNP_BBP_ADVANCED_2_20);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP_ADVANCED, 1), BodyNameVersion::BHUNP_BBP_ADVANCED_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP_ADVANCED, 2), BodyNameVersion::BHUNP_BBP_ADVANCED_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP_ADVANCED, 3), BodyNameVersion::BHUNP_BBP_ADVANCED_2_31);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP_ADVANCED, 4), BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_39);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_BBP_ADVANCED, 5), BodyNameVersion::INVALID_VALUE);

      // BHUNP TBBP
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP, 0), BodyNameVersion::BHUNP_TBBP_2_20);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP, 1), BodyNameVersion::BHUNP_TBBP_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP, 2), BodyNameVersion::BHUNP_TBBP_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP, 3), BodyNameVersion::BHUNP_TBBP_2_31);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP, 4), BodyNameVersion::BHUNP_TBBP_2_35_TO_2_39);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP, 5), BodyNameVersion::INVALID_VALUE);

      // BHUNP TBBP ADVANCED
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP_ADVANCED, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP_ADVANCED, 0), BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP_ADVANCED, 1), BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP_ADVANCED, 2), BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP_ADVANCED, 3), BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP_ADVANCED, 4), BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_39);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_TBBP_ADVANCED, 5), BodyNameVersion::INVALID_VALUE);

      // BHUNP 3BBB ADVANCED VER 2 NEVERNUDE
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 0), BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 1), BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE, 2), BodyNameVersion::INVALID_VALUE);

      // MIMIR EBONIC BODY
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::MIMIR_EBONIC_BODY, -1), BodyNameVersion::INVALID_VALUE);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::MIMIR_EBONIC_BODY, 0), BodyNameVersion::MIMIR_EBONIC_BODY_1_2);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::MIMIR_EBONIC_BODY, 1), BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX);
      Assert::AreEqual(DataLists::GetBodyNameVersion(BodyName::MIMIR_EBONIC_BODY, 2), BodyNameVersion::INVALID_VALUE);
    }

    TEST_METHOD(GetSplittedNameVersionFromBodyVersion)
    {
      // TODO
    }

    TEST_METHOD(GetQRCPathFromBodyName)
    {
      // TODO
    }
  };
}
