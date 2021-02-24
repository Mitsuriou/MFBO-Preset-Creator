#include "MFBOPresetCreator.h"
#include "Utils.h"
#include "stdafx.h"

int main(int argc, char* argv[])
{
  auto currentExitCode{0};

  // Check the launch arguments
  if (argc >= 2)
  {
    // Show the debug console
    Utils::bindConsoleToStdOut();

    if (!(argc == 2 && strcmp(argv[1], "--debug") == 0))
    {
      // If the argument is not '--debug', kill the application
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
      Utils::printMessageStdOut("Only the \"--debug\" parameter is accepted");
      system("pause");
      return currentExitCode;
    }
  }

  do
  {
    Utils::printMessageStdOut("");
    Utils::printMessageStdOut("Running MFBOPC in debug mode");
    Utils::printMessageStdOut("Checking support for SSL...");
    Utils::printMessageStdOut(QString("Supports SSL? %1").arg(QSslSocket::supportsSsl() ? "yes" : "no"));
    Utils::printMessageStdOut(QString("SSL version information: %1").arg(QSslSocket::sslLibraryBuildVersionString()));
    Utils::printMessageStdOut("");

    // Reset the value
    Utils::RESTART_PENDING = false;

    const auto& lAppVersion{Utils::getApplicationVersion()};

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Utils::printMessageStdOut("Creating the application instance...");

    // Create the main GUI handler
    QApplication lMainApplication(argc, argv);
    lMainApplication.setApplicationDisplayName(QString("MFBOPC (v.%1)").arg(lAppVersion));
    lMainApplication.setApplicationName("MFBOPresetCreator");
    lMainApplication.setApplicationVersion(lAppVersion);
    lMainApplication.setWindowIcon(QIcon(QPixmap(":/application/icon")));

    // Set the codec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Show the splash screen
    const QPixmap lSplashScreenBackground(":/application/splashscreen");

    QSplashScreen lSplashScreen(lSplashScreenBackground.scaled(800, 450));
    Utils::printMessageStdOut("Starting the application...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Starting the application...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lSplashScreen.show();
    lMainApplication.processEvents();

    // Update the message
    Utils::printMessageStdOut("Cleaning temporary installer files...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Cleaning temporary installer files...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Check if an installer file needs to be removed at launch
    QFile lInstallerLogFile(Utils::getAppDataPathFolder() + "installer.log");
    if (lInstallerLogFile.exists())
    {
      if (lInstallerLogFile.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        QTextStream lStream(&lInstallerLogFile);
        auto lPathToDelete{lStream.readAll()};
        lInstallerLogFile.close();

        QFile lInstallerFile(lPathToDelete);
        lInstallerFile.remove();
        Utils::printMessageStdOut(QString("Cleaned %1").arg(lPathToDelete));

        lInstallerLogFile.remove();
        Utils::printMessageStdOut(QString("Cleaned %1installer.log").arg(Utils::getAppDataPathFolder()));
      }
    }

    // Update the message
    Utils::printMessageStdOut("Reading and applying custom fonts...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Reading and applying custom fonts...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Embed custom fonts
    QFontDatabase::addApplicationFont(":/fonts/Roboto");

    // Update the message
    Utils::printMessageStdOut("Loading user settings...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Loading user settings...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Read settings file
    auto lSettings{Utils::loadSettingsFromFile()};

    // Update the message
    Utils::printMessageStdOut("Applying translation files...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Applying translation files...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

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

    // Update the message
    Utils::printMessageStdOut("Creating the main window...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Creating the main window...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Create and show the main window
    MFBOPresetCreator lMainWindow(lSettings);

    // Make the splash screen disappear when the main window is displayed
    lSplashScreen.finish(&lMainWindow);

    // Launch the application
    currentExitCode = lMainApplication.exec();
  } while (currentExitCode == Utils::EXIT_CODE_REBOOT);

  FreeConsole();

  return currentExitCode;
}
