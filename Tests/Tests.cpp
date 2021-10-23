#include "pch.h"

#include "CppUnitTest.h"

// Files to test
#include "DataLists.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
  class Tests : public TestClass<Tests>
  {
  public:
    TEST_METHOD(TestGetBodyNameVersion)
    {
      Assert::IsTrue(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, -1) == BodyNameVersion::INVALID_VALUE);
      Assert::IsTrue(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 0) == BodyNameVersion::CBBE_3BBB_3BA_1_50);
      Assert::IsTrue(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 10) == BodyNameVersion::INVALID_VALUE);
      Assert::IsTrue(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 10) == BodyNameVersion::CBBE_3BBB_3BA_1_50);
    }
  };
}
