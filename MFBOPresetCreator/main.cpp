#include "MFBOPresetCreator.h"
#include "Utils.h"
#include "stdafx.h"

int main(int argc, char* argv[])
{
  auto currentExitCode{0};
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  do
  {
    // Create the main GUI handler
    QApplication lMainApplication(argc, argv);
    lMainApplication.setApplicationDisplayName("MFBOPC (v." + Utils::getApplicationVersion() + ")");
    lMainApplication.setApplicationVersion(Utils::getApplicationVersion());
    lMainApplication.setWindowIcon(QIcon(QPixmap(":/application/icon")));

    // Show the splash screen
    QPixmap lSplashScreenBackground(":/application/splashscreen");

    QSplashScreen lSplashScreen(lSplashScreenBackground.scaled(800, 450));
    lSplashScreen.showMessage("MFBOPC (v." + Utils::getApplicationVersion() + ")", Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lSplashScreen.show();

    lMainApplication.processEvents();

    // Read settings file
    auto lSettings{Utils::loadSettingsFromFile()};

    // Apply custom language and translation
    auto lLanguageToSet{Utils::getShortLanguageNameFromEnum(static_cast<int>(lSettings.language))};
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
    MFBOPresetCreator lMainWindow(lSettings);

    // Make the splash screen disappear when the main window is displayed
    lSplashScreen.finish(&lMainWindow);

    // Launch the application
    currentExitCode = lMainApplication.exec();
  } while (currentExitCode == Settings::EXIT_CODE_REBOOT);

  return currentExitCode;
}
