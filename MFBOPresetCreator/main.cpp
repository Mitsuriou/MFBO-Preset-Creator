#include "MFBOPresetCreator.h"
#include "Utils.h"
#include "stdafx.h"

FILE* lOutFile{NULL};
FILE* lErrFile{NULL};

int main(int argc, char* argv[])
{
  auto currentExitCode{0};

  if (argc == 2)
  {
    if (strcmp(argv[1], "--debug") == 0)
    {
      AllocConsole();
      freopen_s(&lOutFile, "CONOUT$", "w", stdout);
      freopen_s(&lErrFile, "CONOUT$", "w", stderr);

      SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
    }
    else
    {
      std::cerr << "Only \"--debug\" parameter is accepted." << std::endl;
      return currentExitCode;
    }
  }
  else if (argc > 2)
  {
    std::cerr << "Cannot launch the program with more than 1 parameter." << std::endl;
    return currentExitCode;
  }

  do
  {
    std::cout << "-- Running MFBOPC in debug mode --" << std::endl;
    std::cout << "Checking support for SSL..." << std::endl;
    std::cout << "Supports SSL? " << (QSslSocket::supportsSsl() ? "yes" : "no") << std::endl;
    std::cout << "SSL version information: " << QSslSocket::sslLibraryBuildVersionString().toStdString() << std::endl
              << std::endl;

    // Reset the value
    Utils::RESTART_PENDING = false;

    const auto& lAppVersion{Utils::getApplicationVersion()};

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    std::cout << "Creating the application instance..." << std::endl;

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
    std::cout << "Starting the application..." << std::endl;
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Starting the application...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lSplashScreen.show();
    lMainApplication.processEvents();

    // Update the message
    std::cout << "Cleaning temporary installer files..." << std::endl;
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
        std::cout << "Cleaned " << lPathToDelete.toStdString() << std::endl;

        lInstallerLogFile.remove();
        std::cout << "Cleaned " << Utils::getAppDataPathFolder().toStdString() << "installer.log" << std::endl;
      }
    }

    // Update the message
    std::cout << "Reading and applying custom fonts..." << std::endl;
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Reading and applying custom fonts...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Embed custom fonts
    QFontDatabase::addApplicationFont(":/fonts/Roboto");

    // Update the message
    std::cout << "Loading user settings..." << std::endl;
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Loading user settings...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Read settings file
    auto lSettings{Utils::loadSettingsFromFile()};

    // Update the message
    std::cout << "Applying translation files..." << std::endl;
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
    std::cout << "Creating the main window..." << std::endl;
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Creating the main window...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Create and show the main window
    MFBOPresetCreator lMainWindow(lSettings);

    // Make the splash screen disappear when the main window is displayed
    lSplashScreen.finish(&lMainWindow);

    // Launch the application
    currentExitCode = lMainApplication.exec();
  } while (currentExitCode == Utils::EXIT_CODE_REBOOT);

  return currentExitCode;
}
