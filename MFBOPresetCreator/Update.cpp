#include "Update.h"

Update::Update(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void Update::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Check for updates"));
  this->setWindowIcon(QIcon(QPixmap(":/black/cloud-search")));
}

void Update::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QVBoxLayout(this)};
  lMainVertical->setAlignment(Qt::AlignCenter);
  this->setupInterface(*lMainVertical);
}

void Update::setupInterface(QVBoxLayout& aLayout)
{
  // Button
  auto lUpdateButton{new QPushButton(this)};
  lUpdateButton->setCursor(Qt::PointingHandCursor);

  QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/cloud-search" : ":/black/cloud-search"};
  lUpdateButton->setObjectName("search_button");
  lUpdateButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
  lUpdateButton->setIconSize(QSize(48, 48));
  lUpdateButton->setContentsMargins(0, 0, 0, 0);
  aLayout.addWidget(lUpdateButton);

  // Fetch status
  auto lCheckForUpdates{new QLabel(tr("Click on the button above to search for new updates."), this)};
  lCheckForUpdates->setAlignment(Qt::AlignCenter);
  lCheckForUpdates->setObjectName("fetch_status");
  aLayout.addWidget(lCheckForUpdates);

  // Event binding
  this->connect(lUpdateButton, &QPushButton::clicked, this, &Update::getLastAvailableVersion);
}

void Update::getLastAvailableVersion()
{
  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/latest"};
  HTTPRequesterGet* lHTTPDownloader{new HTTPRequesterGet(lGitHubURL, this)};
  this->connect(lHTTPDownloader, &HTTPRequesterGet::resultReady, this, &Update::pageFetched);
  this->connect(lHTTPDownloader, &HTTPRequesterGet::finished, lHTTPDownloader, &QObject::deleteLater);
  lHTTPDownloader->start();
}

void Update::pageFetched(const QString& aResult)
{
  auto lFetchStatus{this->findChild<QLabel*>("fetch_status")};
  auto lSearchButton{this->findChild<QPushButton*>("search_button")};

  if (aResult == "fetch_error")
  {
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/alert-circle" : ":/black/alert-circle"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));

    lFetchStatus->setText(tr("An error has occurred while searching for a new version... Make sure your internet connection is operational and try again."));
  }
  else
  {
    // Create a JSON from the fetched string and parse the "tag_name" data
    QJsonDocument doc{QJsonDocument::fromJson(aResult.toUtf8())};
    QJsonObject obj{doc.object()};
    auto lTagName{obj["tag_name"].toString()};
    Utils::cleanBreaksString(lTagName);
    auto lCurrentVersion{Utils::getApplicationVersion()};

#ifdef DEBUG

    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/developer" : ":/black/developer"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setDisabled(true);

    lFetchStatus->setText(tr("You are currently running the developer version \"%1\".\nThe last available version on GitHub is tagged \"%2\".").arg(lCurrentVersion).arg(lTagName));

#else

    if (lCurrentVersion != lTagName)
    {
      // A new version available
      QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/open-in-browser" : ":/black/open-in-browser"};
      lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
      lSearchButton->setIconSize(QSize(48, 48));

      this->disconnect(lSearchButton, &QPushButton::clicked, this, &Update::getLastAvailableVersion);
      this->connect(lSearchButton, &QPushButton::clicked, this, &Update::openGithubInBrowser);

      lFetchStatus->setText(tr("You are currently running the version \"%1\".\nThe new version \"%2\" is available on GitHub.\n\nClick on the download button to be redirected\non Github.com to download the latest MFBOPC version.").arg(lCurrentVersion).arg(lTagName));
    }
    else
    {
      // The user runs the latest version
      QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/cloud-check" : ":/black/cloud-check"};
      lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
      lSearchButton->setIconSize(QSize(48, 48));
      lSearchButton->setDisabled(true);

      lFetchStatus->setText(tr("You are running the latest version \"%1\".").arg(lCurrentVersion));
    }

#endif
  }
}

void Update::openGithubInBrowser()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/latest"));
}
