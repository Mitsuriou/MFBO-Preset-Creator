#include "pch.h"
#include "CppUnitTest.h"

#include "Enum.h"

// Files to test
#include "DataLists.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
  class Tests : public TestClass<Tests>
  {
  public:
    TEST_METHOD(TestMethod1)
    {
      Logger::WriteMessage("Tests");
      Assert::IsTrue(DataLists::GetBodyNameVersion(BodyName::CBBE_3BBB_3BA, 0) == BodyNameVersion::CBBE_3BBB_3BA_1_50);
    }
  };
}
