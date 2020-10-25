#include "MFBOPresetCreator.h"
#include "Utils.h"
#include "stdafx.h"

int main(int argc, char* argv[])
{
  auto currentExitCode{0};
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  // Create the main GUI handler
  QApplication lMainApplication(argc, argv);
  lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getSoftwareVersion());
  lMainApplication.setApplicationVersion(Utils::getSoftwareVersion());
  lMainApplication.setWindowIcon(QIcon(":/software/icon"));

  do
  {
    //// Show the splash screen
    //QPixmap lSplashScreenBackground(":/software/splashscreen");
    //lSplashScreenBackground = lSplashScreenBackground.scaled(800, 450, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //QSplashScreen lSplashScreen(lSplashScreenBackground);
    //lSplashScreen.showMessage("MFBOPC v." + Utils::getSoftwareVersion(), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    //lSplashScreen.show();

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

    // Launch the application
    currentExitCode = lMainApplication.exec();
  } while (currentExitCode == Settings::EXIT_CODE_REBOOT);

  return currentExitCode;
}
