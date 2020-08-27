#include "MFBOPresetCreator.h"
#include "Utils.h"

#include <QtWidgets/QApplication>
#include <QTranslator>

int main(int argc, char* argv[])
{
  // Create the main GUI
  QApplication lMainApplication(argc, argv);
  lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getProgramVersion());
  lMainApplication.setApplicationVersion(Utils::getProgramVersion());
  lMainApplication.setWindowIcon(QIcon(":/software/icon"));

  // Open the config file
  Utils::checkSettingsFileExistence();

  // Set the language of the GUI
  auto lLanguage = Utils::parseLanguageFromSettingsFile();
  QTranslator lTranslator;
  lTranslator.load(QString("mfbopc_%1").arg(lLanguage));
  lMainApplication.installTranslator(&lTranslator);

  // Launch the application
  MFBOPresetCreator lMainWindow;
  lMainWindow.show();
  return lMainApplication.exec();
}
