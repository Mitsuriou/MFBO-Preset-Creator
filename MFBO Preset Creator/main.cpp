#include "MFBOPresetCreator.h"
#include "Utils.h"

#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QScreen>
#include <QRect>
#include <QFont>

int main(int argc, char* argv[])
{
  // Create the main GUI
  QApplication lMainApplication(argc, argv);
  lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getProgramVersion());
  lMainApplication.setApplicationVersion(Utils::getProgramVersion());
  lMainApplication.setWindowIcon(QIcon(":/software/icon"));

  // Open and parse the config file
  auto lSettings = Utils::loadSettingsFromFile();

  // Set the language of the GUI
  QTranslator lTranslator;
  lTranslator.load(QString("mfbopc_%1").arg(lSettings.language));
  lMainApplication.installTranslator(&lTranslator);

  // Create the main window
  MFBOPresetCreator lMainWindow;

  // Set the font properties
  //QFont lFont(lSettings.fontFamily, lSettings.fontSize, -1, false);
  //lMainWindow.setFont(lFont);

  // Set the size of the window
  QScreen* lScreen{QGuiApplication::primaryScreen()};
  QRect lScreenGeom{lScreen->geometry()};

  if (lSettings.defaulWindowWidth < lScreenGeom.width() && lSettings.defaulWindowHeight < lScreenGeom.height())
  {
    lMainWindow.resize(lSettings.defaulWindowWidth, lSettings.defaulWindowHeight);
    lMainWindow.show();
  }
  else
  {
    lMainWindow.showMaximized();
  }

  return lMainApplication.exec();
}
