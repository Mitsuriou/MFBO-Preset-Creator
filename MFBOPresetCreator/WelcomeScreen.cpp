#include "WelcomeScreen.h"
#include "ComponentFactory.h"
#include "Update.h"
#include "Utils.h"
#include <QCheckBox>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QScrollArea>
#include <QScrollBar>
#include <QTextBrowser>

WelcomeScreen::WelcomeScreen(QWidget* aParent, const Struct::Settings& aSettings)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();

  // Search for updates
  this->checkForUpdate();
}

void WelcomeScreen::closeEvent(QCloseEvent* aEvent)
{
  // Before closing the welcome screen window, save the show/hide setting' state
  auto lShowHideWelcomeScreen{this->findChild<QCheckBox*>("always_show_welcome_screen")};

  // Update the setting's value
  auto lSettingsCopy{this->mSettings};
  lSettingsCopy.showWelcomeScreen = lShowHideWelcomeScreen->isChecked();
  Utils::saveSettingsToFile(lSettingsCopy);

  // Update the settings everywhere else in the app
  emit refreshMainUI(lSettingsCopy, true);

  aEvent->accept();
}

void WelcomeScreen::reject()
{
  this->close();
}

void WelcomeScreen::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setMinimumHeight(600);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Welcome screen"));
  this->setWindowIcon(QIcon(QPixmap(":/black/home")));
}

void WelcomeScreen::initializeGUI()
{
  // Keep a reference to the user theme
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Main layout
  auto lMainLayout{ComponentFactory::createScrollAreaWindowLayout(this, true, false)};

  /*================================*/
  /* Show / Hide the welcome screen */
  /*================================*/
  auto lShowHideWelcomeScreen{new QCheckBox(tr("Always show the welcome screen at application startup"), this)};
  lShowHideWelcomeScreen->setCursor(Qt::PointingHandCursor);
  lShowHideWelcomeScreen->setObjectName("always_show_welcome_screen");
  lShowHideWelcomeScreen->setChecked(this->mSettings.showWelcomeScreen);
  lMainLayout->addWidget(lShowHideWelcomeScreen);

  /*============*/
  /* Main title */
  /*============*/
  auto lMainTitle{new QLabel(tr("MFBO: Preset Creator v.%1").arg(Utils::getApplicationVersion()), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->mSettings.font.size * 2));
  lMainLayout->addWidget(lMainTitle);

  /*========*/
  /* STABLE */
  /*========*/
  // Stable version release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Latest stable release notes"), this->mSettings.font.size));

  // Latest stable release notes
  auto lBrowserStableReleaseNotes{new QTextBrowser(this)};
  lBrowserStableReleaseNotes->setObjectName("browser_stable");
  lBrowserStableReleaseNotes->setOpenExternalLinks(true);
  lMainLayout->addWidget(lBrowserStableReleaseNotes);

  // Stable status label
  auto lStableStatusLabel{new QLabel(tr("Contacting GitHub.com..."), this)};
  lStableStatusLabel->setObjectName("stable_status_label");
  lMainLayout->addWidget(lStableStatusLabel);

  // Download stable update button
  auto lDownloadStableUpdate{ComponentFactory::createButton(this, tr("Download the latest stable update"), "", "cloud-download", lIconFolder, "download_stable_update", false, true)};
  lMainLayout->addWidget(lDownloadStableUpdate);
  lDownloadStableUpdate->hide();

  /*======*/
  /* BETA */
  /*======*/
  // BETA version release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Latest BETA release notes"), this->mSettings.font.size));

  // Latest BETA release notes
  auto lBrowserBetaReleaseNotes{new QTextBrowser(this)};
  lBrowserBetaReleaseNotes->setObjectName("browser_beta");
  lBrowserBetaReleaseNotes->setOpenExternalLinks(true);
  lBrowserBetaReleaseNotes->hide();
  lMainLayout->addWidget(lBrowserBetaReleaseNotes);

  // BETA status label
  auto lBetaStatusLabel{new QLabel(tr("Contacting GitHub.com..."), this)};
  lBetaStatusLabel->setObjectName("beta_status_label");
  lMainLayout->addWidget(lBetaStatusLabel);

  // Download BETA update button
  auto lDownloadBetaUpdate{ComponentFactory::createButton(this, tr("Download the latest BETA update"), "", "cloud-download", lIconFolder, "download_beta_update", false, true)};
  lMainLayout->addWidget(lDownloadBetaUpdate);
  lDownloadBetaUpdate->hide();

  /*===================*/
  /* Incoming features */
  /*===================*/
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Incoming features"), this->mSettings.font.size));

  auto lIncomingFeaturesText{QString("<p style='padding: 0px; margin: 0px;'>"
                                     "<span>Batch Conversion tool.</span><br />"
                                     "<span>Portable installer archive.</span>"
                                     "</p>")};

  auto lIncomingFeatures{new QLabel(this)};
  lIncomingFeatures->setTextFormat(Qt::RichText);
  lIncomingFeatures->setText(lIncomingFeaturesText);
  lMainLayout->addWidget(lIncomingFeatures);

  /*==============*/
  /* Known issues */
  /*==============*/
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Known issues"), this->mSettings.font.size));

  auto lKnownIssuesText{QString("<p style='padding: 0px; margin: 0px;'>"
                                "<span>Collapsing a groupbox on a small resolution window makes the GUI blink.</span>"
                                "</p>")};

  auto lKnownIssues{new QLabel(this)};
  lKnownIssues->setTextFormat(Qt::RichText);
  lKnownIssues->setText(lKnownIssuesText);
  lMainLayout->addWidget(lKnownIssues);

  /*=====================*/
  /* Guide and tutorials */
  /*=====================*/
  lMainLayout->addWidget(this->createTitleLabel(this, tr("User guide and tutorials"), this->mSettings.font.size));

  auto lGuideLabel{new QLabel(tr("Whether it is your first time using the application or you're wondering how a particular feature works, you should check the user guide and detailed tutorials by cliking the link below:"), this)};
  lGuideLabel->setWordWrap(true);
  lMainLayout->addWidget(lGuideLabel);

  auto lOpenGuideTutorials{ComponentFactory::createButton(this, "User guide and tutorials (docs.google.com)", "", "external", lIconFolder, "", false, true)};
  lMainLayout->addWidget(lOpenGuideTutorials);

  /*===============*/
  /* Event binding */
  /*===============*/
  this->connect(lDownloadStableUpdate, &QPushButton::clicked, this, &WelcomeScreen::launchUpdateDialog);
  this->connect(lDownloadBetaUpdate, &QPushButton::clicked, this, &WelcomeScreen::launchUpdateDialog);
  this->connect(lOpenGuideTutorials, &QPushButton::clicked, this, &WelcomeScreen::openGoogleDriveGuide);
}

QLabel* WelcomeScreen::createTitleLabel(QWidget* aParent, const QString& aText, const int aAppFontSize)
{
  auto lLabel{new QLabel(aText, aParent)};

  // Hacky links' colors override for some themes
  auto lColorOverride{QString()};

  if (this->mSettings.appTheme == GUITheme::MITSURIOU_BLACK_THEME
      || this->mSettings.appTheme == GUITheme::MITSURIOU_DARK_THEME
      || this->mSettings.appTheme == GUITheme::MITSURIOU_LIGHT_THEME)
  {
    lColorOverride = this->mSettings.appTheme == GUITheme::MITSURIOU_BLACK_THEME ? QString("color:#3991ff;") : QString("color:#e95985;");
  }

  lLabel->setStyleSheet(QString("font-size: %1pt; %2").arg(static_cast<int>(std::floor(aAppFontSize * 1.75))).arg(lColorOverride));

  return lLabel;
}

void WelcomeScreen::overrideHTMLLinksColor(QString& aHTMLString)
{
  // If no color change is needed
  if (this->mSettings.appTheme != GUITheme::MITSURIOU_BLACK_THEME
      && this->mSettings.appTheme != GUITheme::MITSURIOU_DARK_THEME
      && this->mSettings.appTheme != GUITheme::MITSURIOU_LIGHT_THEME)
  {
    return;
  }

  // Hacky links' colors override for some themes
  auto lLinksColorOverride{this->mSettings.appTheme == GUITheme::MITSURIOU_BLACK_THEME ? QString("color:#3991ff") : QString("color:#e95985")};

  // Go through the string to find the link colors
  auto i{0};
  while ((i = aHTMLString.indexOf("color:#0000ff", i)) != -1)
  {
    aHTMLString.replace(i, lLinksColorOverride.size(), lLinksColorOverride);
  }
}

void WelcomeScreen::launchUpdateDialog()
{
  auto lEventSource{qobject_cast<QPushButton*>(sender())};
  if (lEventSource == this->findChild<QPushButton*>("download_stable_update"))
  {
    new Update(this->parentWidget(), this->mSettings, false);
    this->close();
  }
  else if (lEventSource == this->findChild<QPushButton*>("download_beta_update"))
  {
    new Update(this->parentWidget(), this->mSettings, true);
    this->close();
  }
}

void WelcomeScreen::openGoogleDriveGuide()
{
  QDesktopServices::openUrl(QUrl("https://docs.google.com/document/d/1WpDKMk_WoPRrj0Lkst6TptUGEFAC2xYGd3HUBYxPQ-A/edit?usp=sharing"));
}

void WelcomeScreen::checkForUpdate()
{
  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  connect(lReply, &QNetworkReply::finished, this, &WelcomeScreen::updateCheckFinished);
}

void WelcomeScreen::displayUpdateMessage(const QString& aResult)
{
  auto lBrowserStableReleaseNotes{this->findChild<QTextBrowser*>("browser_stable")};
  auto lStableStatusLabel{this->findChild<QLabel*>("stable_status_label")};
  auto lBrowserBetaReleaseNotes{this->findChild<QTextBrowser*>("browser_beta")};
  auto lBetaStatusLabel{this->findChild<QLabel*>("beta_status_label")};

  auto lStableVersions{QStringList()};
  auto lLatestStableReleaseNote{QString()};
  auto lBetaVersions{QStringList()};
  auto lLatestBetaReleaseNote{QString()};
  auto lUserRunningBetaVersion{false};
  const auto lCurrentVersion{Utils::getApplicationVersion()};

  if (aResult == "fetch_error")
  {
    lStableStatusLabel->setText(tr("An error has occurred... Make sure your internet connection is operational and try again."));
    lBetaStatusLabel->setText(tr("An error has occurred... Make sure your internet connection is operational and try again."));
    return;
  }

  // Declare and initialize local variables
  auto lTagName{QString()};

  // Create a JSON from the fetched string and parse the "tag_name" data
  QJsonDocument lJsonDocument{QJsonDocument::fromJson(aResult.toUtf8())};
  QJsonArray lTagsArray{lJsonDocument.array()};

  // Iterate in the versions array
  for (int i = 0; i < lTagsArray.size(); i++)
  {
    // Parse the tag_name
    lTagName = lTagsArray.at(i)["tag_name"].toString();
    Utils::cleanBreaksString(lTagName);

    // Check if it is a stable or a BETA version
    if (lTagsArray.at(i)["prerelease"].toBool())
    {
      // Save the latest beta's body tag value
      if (lLatestBetaReleaseNote.length() == 0)
      {
        lLatestBetaReleaseNote = lTagsArray.at(i)["body"].toString();
        Utils::cleanBreaksString(lLatestBetaReleaseNote);
      }

      // Add this version name to the beta versions list
      lBetaVersions.push_back(lTagName);
    }
    else
    {
      // Save the latest stable's body tag value
      if (lLatestStableReleaseNote.length() == 0)
      {
        lLatestStableReleaseNote = lTagsArray.at(i)["body"].toString();
        Utils::cleanBreaksString(lLatestStableReleaseNote);
      }

      // Add this version name to the stable versions list
      lStableVersions.push_back(lTagName);
    }
  }

  lUserRunningBetaVersion = lBetaVersions.size() > 0 && lBetaVersions.contains(lCurrentVersion);

  ///*========*/
  ///* STABLE */
  ///*========*/
  // Set the release note in the text browser
  lBrowserStableReleaseNotes->setMarkdown(lLatestStableReleaseNote);

  // Links color override
  auto lHTMLString{lBrowserStableReleaseNotes->toHtml()};
  this->overrideHTMLLinksColor(lHTMLString);
  lBrowserStableReleaseNotes->setHtml(lHTMLString);

  // Parsing error
  if (lStableVersions.size() == 0)
  {
    lStableStatusLabel->setText(tr("An error has occured while analyzing GitHub's API data. Please retry in a few seconds."));
  }
  // A new version is available
  else if (!lUserRunningBetaVersion && Utils::compareVersionNumbers(lStableVersions.at(0), lCurrentVersion) == ApplicationVersionRelative::NEWER)
  {
    lBrowserStableReleaseNotes->show();
    lStableStatusLabel->setText(tr("The new stable version \"%1\" is available on GitHub. Press the button below to open the updater window:").arg(lStableVersions.at(0)));
    this->findChild<QPushButton*>("download_stable_update")->show();
  }
  // Already running the latest version
  else if (Utils::compareVersionNumbers(lStableVersions.at(0), lCurrentVersion) == ApplicationVersionRelative::EQUIVALENT)
  {
    lBrowserStableReleaseNotes->show();
    lStableStatusLabel->setText(tr("You are already running the latest stable version \"%1\".").arg(lStableVersions.at(0)));
  }
  else if (lUserRunningBetaVersion)
  {
    lBrowserStableReleaseNotes->show();
    lStableStatusLabel->setText(tr("You are running a BETA version. Above are displayed the latest stable version release notes. Press the button below to open the updater window:"));
    this->findChild<QPushButton*>("download_stable_update")->show();
  }
  // Running a developper version (since the current version number is higher than the latest one available on GitHub)
  else if (Utils::compareVersionNumbers(lStableVersions.at(0), lCurrentVersion) == ApplicationVersionRelative::OLDER)
  {
    lBrowserStableReleaseNotes->show();
    lStableStatusLabel->setText(tr("You are running a developer version."));
  }

  /*======*/
  /* BETA */
  /*======*/
  // Set the release note in the text browser
  lBrowserBetaReleaseNotes->setMarkdown(lLatestBetaReleaseNote);

  // Links color override
  lHTMLString = lBrowserBetaReleaseNotes->toHtml();
  this->overrideHTMLLinksColor(lHTMLString);
  lBrowserBetaReleaseNotes->setHtml(lHTMLString);

  // Parsing error
  if (lStableVersions.size() == 0 && lBetaVersions.size() == 0)
  {
    lBetaStatusLabel->setText(tr("An error has occured while analyzing GitHub's API data. Please retry in a few seconds."));
  }
  // There is not any BETA version available on GitHub
  else if (lBetaVersions.size() == 0)
  {
    lBetaStatusLabel->setText(tr("No BETA version found on GitHub."));
  }
  // The BETA version is older than the current stable
  else if (Utils::compareVersionNumbers(lBetaVersions.at(0), lStableVersions.at(0)) == ApplicationVersionRelative::OLDER)
  {
    lBetaStatusLabel->setText(tr("No newer BETA version is currently available."));
  }
  // Already running the latest version
  else if (Utils::compareVersionNumbers(lBetaVersions.at(0), lCurrentVersion) == ApplicationVersionRelative::EQUIVALENT)
  {
    lBrowserBetaReleaseNotes->show();
    lBetaStatusLabel->setText(tr("You are already running the latest BETA version \"%1\".").arg(lBetaVersions.at(0)));
  }
  // Running a developper version (since the current version number is higher than the latest one available on GitHub)
  else if (Utils::compareVersionNumbers(lBetaVersions.at(0), lCurrentVersion) == ApplicationVersionRelative::NEWER)
  {
    lBrowserBetaReleaseNotes->show();
    lBetaStatusLabel->setText(tr("The new BETA version \"%1\" is available on GitHub. Press the button below to open the updater window:").arg(lBetaVersions.at(0)));
    this->findChild<QPushButton*>("download_beta_update")->show();
  }
}

void WelcomeScreen::updateCheckFinished()
{
  auto lReply{qobject_cast<QNetworkReply*>(sender())};

  if (lReply->error() == QNetworkReply::NoError)
  {
    this->displayUpdateMessage(QString::fromLocal8Bit(lReply->readAll()));
  }
  else
  {
    this->displayUpdateMessage("fetch_error");
  }

  lReply->deleteLater();
}

void WelcomeScreen::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
