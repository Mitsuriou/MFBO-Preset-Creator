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
      std::wstring ToString(const BodyName& aBodyName)
      {
        return std::to_wstring(static_cast<int>(aBodyName));
      }

      std::wstring ToString(const BodyVariant& aBodyVariant)
      {
        return std::to_wstring(static_cast<int>(aBodyVariant));
      }

      std::wstring ToString(const BodyNameVersion& aBodyNameVersion)
      {
        return std::to_wstring(static_cast<int>(aBodyNameVersion));
      }

      // Feet enumerations
      std::wstring ToString(const FeetName& aFeetName)
      {
        return std::to_wstring(static_cast<int>(aFeetName));
      }

      std::wstring ToString(const FeetVariant& aFeetVariant)
      {
        return std::to_wstring(static_cast<int>(aFeetVariant));
      }

      std::wstring ToString(const FeetNameVersion& aFeetNameVersion)
      {
        return std::to_wstring(static_cast<int>(aFeetNameVersion));
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
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE));
      Assert::AreEqual(FeetName::MORE_SLIDERS_FOR_FEET, DataLists::GetName(FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP));
      Assert::AreEqual(FeetName::HG_FEET, DataLists::GetName(FeetVariant::HG_FEET));
      Assert::AreEqual(FeetName::KHRYSAMERE_HG_FEET, DataLists::GetName(FeetVariant::KHRYSAMERE_HG_FEET));
    }
  };
}
