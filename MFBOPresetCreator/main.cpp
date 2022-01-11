#include "MFBOPresetCreator.h"
#include "Utils.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QLibraryInfo>
#include <QSplashScreen>
#include <QStandardPaths>
#include <QTextCodec>
#include <QTranslator>
#include <iostream>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#if defined(DEBUG) || !defined(QT_NO_DEBUG)
bool FORCE_CONSOLE_DISPLAY = true;
#else
bool FORCE_CONSOLE_DISPLAY = false;
#endif

bool isArgValidFilePath(const QString& aPathToTest)
{
  QFile lFileToTest{aPathToTest};
  QFileInfo lFileInfo{lFileToTest};
  return (lFileToTest.exists() && lFileInfo.completeSuffix().compare("pcp") == 0);
}

int main(int argc, char* argv[])
{
  auto currentExitCode{0};
  auto lInjectedFilePath{QString()};

  // Check the launch arguments
  if (argc >= 2 || FORCE_CONSOLE_DISPLAY)
  {
    // Too many arguments, kill the application
    if (argc > 3)
    {
      return -1;
    }

    if (FORCE_CONSOLE_DISPLAY)
    {
      // Show the debug console
      Utils::BindConsoleToStdOut();
    }

    if (argc == 2)
    {
      if (strcmp(argv[1], "--debug") == 0)
      {
        // Show the debug console
        Utils::BindConsoleToStdOut();
      }
      else
      {
        // Test if it is a file
        auto lArgPath{QString(argv[1])};
        if (isArgValidFilePath(lArgPath))
        {
          lInjectedFilePath = lArgPath;
        }
      }
    }
    else if (argc == 3)
    {
      if (strcmp(argv[1], "--debug") == 0 || strcmp(argv[2], "--debug") == 0)
      {
        // Show the debug console
        Utils::BindConsoleToStdOut();
      }

      // Test if the first argument is a file
      auto lArgPath{QString(argv[1])};
      if (isArgValidFilePath(lArgPath))
      {
        lInjectedFilePath = lArgPath;
      }
      else
      {
        // Test if the second argument is a file
        lArgPath = QString(argv[2]);
        if (isArgValidFilePath(lArgPath))
        {
          lInjectedFilePath = lArgPath;
        }
      }
    }
  }

  do
  {
    Utils::PrintMessageStdOut("");
    Utils::PrintMessageStdOut("Running MFBOPC in debug mode");
    Utils::PrintMessageStdOut("Checking support for SSL...");
    Utils::PrintMessageStdOut(QString("Supports SSL? %1").arg(QSslSocket::supportsSsl() ? "yes" : "no"));
    Utils::PrintMessageStdOut(QString("SSL version information: %1").arg(QSslSocket::sslLibraryBuildVersionString()));
    Utils::PrintMessageStdOut("");

    // Reset the value
    Utils::RESTART_PENDING = false;

    qApp->setAttribute(Qt::AA_EnableHighDpiScaling, true);
    qApp->setAttribute(Qt::AA_DisableWindowContextHelpButton, true);
    qApp->setApplicationVersion("3.6.3.0");
    const auto& lAppVersion{Utils::GetApplicationVersion()};

    Utils::PrintMessageStdOut("Creating the application instance...");

    // Create the main GUI handler
    QApplication lMainApplication(argc, argv);

    auto lAppNamePreffix{QString()};
    if (Utils::IsRunningStandaloneVersion())
    {
      lAppNamePreffix = "[standalone] ";
    }

    lMainApplication.setApplicationDisplayName(lAppNamePreffix + QString("MFBOPC (v.%1)").arg(lAppVersion));
    lMainApplication.setApplicationName("MFBOPresetCreator");
    lMainApplication.setApplicationVersion(lAppVersion);
    lMainApplication.setWindowIcon(QIcon(QPixmap(":/application/icon")));

    // Set the codec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Show the splash screen
    const QPixmap lSplashScreenBackground(":/application/splashscreen");

    QSplashScreen lSplashScreen(lSplashScreenBackground.scaled(800, 450));
    Utils::PrintMessageStdOut("Starting the application...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Starting the application...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lSplashScreen.show();
    lMainApplication.processEvents();

    // Update the message
    Utils::PrintMessageStdOut("Cleaning temporary installer files...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Cleaning temporary installer files...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Check if an installer file needs to be removed at launch
    QFile lInstallerLogFile(Utils::GetAppDataPathFolder() + "installer.log");
    if (lInstallerLogFile.exists())
    {
      if (lInstallerLogFile.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        auto lPathToDelete{QString::fromLocal8Bit(lInstallerLogFile.readAll())};
        lInstallerLogFile.close();

        QFile lInstallerFile(lPathToDelete);
        lInstallerFile.remove();
        Utils::PrintMessageStdOut(QString("Cleaned %1").arg(lPathToDelete));

        lInstallerLogFile.remove();
        Utils::PrintMessageStdOut(QString("Cleaned %1installer.log").arg(Utils::GetAppDataPathFolder()));
      }
    }

    // Update the message
    Utils::PrintMessageStdOut("Reading and applying custom fonts...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Reading and applying custom fonts...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Embed custom fonts
    QFontDatabase::addApplicationFont(":/fonts/Roboto");

    // Update the message
    Utils::PrintMessageStdOut("Loading user settings...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Loading user settings...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Read settings file
    auto lSettings{Utils::LoadSettingsFromFile()};

    // Update the message
    Utils::PrintMessageStdOut("Applying translation files...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Applying translation files...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Apply custom language and translation
    auto lLanguageToSet{Utils::GetShortLanguageNameFromEnum(static_cast<int>(lSettings.display.language))};

    auto lTranslator{new QTranslator()};
    if (lTranslator->load(QString(":/translations/mfbopc_%1.qm").arg(lLanguageToSet)))
    {
      lMainApplication.installTranslator(lTranslator);
    }

    // Apply default Qt language and translation
    auto lQtBaseTranslator{new QTranslator()};
    if (lQtBaseTranslator->load(QString("qt_%1.qm").arg(lLanguageToSet), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
      lMainApplication.installTranslator(lQtBaseTranslator);
    }

    // Update the message
    Utils::PrintMessageStdOut("Creating the main window...");
    lSplashScreen.showMessage(QString("MFBOPC (v.%1): Creating the main window...").arg(lAppVersion), Qt::AlignBottom | Qt::AlignRight, Qt::white);
    lMainApplication.processEvents();

    // Create and show the main window
    MFBOPresetCreator lMainWindow(lSettings, lInjectedFilePath);

    // Avoid re-injecting the file a second time if quick reloading the application
    lInjectedFilePath.clear();

    // Make the splash screen disappear when the main window is displayed
    lSplashScreen.finish(&lMainWindow);

    // Launch the application
    currentExitCode = lMainApplication.exec();
  } while (currentExitCode == Utils::EXIT_CODE_REBOOT);

#ifdef Q_OS_WIN
  FreeConsole();
#endif

  return currentExitCode;
}
