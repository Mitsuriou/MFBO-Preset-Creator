#include "MFBOPresetCreator.h"
#include "Utils.h"

#include <QtWidgets/QApplication>
#include <QIcon>
#include <QTranslator>

int main(int argc, char* argv[])
{
  // Create the main GUI
  QApplication lMainApplication(argc, argv);
  lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getProgramVersion());
  lMainApplication.setApplicationVersion(Utils::getProgramVersion());
  lMainApplication.setWindowIcon(QIcon(":/software/icon"));

  auto lLanguageToSet{Utils::getShortLanguageNameFromEnum(static_cast<int>(Utils::loadSettingsFromFile().language))};
  auto lTranslator{new QTranslator()};
  if (lTranslator->load(QString("mfbopc_%1").arg(lLanguageToSet)))
  {
    lMainApplication.installTranslator(lTranslator);
  }

  // Create the main window
  MFBOPresetCreator lMainWindow(NULL, lTranslator);
  return lMainApplication.exec();
}
