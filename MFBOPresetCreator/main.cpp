#include "MFBOPresetCreator.h"
#include "Utils.h"
#include "stdafx.h"

int main(int argc, char* argv[])
{
  auto currentExitCode{0};

  do
  {
    // Create the main GUI handler
    QApplication lMainApplication(argc, argv);
    lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getSoftwareVersion());
    lMainApplication.setApplicationVersion(Utils::getSoftwareVersion());
    lMainApplication.setWindowIcon(QIcon(":/software/icon"));

    // Apply custom language and translation
    auto lLanguageToSet{Utils::getShortLanguageNameFromEnum(static_cast<int>(Utils::loadSettingsFromFile().language))};
    auto lTranslator{new QTranslator()};
    if (lTranslator->load(QString(":/translations/mfbopc_%1.qm").arg(lLanguageToSet)))
    {
      lMainApplication.installTranslator(lTranslator);
    }

    // Apply default Qt language and translation
    auto lQtBaseTranslator{new QTranslator()};
    if (lQtBaseTranslator->load("qt_" + lLanguageToSet + ".qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
      lMainApplication.installTranslator(lQtBaseTranslator);
    }

    // Create and show the main window
    MFBOPresetCreator lMainWindow;
    lMainWindow.show();

    // Launch the application
    currentExitCode = lMainApplication.exec();
  } while (currentExitCode == Settings::EXIT_CODE_REBOOT);

  return currentExitCode;
}
