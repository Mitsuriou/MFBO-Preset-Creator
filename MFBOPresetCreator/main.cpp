#include "MFBOPresetCreator.h"
#include "Utils.h"
#include "stdafx.h"

int main(int argc, char* argv[])
{
  auto currentExitCode{0};

  do
  {
    // Reset the value
    Utils::RESTART_PENDING = false;

    // Create the main GUI handler
    QApplication lMainApplication(argc, argv);
    lMainApplication.setApplicationDisplayName("MFBOPC (v." + Utils::getApplicationVersion() + ")");
    lMainApplication.setApplicationVersion(Utils::getApplicationVersion());
    lMainApplication.setWindowIcon(QIcon(QPixmap(":/application/icon")));
    lMainApplication.setAttribute(Qt::AA_EnableHighDpiScaling);

    // Set the codec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Embed custom fonts
    QFontDatabase::addApplicationFont(":/fonts/Roboto");

    // Show the splash screen
    QPixmap lSplashScreenBackground(":/application/splashscreen");

    QSplashScreen lSplashScreen(lSplashScreenBackground.scaled(800, 450));
    lSplashScreen.showMessage("MFBOPC (v." + Utils::getApplicationVersion() + ")", Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lSplashScreen.show();

    // Refresh the screen to show the splash screen event though the main thread is busy
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
  } while (currentExitCode == Utils::EXIT_CODE_REBOOT);

  return currentExitCode;
}
