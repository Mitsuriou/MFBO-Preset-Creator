#include "MFBOPresetCreator.h"
#include "Utils.h"

#include <QtWidgets/QApplication>
#include <QIcon>
#include <QTranslator>

int main(int argc, char* argv[])
{
  auto currentExitCode{0};

  do
  {
    // Create the main GUI handler
    QApplication lMainApplication(argc, argv);
    lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getProgramVersion());
    lMainApplication.setApplicationVersion(Utils::getProgramVersion());
    lMainApplication.setWindowIcon(QIcon(":/software/icon"));

    auto lLanguageToSet{Utils::getShortLanguageNameFromEnum(static_cast<int>(Utils::loadSettingsFromFile().language))};
    auto lTranslator{new QTranslator()};
    if (lTranslator->load(QString(":/translations/mfbopc_%1.qm").arg(lLanguageToSet)))
    {
      lMainApplication.installTranslator(lTranslator);
    }

    // Create and show the main window
    MFBOPresetCreator lMainWindow(NULL, lTranslator);
    lMainWindow.show();

    // Launch the application
    currentExitCode = lMainApplication.exec();
  } while (currentExitCode == Settings::EXIT_CODE_REBOOT);

  return currentExitCode;
}
