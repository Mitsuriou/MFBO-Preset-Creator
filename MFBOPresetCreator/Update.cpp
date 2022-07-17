#include "Update.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QApplication>
#include <QDesktopServices>
#include <QFileInfo>
#include <QLabel>
#include <QNetworkRequest>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QStandardPaths>
#include <QTextBrowser>

Update::Update(QWidget* aParent,
               const Struct::Settings& aSettings,
               std::map<QString, QString>* aLastPaths,
               const bool aForceStableContext,
               const bool aForceBetaContext)
  : TitleDialog(aParent, tr("Check for updates"), "cloud-search", aSettings, aLastPaths, 700, 500)
  , mForceStableContext(aForceStableContext)
  , mForceBetaContext(aForceBetaContext)
{
  // Build the window's interface
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();

  // Search for updates instantly
  auto lSearchButton{this->findChild<QPushButton*>(QString("search_button"))};
  lSearchButton->click();
}

void Update::closeEvent(QCloseEvent*)
{
  // Cancel the download before closing the window
  if (this->mDownloadedFile && this->mReply)
  {
    this->cancelCurrentDownload();
  }

  // Clear the downloaded file since the user did not ask to install the update (but ignore the standalone case)
  if (QFile::exists(this->mSaveFilePath) && !Utils::IsRunningStandaloneVersion())
  {
    QFile::remove(this->mSaveFilePath);
  }

  this->accept();
}

void Update::initializeGUI()
{
  // Set a layout for this dialog box
  const auto lMainLayout{new QVBoxLayout{this}};
  lMainLayout->setAlignment(Qt::AlignTop);
  this->getCentralWidget()->setLayout(lMainLayout);

  const auto lForcedVersionSuffix(this->mForceStableContext ? tr(" (stable only)") : (this->mForceBetaContext ? tr(" (BETA only)") : QString()));

  // Check for updates
  auto lUpdateButton{ComponentFactory::CreateButton(this, tr("Check for updates") + lForcedVersionSuffix, "", "cloud-search", this->getThemedResourcePath(), "search_button")};
  lUpdateButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
  lUpdateButton->setContentsMargins(0, 0, 0, 0);
  lMainLayout->addWidget(lUpdateButton);

  // Download progress bar
  auto lDownloadProgressBar{new QProgressBar(this)};
  lDownloadProgressBar->setObjectName(QString("download_progress_bar"));
  lDownloadProgressBar->setTextVisible(true);
  lDownloadProgressBar->setFormat(tr("Initializing..."));
  lMainLayout->addWidget(lDownloadProgressBar);
  lDownloadProgressBar->hide();

  // Fetch status
  auto lFetchStatus{new QLabel(this)};
  lFetchStatus->setObjectName(QString("fetch_status"));
  lFetchStatus->setWordWrap(true);
  lFetchStatus->hide();
  lMainLayout->addWidget(lFetchStatus);

  // Event binding
  QObject::connect(lUpdateButton, &QPushButton::clicked, this, &Update::checkForUpdate);
}

void Update::checkForUpdate()
{
  auto lFetchStatus{this->findChild<QLabel*>(QString("fetch_status"))};
  lFetchStatus->setText(tr("Contacting GitHub.com..."));
  lFetchStatus->show();

  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  QObject::connect(lReply, &QNetworkReply::finished, this, &Update::updateCheckFinished);
}

void Update::updateCheckFinished()
{
  auto lReply{qobject_cast<QNetworkReply*>(this->sender())};

  if (lReply->error() == QNetworkReply::NoError)
  {
    this->displayUpdateMessage(true, QString::fromUtf8(lReply->readAll()));
  }
  else
  {
    this->displayUpdateMessage(false, "");
  }

  lReply->deleteLater();
}

void Update::displayUpdateMessage(const bool aSucceeded, const QString& aResult)
{
  auto lFetchStatus{this->findChild<QLabel*>(QString("fetch_status"))};
  auto lSearchButton{this->findChild<QPushButton*>(QString("search_button"))};

  if (!aSucceeded)
  {
    lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/alert-circle").arg(this->getThemedResourcePath()))));
    lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
    lSearchButton->setText(tr("Check for updates once again"));
    lSearchButton->setToolTip(tr("Check for updates once again"));

    lFetchStatus->setText(tr("An error has occurred while searching for a new version.\nPlease make sure your internet connection is working correctly and try again."));
    return;
  }

  const auto lCurrentVersion{Utils::GetApplicationVersion()};
  auto lUseStableVersionNotes{false};
  const auto lVersionsInformation{Utils::ParseGitHubReleasesRequestResult(aResult)};

  if (lVersionsInformation.sizeStableVersionsList() > 0 && lVersionsInformation.sizeBetaVersionsList() > 0)
  {
    // A new BETA version is available and no newer stable available
    if (!this->mForceStableContext && Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::NEWER
        && Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lVersionsInformation.getLatestStableVersionNumber()) == ApplicationVersionRelative::NEWER)
    {
      lUseStableVersionNotes = false;

      if (Utils::IsRunningStandaloneVersion())
      {
#ifdef Q_OS_WIN
        this->mDownloadURL = QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/%1/MFBOPC.v%1.standalone.windows.zip").arg(lVersionsInformation.getLatestBetaVersionNumber());
#else
        this->mDownloadURL = QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/%1/MFBOPC.v%1.standalone.linux.zip").arg(lVersionsInformation.getLatestBetaVersionNumber());
#endif
      }
      else
      {
        this->mDownloadURL = QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/%1/mfbopc-install-wizard.exe").arg(lVersionsInformation.getLatestBetaVersionNumber());
      }

      // A new beta version is available
      lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/cloud-download").arg(this->getThemedResourcePath()))));
      lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
      lSearchButton->setText(tr("Download the update"));
      lSearchButton->setToolTip(tr("Download the update"));

      QObject::disconnect(lSearchButton, &QPushButton::clicked, this, &Update::checkForUpdate);
      QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
      auto lVersionFileName = lVersionsInformation.getLatestBetaVersionNumber();
      if (Utils::IsRunningStandaloneVersion())
      {
        this->mSaveFilePath = QString("%1/MFBOPC v.%2 standalone.zip").arg(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation), lVersionFileName);
      }
      else
      {
        lVersionFileName.replace(".", "-");
        this->mSaveFilePath = QString("%1/mfbopc-wizard-beta-%2.exe").arg(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation), lVersionFileName);
      }
      Utils::CleanPathString(this->mSaveFilePath);
      lFetchStatus->setText(tr("You are currently running the version \"%1\".\nThe new BETA version \"%2\" is available on GitHub.\n\nClick on the download button above to start downloading the update.\nThe download size is about 11MB~.\nThe download will be saved under \"%3\".\n\nBelow are the release notes for the BETA version \"%2\":")
                              .arg(lCurrentVersion, lVersionsInformation.getLatestBetaVersionNumber(), this->mSaveFilePath));
    }
    // A new stable version is available
    else if (!this->mForceBetaContext && Utils::CompareVersionNumbers(lVersionsInformation.getLatestStableVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::NEWER)
    {
      lUseStableVersionNotes = true;

      if (Utils::IsRunningStandaloneVersion())
      {
#ifdef Q_OS_WIN
        this->mDownloadURL = QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/%1/MFBOPC.v%1.standalone.windows.zip").arg(lVersionsInformation.getLatestStableVersionNumber());
#else
        this->mDownloadURL = QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/%1/MFBOPC.v%1.standalone.linux.zip").arg(lVersionsInformation.getLatestStableVersionNumber());
#endif
      }
      else
      {
        this->mDownloadURL = QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/%1/mfbopc-install-wizard.exe").arg(lVersionsInformation.getLatestStableVersionNumber());
      }

      // A new stable version is available
      lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/cloud-download").arg(this->getThemedResourcePath()))));
      lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
      lSearchButton->setText(tr("Download the update"));
      lSearchButton->setToolTip(tr("Download the update"));

      QObject::disconnect(lSearchButton, &QPushButton::clicked, this, &Update::checkForUpdate);
      QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
      auto lVersionFileName = lVersionsInformation.getLatestStableVersionNumber();
      if (Utils::IsRunningStandaloneVersion())
      {
        this->mSaveFilePath = QString("%1/MFBOPC v.%2 standalone.zip").arg(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation), lVersionFileName);
      }
      else
      {
        lVersionFileName.replace(".", "-");
        this->mSaveFilePath = QString("%1/mfbopc-wizard-stable-%2.exe").arg(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation), lVersionFileName);
      }
      Utils::CleanPathString(this->mSaveFilePath);
      lFetchStatus->setText(tr("You are currently running the version \"%1\".\nThe new stable version \"%2\" is available on GitHub.\n\nClick on the download button above to start downloading the update.\nThe download size is about 11MB~.\nThe download will be saved under \"%3\".\n\nBelow are the release notes for the stable version \"%2\":")
                              .arg(lCurrentVersion, lVersionsInformation.getLatestStableVersionNumber(), this->mSaveFilePath));
    }
    else
    {
      // The user runs the latest version
      lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/cloud-check").arg(this->getThemedResourcePath()))));
      lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
      lSearchButton->setDisabled(true);
      lSearchButton->setText(tr("Already running the latest version"));
      lSearchButton->setToolTip(tr("Already running the latest version"));
      lFetchStatus->setText(tr("Awesome! You are already running the latest available version."));

      return;
    }
  }

  // Display the latest release's full release notes as markdown format
  const auto& lDescription = lUseStableVersionNotes ? lVersionsInformation.getLatestStableReleaseNotes() : lVersionsInformation.getLatestBetaReleaseNotes();
  auto lTextContainer{new QTextBrowser(this)};
  lTextContainer->setOpenExternalLinks(true);
  lTextContainer->setMarkdown(lDescription);
  lTextContainer->setMinimumHeight(200);
  lTextContainer->adjustSize();
  lTextContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  qobject_cast<QVBoxLayout*>(this->getCentralLayout())->addWidget(lTextContainer);

  // Links color override
  auto lHTMLString{lTextContainer->toHtml()};
  Utils::OverrideHTMLLinksColor(lHTMLString, this->settings().display.applicationTheme);
  lTextContainer->setHtml(lHTMLString);
}

void Update::downloadLatestUpdate()
{
  auto lSearchButton{this->findChild<QPushButton*>(QString("search_button"))};
  lSearchButton->setText(tr("Cancel the download"));
  lSearchButton->setToolTip(tr("Cancel the download"));

  // Update the icon in case the user had an error before
  lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/cloud-download").arg(this->getThemedResourcePath()))));
  lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale

  // Rebind the events to the correct handlers
  QObject::disconnect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
  QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::cancelCurrentDownload);

  if (QFile::exists(this->mSaveFilePath))
  {
    QFile::remove(this->mSaveFilePath);
  }

  this->mDownloadedFile = new QFile(this->mSaveFilePath);
  if (!this->mDownloadedFile->open(QIODevice::WriteOnly))
  {
    this->displayFileDownloadEndStatus(false);
    delete this->mDownloadedFile;
    this->mDownloadedFile = nullptr;
    return;
  }

  this->mHasDownloadBeenCanceled = false;

  this->mReply = this->mManager.get(QNetworkRequest(QUrl(this->mDownloadURL)));

  QObject::connect(this->mReply, &QNetworkReply::readyRead, this, &Update::fileChunkReceived);
  QObject::connect(this->mReply, &QNetworkReply::downloadProgress, this, &Update::chunkSizeUpdated);
  QObject::connect(this->mReply, &QNetworkReply::finished, this, &Update::fileDownloadEnded);
}

void Update::cancelCurrentDownload()
{
  // Update the GUI
  auto lSearchButton{this->findChild<QPushButton*>(QString("search_button"))};
  lSearchButton->setText(tr("Download canceled: click to try to download the update once again"));
  lSearchButton->setToolTip(tr("Download canceled: click to try to download the update once again"));

  // Rebind the correct events
  QObject::disconnect(lSearchButton, &QPushButton::clicked, this, &Update::cancelCurrentDownload);
  QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);

  // Cancel the currently running download process
  this->mHasDownloadBeenCanceled = true;
  this->mReply->abort();
}

void Update::fileChunkReceived()
{
  // Append the new bytes chunk to the file currently being downloaded
  if (this->mDownloadedFile)
    this->mDownloadedFile->write(this->mReply->readAll());
}

void Update::chunkSizeUpdated(qint64 aBytesRead, qint64 aTotal)
{
  // Skip the GUI update if the user canceled the download to avoid any possible confusion
  if (this->mHasDownloadBeenCanceled)
  {
    return;
  }

  auto lDownloadProgressBar{this->findChild<QProgressBar*>(QString("download_progress_bar"))};

  // Show and update the bar only if a relevant download has started
  if (aTotal > 200000) // 0.20MB
  {
    lDownloadProgressBar->show();

    // Change the value displayed on the progress bar
    lDownloadProgressBar->setValue(aBytesRead);
    lDownloadProgressBar->setMaximum(aTotal);
    lDownloadProgressBar->setFormat(tr("%1 bytes out of %2 bytes (%p%)").arg(QString::number(aBytesRead), QString::number(aTotal)));
  }
}

void Update::fileDownloadEnded()
{
  // Handle download cancelation
  if (this->mHasDownloadBeenCanceled)
  {
    if (this->mDownloadedFile)
    {
      this->mDownloadedFile->close();
      this->mDownloadedFile->remove();

      delete this->mDownloadedFile;
      this->mDownloadedFile = nullptr;
    }

    auto lDownloadProgressBar{this->findChild<QProgressBar*>(QString("download_progress_bar"))};
    lDownloadProgressBar->hide();

    this->mReply->deleteLater();
    return;
  }

  // Write the last bytes in the file and close the latter
  this->mDownloadedFile->flush();
  this->mDownloadedFile->close();

  // Handle errors and URL redirection
  QVariant lRedirectionUrl{this->mReply->attribute(QNetworkRequest::RedirectionTargetAttribute)};
  if (this->mReply->error())
  {
    this->mDownloadedFile->remove();
    this->displayFileDownloadEndStatus(false);
  }
  else if (!lRedirectionUrl.isNull())
  {
    // Get the new URL
    this->mDownloadURL = this->mDownloadURL.resolved(lRedirectionUrl.toUrl());

    // Delete the network reply
    this->mReply->deleteLater();

    // Clear the file on the disk
    this->mDownloadedFile->open(QIODevice::WriteOnly);
    this->mDownloadedFile->resize(0);
    this->mDownloadedFile->close();

    // Relaunch the download with the brand new URL
    downloadLatestUpdate();

    return;
  }
  else
  {
    this->displayFileDownloadEndStatus(true);
  }

  // Memory management
  this->mReply->deleteLater();
  this->mReply = nullptr;

  delete this->mDownloadedFile;
  this->mDownloadedFile = nullptr;
}

void Update::displayFileDownloadEndStatus(const bool aResult)
{
  auto lDownloadProgressBar{this->findChild<QProgressBar*>(QString("download_progress_bar"))};
  lDownloadProgressBar->hide();

  auto lFetchStatus{this->findChild<QLabel*>(QString("fetch_status"))};
  auto lSearchButton{this->findChild<QPushButton*>(QString("search_button"))};
  QObject::disconnect(lSearchButton, &QPushButton::clicked, this, &Update::cancelCurrentDownload);

  auto lStandaloneSuccessText{tr("Download successful. Click the button above to open the directory where the file has been downloaded.\n\n")};
  auto lSuccessText{tr("Download successful. Click the button above to start updating MFBOPC.\nMake sure that you saved everything before starting the update as the application will be closed!\n\n")};
  auto lErrorText{tr("An error has occurred while downloading the update.\nPlease make sure your internet connection is working correctly and try again.\n\n")};

  if (aResult)
  {
    if (Utils::IsRunningStandaloneVersion())
    {
      // The app has been downloaded
      lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/folder").arg(this->getThemedResourcePath()))));
      lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
      lSearchButton->setText(tr("Show the ZIP file in Windows Explorer"));
      lSearchButton->setToolTip(tr("Show the ZIP file in Windows Explorer"));

      // Display an success message in the status label
      auto lStatusText{lFetchStatus->text()};
      if (lStatusText.startsWith(lErrorText))
      {
        lStatusText.replace(lErrorText, lStandaloneSuccessText);
        lFetchStatus->setText(lStatusText);
      }
      else if (!lStatusText.startsWith(lStandaloneSuccessText))
      {
        lStatusText.prepend(lStandaloneSuccessText);
        lFetchStatus->setText(lStatusText);
      }

      QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::openStandaloneOutputDirectory);
    }
    else
    {
      // The app has been downloaded
      lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/arrow-up").arg(this->getThemedResourcePath()))));
      lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
      lSearchButton->setText(tr("Close MFBOPC and install the update"));
      lSearchButton->setToolTip(tr("Close MFBOPC and install the update"));

      // Display an success message in the status label
      auto lStatusText{lFetchStatus->text()};
      if (lStatusText.startsWith(lErrorText))
      {
        lStatusText.replace(lErrorText, lSuccessText);
        lFetchStatus->setText(lStatusText);
      }
      else if (!lStatusText.startsWith(lSuccessText))
      {
        lStatusText.prepend(lSuccessText);
        lFetchStatus->setText(lStatusText);
      }

      QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::installLatestUpdate);
    }
  }
  else
  {
    // Error while downloading
    lSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/alert-circle").arg(this->getThemedResourcePath()))));
    lSearchButton->setIconSize(QSize(17 * 2, 17 * 2)); // TODO: Multiply the size by the DPI scale
    lSearchButton->setText(tr("Try to download the update once again"));
    lSearchButton->setToolTip(tr("Try to download the update once again"));

    // Display an error message in the status label
    auto lStatusText{lFetchStatus->text()};
    if (lStatusText.startsWith(lSuccessText))
    {
      lStatusText.replace(lSuccessText, lErrorText);
      lFetchStatus->setText(lStatusText);
    }
    else if (!lStatusText.startsWith(lErrorText))
    {
      lStatusText.prepend(lErrorText);
      lFetchStatus->setText(lStatusText);
    }

    QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
  }
}

void Update::installLatestUpdate()
{
  auto lSearchButton{this->findChild<QPushButton*>(QString("search_button"))};
  lSearchButton->setDisabled(true);

  QObject::disconnect(lSearchButton, &QPushButton::clicked, this, &Update::installLatestUpdate);

  // Create a file to be able to know that the installer file needs to be removed at next launch
  QFile lLogFile(Utils::GetInstallerLogFilePath());
  if (lLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    QTextStream lTextStream(&lLogFile);
    lTextStream << this->mSaveFilePath;
    lTextStream.flush();

    lLogFile.close();
  }

  // Start the update process
  if (QProcess::startDetached(this->mSaveFilePath, QStringList()))
  {
    qApp->exit();
  }
  else
  {
    Utils::DisplayWarningMessage(tr("The update process could not be started. Please verify that your firewall and your antivirus do not block the update file, before trying again."));
    QObject::connect(lSearchButton, &QPushButton::clicked, this, &Update::installLatestUpdate);
    lSearchButton->setDisabled(false);
  }
}

void Update::openStandaloneOutputDirectory()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)));
}
