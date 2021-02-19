#include "Update.h"

Update::Update(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->setupInterface();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void Update::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setMinimumHeight(450);
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

    this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::getLastAvailableVersion);
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
  lSearchButton->setDisabled(true);
  lSearchButton->setText(tr("Download in progress..."));

  // Create and execute the HTTP request
  auto lGitHubURL{QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/latest/download/mfbopc-install-wizard.exe")};
  HTTPRequesterFile* lHTTPDownloader{new HTTPRequesterFile(lGitHubURL, this->mSaveFilePath, this)};
  this->connect(lHTTPDownloader, &HTTPRequesterFile::resultReady, this, &Update::fileFetched);
  this->connect(lHTTPDownloader, &HTTPRequesterFile::finished, lHTTPDownloader, &QObject::deleteLater);
  lHTTPDownloader->start();
}

void Update::fileFetched(const bool& aResult)
{
  auto lFetchStatus{this->findChild<QLabel*>("fetch_status")};
  auto lSearchButton{this->findChild<QPushButton*>("search_button")};

  auto lSuccessText{tr("Download successful. Click the button above to start updating MFBOPC.\nMake sure that you saved everything before starting the update as the application will be closed!\n\n")};
  auto lErrorText{tr("An error has occurred while downloading the update.\nPlease make sure your internet connection is working correctly and try again.\n\n")};

  if (aResult)
  {
    // The app has been downloaded
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/arrow-up" : ":/black/arrow-up"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setDisabled(false);
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

    this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::downloadLatestUpdate);
    this->connect(lSearchButton, &QPushButton::clicked, this, &Update::installLatestUpdate);
  }
  else
  {
    // Error while downloading
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/alert-circle" : ":/black/alert-circle"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setDisabled(false);
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
  }
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
