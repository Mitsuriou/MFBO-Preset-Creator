#include "MFBOPresetCreator.h"
#include "Utils.h"

#include <QtWidgets/QApplication>
//#include <QTranslator>

int main(int argc, char* argv[])
{
  // Create the main GUI
  QApplication lMainApplication(argc, argv);
  lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getProgramVersion());
  lMainApplication.setApplicationVersion(Utils::getProgramVersion());
  lMainApplication.setWindowIcon(QIcon(":/software/icon"));

  // Set the language of the GUI
  //QTranslator lTranslator;
  //lTranslator.load("mfbopc_fr");
  //lMainApplication.installTranslator(&lTranslator);

  // Launch the application
  MFBOPresetCreator lMainWindow;
  lMainWindow.show();
  return lMainApplication.exec();
}
