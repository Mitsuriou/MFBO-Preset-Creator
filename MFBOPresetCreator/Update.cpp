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
  this->setMinimumWidth(450);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Check for updates"));
  this->setWindowIcon(QIcon(QPixmap(":/black/cloud-search")));
}

void Update::setupInterface()
{
  // Set a layout for this dialog box
  this->setLayout(new QVBoxLayout(this));

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
  this->connect(lUpdateButton, &QPushButton::clicked, this, &Update::getLastAvailableVersion);
}

void Update::overrideHTMLLinksColor(QString& aHTMLString)
{
  // Hacky links' colors override for some themes
  auto lLinksColorOverride{QString("")};
  if (this->mSettings.appTheme == GUITheme::MitsuriouLightTheme || this->mSettings.appTheme == GUITheme::MitsuriouDarkTheme)
  {
    lLinksColorOverride = " style='color: #e95985;'";
  }

  std::vector<int> lPositions;
  int j = 0;
  while ((j = aHTMLString.indexOf("<a ", j)) != -1)
  {
    lPositions.push_back(j);
    qDebug() << "Found starting \"<a \" tag at index position" << j;
    //aHTMLString.insert(j, lLinksColorOverride);
    j++;
  }

  // WIP / TODO
  for (auto i = lPositions.rbegin(); i != lPositions.rend(); ++i)
  {
    // Not working:
    aHTMLString.insert(*i, lLinksColorOverride);
    //aHTMLString.append("TEST");
  }
}

void Update::getLastAvailableVersion()
{
  auto lFetchStatus{this->findChild<QLabel*>("fetch_status")};
  lFetchStatus->setText(tr("Contacting GitHub.com..."));
  lFetchStatus->show();

  qApp->processEvents();

#ifdef DEBUG
  // DEBUG MODE TO NOT EXHAUST GITHUB API REQUESTS QUOTA
  pageFetched("{\"url\":\"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/36828575\",\"assets_url\":\"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/36828575/assets\",\"upload_url\":\"https://uploads.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/36828575/assets{?name,label}\",\"html_url\":\"https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/tag/2.4.0\",\"id\":36828575,\"author\":{\"login\":\"Mitsuriou\",\"id\":37184309,\"node_id\":\"MDQ6VXNlcjM3MTg0MzA5\",\"avatar_url\":\"https://avatars.githubusercontent.com/u/37184309?v=4\",\"gravatar_id\":\"\",\"url\":\"https://api.github.com/users/Mitsuriou\",\"html_url\":\"https://github.com/Mitsuriou\",\"followers_url\":\"https://api.github.com/users/Mitsuriou/followers\",\"following_url\":\"https://api.github.com/users/Mitsuriou/following{/other_user}\",\"gists_url\":\"https://api.github.com/users/Mitsuriou/gists{/gist_id}\",\"starred_url\":\"https://api.github.com/users/Mitsuriou/starred{/owner}{/repo}\",\"subscriptions_url\":\"https://api.github.com/users/Mitsuriou/subscriptions\",\"organizations_url\":\"https://api.github.com/users/Mitsuriou/orgs\",\"repos_url\":\"https://api.github.com/users/Mitsuriou/repos\",\"events_url\":\"https://api.github.com/users/Mitsuriou/events{/privacy}\",\"received_events_url\":\"https://api.github.com/users/Mitsuriou/received_events\",\"type\":\"User\",\"site_admin\":false},\"node_id\":\"MDc6UmVsZWFzZTM2ODI4NTc1\",\"tag_name\":\"2.4.0\",\"target_commitish\":\"main\",\"name\":\"MFBOPC v2.4.0\",\"draft\":false,\"prerelease\":false,\"created_at\":\"2021-01-24T18:31:58Z\",\"published_at\":\"2021-01-24T18:40:14Z\",\"assets\":[{\"url\":\"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/assets/31124926\",\"id\":31124926,\"node_id\":\"MDEyOlJlbGVhc2VBc3NldDMxMTI0OTI2\",\"name\":\"Manual.install.7z\",\"label\":null,\"uploader\":{\"login\":\"Mitsuriou\",\"id\":37184309,\"node_id\":\"MDQ6VXNlcjM3MTg0MzA5\",\"avatar_url\":\"https://avatars.githubusercontent.com/u/37184309?v=4\",\"gravatar_id\":\"\",\"url\":\"https://api.github.com/users/Mitsuriou\",\"html_url\":\"https://github.com/Mitsuriou\",\"followers_url\":\"https://api.github.com/users/Mitsuriou/followers\",\"following_url\":\"https://api.github.com/users/Mitsuriou/following{/other_user}\",\"gists_url\":\"https://api.github.com/users/Mitsuriou/gists{/gist_id}\",\"starred_url\":\"https://api.github.com/users/Mitsuriou/starred{/owner}{/repo}\",\"subscriptions_url\":\"https://api.github.com/users/Mitsuriou/subscriptions\",\"organizations_url\":\"https://api.github.com/users/Mitsuriou/orgs\",\"repos_url\":\"https://api.github.com/users/Mitsuriou/repos\",\"events_url\":\"https://api.github.com/users/Mitsuriou/events{/privacy}\",\"received_events_url\":\"https://api.github.com/users/Mitsuriou/received_events\",\"type\":\"User\",\"site_admin\":false},\"content_type\":\"application/octet-stream\",\"state\":\"uploaded\",\"size\":8171484,\"download_count\":5,\"created_at\":\"2021-01-24T20:26:01Z\",\"updated_at\":\"2021-01-24T20:26:08Z\",\"browser_download_url\":\"https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/2.4.0/Manual.install.7z\"},{\"url\":\"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/assets/31124929\",\"id\":31124929,\"node_id\":\"MDEyOlJlbGVhc2VBc3NldDMxMTI0OTI5\",\"name\":\"Manual.install.zip\",\"label\":null,\"uploader\":{\"login\":\"Mitsuriou\",\"id\":37184309,\"node_id\":\"MDQ6VXNlcjM3MTg0MzA5\",\"avatar_url\":\"https://avatars.githubusercontent.com/u/37184309?v=4\",\"gravatar_id\":\"\",\"url\":\"https://api.github.com/users/Mitsuriou\",\"html_url\":\"https://github.com/Mitsuriou\",\"followers_url\":\"https://api.github.com/users/Mitsuriou/followers\",\"following_url\":\"https://api.github.com/users/Mitsuriou/following{/other_user}\",\"gists_url\":\"https://api.github.com/users/Mitsuriou/gists{/gist_id}\",\"starred_url\":\"https://api.github.com/users/Mitsuriou/starred{/owner}{/repo}\",\"subscriptions_url\":\"https://api.github.com/users/Mitsuriou/subscriptions\",\"organizations_url\":\"https://api.github.com/users/Mitsuriou/orgs\",\"repos_url\":\"https://api.github.com/users/Mitsuriou/repos\",\"events_url\":\"https://api.github.com/users/Mitsuriou/events{/privacy}\",\"received_events_url\":\"https://api.github.com/users/Mitsuriou/received_events\",\"type\":\"User\",\"site_admin\":false},\"content_type\":\"application/x-zip-compressed\",\"state\":\"uploaded\",\"size\":11464911,\"download_count\":1,\"created_at\":\"2021-01-24T20:26:06Z\",\"updated_at\":\"2021-01-24T20:26:15Z\",\"browser_download_url\":\"https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/2.4.0/Manual.install.zip\"},{\"url\":\"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/assets/31122274\",\"id\":31122274,\"node_id\":\"MDEyOlJlbGVhc2VBc3NldDMxMTIyMjc0\",\"name\":\"mfbopc-install-wizard.exe\",\"label\":null,\"uploader\":{\"login\":\"Mitsuriou\",\"id\":37184309,\"node_id\":\"MDQ6VXNlcjM3MTg0MzA5\",\"avatar_url\":\"https://avatars.githubusercontent.com/u/37184309?v=4\",\"gravatar_id\":\"\",\"url\":\"https://api.github.com/users/Mitsuriou\",\"html_url\":\"https://github.com/Mitsuriou\",\"followers_url\":\"https://api.github.com/users/Mitsuriou/followers\",\"following_url\":\"https://api.github.com/users/Mitsuriou/following{/other_user}\",\"gists_url\":\"https://api.github.com/users/Mitsuriou/gists{/gist_id}\",\"starred_url\":\"https://api.github.com/users/Mitsuriou/starred{/owner}{/repo}\",\"subscriptions_url\":\"https://api.github.com/users/Mitsuriou/subscriptions\",\"organizations_url\":\"https://api.github.com/users/Mitsuriou/orgs\",\"repos_url\":\"https://api.github.com/users/Mitsuriou/repos\",\"events_url\":\"https://api.github.com/users/Mitsuriou/events{/privacy}\",\"received_events_url\":\"https://api.github.com/users/Mitsuriou/received_events\",\"type\":\"User\",\"site_admin\":false},\"content_type\":\"application/x-msdownload\",\"state\":\"uploaded\",\"size\":10179154,\"download_count\":33,\"created_at\":\"2021-01-24T18:34:22Z\",\"updated_at\":\"2021-01-24T18:34:37Z\",\"browser_download_url\":\"https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/download/2.4.0/mfbopc-install-wizard.exe\"}],\"tarball_url\":\"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/tarball/2.4.0\",\"zipball_url\":\"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/zipball/2.4.0\",\"body\":\"## VERSION 2.4.0\r\n### GENERAL IMPROVEMENTS\r\n- Added a checkbox to use only the subdirectory field as the export path.\r\n- Added color choosers in the Settings window. It allows the user to modify the success, warning and danger colors.\r\n- Moved the JSON configurations files to AppData/Local/MFBOPresetCreator/*.\r\n\r\n### BUGS FIXED\r\n- Quick restarting the application makes the user lose all its last opened paths.\r\n- The user cannot save their settings, filters list nor last opened path when using the Installer version.\r\n- The user loses their custom filters list if reinstalling the app through the Installer version.\r\n\r\n## PREVIOUS VERSIONS NOTES\r\n#### [Patch note for the version 2.3.0 is available here](https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/tag/2.3.0).\"}");
  //pageFetched("fetch_error");
#else
  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/latest"};
  HTTPRequesterGet* lHTTPDownloader{new HTTPRequesterGet(lGitHubURL, this)};
  this->connect(lHTTPDownloader, &HTTPRequesterGet::resultReady, this, &Update::pageFetched);
  this->connect(lHTTPDownloader, &HTTPRequesterGet::finished, lHTTPDownloader, &QObject::deleteLater);
  lHTTPDownloader->start();
#endif
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
    return;
  }

  // Create a JSON from the fetched string and parse the "tag_name" data
  QJsonDocument doc{QJsonDocument::fromJson(aResult.toUtf8())};
  QJsonObject obj{doc.object()};
  auto lTagName{obj["tag_name"].toString()};
  Utils::cleanBreaksString(lTagName);
  auto lCurrentVersion{Utils::getApplicationVersion()};

#ifndef DEBUG
  QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/developer" : ":/black/developer"};
  lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
  lSearchButton->setIconSize(QSize(48, 48));
  lSearchButton->setDisabled(true);
  lSearchButton->setText(tr("You are running a developper version."));

  lFetchStatus->setText(tr("You are currently running the developer version v.\"%1\".\nThe last available version on GitHub is tagged v.\"%2\".\n\nBelow are the release notes for the latest stable version:").arg(lCurrentVersion).arg(lTagName));
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

    lFetchStatus->setText(tr("You are currently running the version v.\"%1\".\nThe new version v.\"%2\" is available on GitHub.\n\nClick on the download button above to start downloading the update.\nThe download size is about 10MB~.").arg(lCurrentVersion).arg(lTagName));
  }
  else
  {
    // The user runs the latest version
    QString lPath{Utils::isThemeDark(mSettings.appTheme) ? ":/white/cloud-check" : ":/black/cloud-check"};
    lSearchButton->setIcon(QIcon(QPixmap(lPath).scaledToHeight(48, Qt::SmoothTransformation)));
    lSearchButton->setIconSize(QSize(48, 48));
    lSearchButton->setDisabled(true);

    lFetchStatus->setText(tr("You are already running the latest version (v.\"%1\").").arg(lCurrentVersion));
  }
#endif

  // Display the latest release's full release notes as markdown format
  auto lDescription{obj["body"].toString()};
  Utils::cleanBreaksString(lDescription);
  auto lTextContainer{new QTextBrowser(this)};
  lTextContainer->setOpenExternalLinks(true);
  lTextContainer->setMarkdown(lDescription);
  lTextContainer->setMinimumHeight(200);
  lTextContainer->adjustSize();
  lTextContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->layout()->addWidget(lTextContainer);

  auto lA = lTextContainer->toHtml();
  this->overrideHTMLLinksColor(lA);
}

void Update::downloadLatestUpdate()
{
  auto lGitHubURL{QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/latest/download/mfbopc-install-wizard.exe")};
  HTTPRequesterFile* lHTTPDownloader{new HTTPRequesterFile(lGitHubURL, this)};
  this->connect(lHTTPDownloader, &HTTPRequesterFile::resultReady, this, &Update::fileFetched);
  this->connect(lHTTPDownloader, &HTTPRequesterFile::finished, lHTTPDownloader, &QObject::deleteLater);
  lHTTPDownloader->start();
}

void Update::fileFetched(const bool& aResult)
{
  // TODO
}
