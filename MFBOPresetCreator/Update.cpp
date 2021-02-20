#include "Update.h"

Update::Update(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
  , mDownloadURL(QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/latest/download/mfbopc-install-wizard.exe"))
  , mHasDownloadBeenCanceled(false)
  , mDownloadedFile(nullptr)
  , mReply(nullptr)
{
  // Build the window's interface
  this->setWindowProperties();
  this->setupInterface();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void Update::reject()
{
  // Cancel the download before closing the window
  if (this->mDownloadedFile && this->mReply)
  {
    this->cancelCurrentDownload();
  }

  this->accept();
}

void Update::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setMinimumHeight(500);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Check for updates"));
  this->setWindowIcon(QIcon(QPixmap(":/black/cloud-search")));
}

void Update::setupInterface()
{
  // Set a layout for this dialog box
  this->setLayout(new QVBoxLayout(this));
  this->layout()->setAlignment(Qt::AlignTop);

  // Current version
  auto lCurrentVersion{new QLabel(tr("Current version: v.%1").arg(Utils::getApplicationVersion()), this)};
  this->layout()->addWidget(lCurrentVersion);

  // Check for updates
  auto lUpdateButton{new QPushButton(this)};
  lUpdateButton->setCursor(Qt::PointingHandCursor);
  lUpdateButton->setObjectName("search_button");
  QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/cloud-search" : ":/black/cloud-search"};
  lUpdateButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
  lUpdateButton->setIconSize(QSize(48, 48));
  lUpdateButton->setContentsMargins(0, 0, 0, 0);
  lUpdateButton->setText(tr("Check for updates"));
  this->layout()->addWidget(lUpdateButton);

  // Download progress bar
  auto lDownloadProgressBar{new QProgressBar(this)};
  lDownloadProgressBar->setObjectName("download_progress_bar");
  lDownloadProgressBar->setTextVisible(true);
  lDownloadProgressBar->setFormat(tr("Initializing..."));
  this->layout()->addWidget(lDownloadProgressBar);
  lDownloadProgressBar->hide();

  // Fetch status
  auto lFetchStatus{new QLabel(this)};
  lFetchStatus->setObjectName("fetch_status");
  lFetchStatus->hide();
  this->layout()->addWidget(lFetchStatus);

  // Event binding
  this->connect(lUpdateButton, &QPushButton::clicked, this, &Update::checkForUpdate);
}

void Update::overrideHTMLLinksColor(QString& aHTMLString)
{
  // If no color change is needed
  if (this->mSettings.appTheme != GUITheme::MitsuriouLightTheme && this->mSettings.appTheme != GUITheme::MitsuriouDarkTheme)
  {
    return;
  }

  // Hacky links' colors override for some themes
  auto lLinksColorOverride{QString("color:#e95985")};

  // Go through the string to find the link colors
  auto i{0};
  while ((i = aHTMLString.indexOf("color:#0000ff", i)) != -1)
  {
    aHTMLString.replace(i, lLinksColorOverride.size(), lLinksColorOverride);
  }
}

void Update::displayUpdateMessage(const QString& aResult)
{
  auto lFetchStatus{this->findChild<QLabel*>("fetch_status")};
  auto lSearchButton{this->findChild<QPushButton*>("search_button")};

  if (aResult == "fetch_error")
  {
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/alert-circle" : ":/black/alert-circle"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setText(tr("Check for updates once again"));

    lFetchStatus->setText(tr("An error has occurred while searching for a new version.\nPlease make sure your internet connection is working correctly and try again."));
    return;
  }

  // Create a JSON object from the fetched string
  const auto lJSONObject{QJsonDocument::fromJson(aResult.toUtf8()).object()};

  // Get the "tag_name" value
  auto lTagName{lJSONObject["tag_name"].toString()};
  Utils::cleanBreaksString(lTagName);
  this->mNewVersionTag = lTagName;
  this->mNewVersionTag.replace(".", "-");
  auto lCurrentVersion{Utils::getApplicationVersion()};

#ifdef DEBUG
  QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/developer" : ":/black/developer"};
  lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
  lSearchButton->setIconSize(QSize(48, 48));
  lSearchButton->setDisabled(true);
  lSearchButton->setText(tr("You are running a developer version"));

  lFetchStatus->setText(tr("You are currently running the developer version \"v.%1\".\nThe last available version on GitHub is tagged \"v.%2\".\n\nBelow are the release notes for the latest stable version:").arg(lCurrentVersion).arg(lTagName));
#else
  if (lCurrentVersion != lTagName)
  {
    // A new version available
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/cloud-download" : ":/black/cloud-download"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setText(tr("Download the update"));

    this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::checkForUpdate);
    this->connect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
    this->mSaveFilePath = QString("%1/mfbopc-wizard-%2.exe").arg(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).arg(this->mNewVersionTag);
    Utils::cleanPathString(this->mSaveFilePath);
    lFetchStatus->setText(tr("You are currently running the version \"v.%1\".\nThe new version \"v.%2\" is available on GitHub.\n\nClick on the download button above to start downloading the update.\nThe download size is about 10MB~.\nThe download will be saved under \"%3\".\n\nBelow are the release notes for this update:").arg(lCurrentVersion).arg(lTagName).arg(this->mSaveFilePath));
  }
  else
  {
    // The user runs the latest version
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/cloud-check" : ":/black/cloud-check"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setDisabled(true);
    lSearchButton->setText(tr("You are already running the latest version"));

    lFetchStatus->setText(tr("Awesome! You are already running the latest version (\"v.%1\").\nBelow are the release notes for this version:").arg(lCurrentVersion));
  }
#endif

  // Display the latest release's full release notes as markdown format
  auto lDescription{lJSONObject["body"].toString()};
  Utils::cleanBreaksString(lDescription);
  auto lTextContainer{new QTextBrowser(this)};
  lTextContainer->setOpenExternalLinks(true);
  lTextContainer->setMarkdown(lDescription);
  lTextContainer->setMinimumHeight(200);
  lTextContainer->adjustSize();
  lTextContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->layout()->addWidget(lTextContainer);

  // Links color override
  auto lHTMLString{lTextContainer->toHtml()};
  this->overrideHTMLLinksColor(lHTMLString);
  lTextContainer->setHtml(lHTMLString);
}

void Update::displayFileDownloadEndStatus(const bool& aResult)
{
  auto lDownloadProgressBar{this->findChild<QProgressBar*>("download_progress_bar")};
  lDownloadProgressBar->hide();

  auto lFetchStatus{this->findChild<QLabel*>("fetch_status")};
  auto lSearchButton{this->findChild<QPushButton*>("search_button")};
  this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::cancelCurrentDownload);

  auto lSuccessText{tr("Download successful. Click the button above to start updating MFBOPC.\nMake sure that you saved everything before starting the update as the application will be closed!\n\n")};
  auto lErrorText{tr("An error has occurred while downloading the update.\nPlease make sure your internet connection is working correctly and try again.\n\n")};

  if (aResult)
  {
    // The app has been downloaded
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/arrow-up" : ":/black/arrow-up"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setText(tr("Close MFBOPC and install the update"));

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

    this->connect(lSearchButton, &QPushButton::clicked, this, &Update::installLatestUpdate);
  }
  else
  {
    // Error while downloading
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/alert-circle" : ":/black/alert-circle"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setText(tr("Try to download the update once again"));

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

    this->connect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
  }
}

void Update::checkForUpdate()
{
  auto lFetchStatus{this->findChild<QLabel*>("fetch_status")};
  lFetchStatus->setText(tr("Contacting GitHub.com..."));
  lFetchStatus->show();

  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/latest"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  connect(lReply, &QNetworkReply::finished, this, &Update::updateCheckSuccess);
  connect(lReply, &QNetworkReply::errorOccurred, this, &Update::updateCheckError);
}

void Update::updateCheckSuccess()
{
  auto lReply{qobject_cast<QNetworkReply*>(sender())};
  this->displayUpdateMessage(QString::fromLocal8Bit(lReply->readAll()));
  lReply->deleteLater();
}

void Update::updateCheckError(QNetworkReply::NetworkError)
{
  auto lReply{qobject_cast<QNetworkReply*>(sender())};
  this->displayUpdateMessage("fetch_error");
  lReply->deleteLater();
}

void Update::downloadLatestUpdate()
{
  auto lSearchButton{this->findChild<QPushButton*>("search_button")};
  lSearchButton->setText(tr("Cancel the download"));

  // Update the icon in case the user had an error before
  QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/cloud-download" : ":/black/cloud-download"};
  lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
  lSearchButton->setIconSize(QSize(48, 48));

  // Rebind the events to the correct handlers
  this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
  this->connect(lSearchButton, &QPushButton::clicked, this, &Update::cancelCurrentDownload);

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
  this->connect(this->mReply, &QNetworkReply::readyRead, this, &Update::fileChunkReceived);
  this->connect(mReply, &QNetworkReply::downloadProgress, this, &Update::chunkSizeUpdated);
  this->connect(mReply, &QNetworkReply::finished, this, &Update::fileDownloadEnded);
}

void Update::cancelCurrentDownload()
{
  // Update the GUI
  auto lSearchButton{this->findChild<QPushButton*>("search_button")};
  lSearchButton->setText(tr("Download canceled: click to try to download the update once again"));

  // Rebind the correct events
  this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::cancelCurrentDownload);
  this->connect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);

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

  auto lDownloadProgressBar{this->findChild<QProgressBar*>("download_progress_bar")};

  // Show the bar only if a relevant download has started
  if (aTotal > 200000) // 0.20MB
  {
    lDownloadProgressBar->show();
  }

  // Change the value displayed on the progress bar
  lDownloadProgressBar->setValue(aBytesRead);
  lDownloadProgressBar->setMaximum(aTotal);
  lDownloadProgressBar->setFormat(tr("%1 bytes out of %2 bytes (%p%)").arg(QString::number(aBytesRead)).arg(QString::number(aTotal)));
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

    auto lDownloadProgressBar{this->findChild<QProgressBar*>("download_progress_bar")};
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

void Update::installLatestUpdate()
{
  auto lSearchButton{this->findChild<QPushButton*>("search_button")};
  lSearchButton->setDisabled(true);

  this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::installLatestUpdate);

  // Create a file to be able to know that the installer file needs to be removed at next launch
  QFile lLogFile(Utils::getAppDataPathFolder() + "installer.log");
  lLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

  QTextStream lTextStream(&lLogFile);
  lTextStream << this->mSaveFilePath;
  lTextStream.flush();

  lLogFile.close();

  // Start the update process
  QProcess::startDetached(this->mSaveFilePath, QStringList());
  qApp->exit();
}
