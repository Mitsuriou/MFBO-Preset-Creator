#include "WelcomeScreen.h"
#include "ComponentFactory.h"
#include "Update.h"
#include "Utils.h"
#include <QCheckBox>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QIcon>
#include <QScrollArea>
#include <QTextBrowser>
#include <math.h>

WelcomeScreen::WelcomeScreen(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, tr("Welcome screen"), "home", aSettings, aLastPaths, 700, 600)
{
  this->overrideContentTitle(tr("MFBO: Preset Creator v.%1").arg(Utils::GetApplicationVersion()));

  // Build the window's interface
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
  auto lShowHideWelcomeScreen{this->findChild<QCheckBox*>(QString("always_show_welcome_screen"))};

  // Update the setting's value
  auto lSettingsCopy{this->settings()};
  if (this->settings().general.startupAction == StartupAction::OPEN_WELCOME_SCREEN && !lShowHideWelcomeScreen->isChecked())
  {
    lSettingsCopy.general.startupAction = StartupAction::CHECK_FOR_UPDATES;
  }
  else if (lShowHideWelcomeScreen->isChecked())
  {
    lSettingsCopy.general.startupAction = StartupAction::OPEN_WELCOME_SCREEN;
  }
  else
  {
    lSettingsCopy.general.startupAction = this->settings().general.startupAction;
  }

  Utils::SaveSettingsToFile(lSettingsCopy);

  // Update the settings everywhere else in the app
  emit refreshMainUI(lSettingsCopy, true);

  aEvent->accept();
}

void WelcomeScreen::initializeGUI()
{
  // Main layout
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this->getCentralWidget())};
  const auto lButtonsLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  /*================================*/
  /* Show / Hide the welcome screen */
  /*================================*/
  auto lShowHideWelcomeScreen{ComponentFactory::CreateCheckBox(this,
                                                               tr("Show the welcome screen at application startup"),
                                                               "",
                                                               "always_show_welcome_screen",
                                                               this->settings().general.startupAction == StartupAction::OPEN_WELCOME_SCREEN)};
  lButtonsLayout->addWidget(lShowHideWelcomeScreen);

  /*============*/
  /* Ko-Fi page */
  /*============*/
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Support me: donate - buy me a coffee"), this->settings().display.font.pointSize));

  auto lFreewareLabel{new QLabel(tr("Anything I create is for free and is created during my free time. Any given cent is meaningful to me. If you want to support me financially, click the button below:"), this)};
  lFreewareLabel->setWordWrap(true);
  lMainLayout->addWidget(lFreewareLabel);

  // Donate button
  auto lDonateButton{ComponentFactory::CreateButton(this,
                                                    tr("Donate - buy me a coffee (ko-fi.com)"),
                                                    "",
                                                    "coffee",
                                                    this->getThemedResourcePath(),
                                                    "",
                                                    false,
                                                    true)};
  lMainLayout->addWidget(lDonateButton);

  /*=================*/
  /* CURRENT VERSION */
  /*=================*/
  // Current version's release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Current version's release notes"), this->settings().display.font.pointSize));

  // Current version's status label
  auto lCurrentVersionStatusLabel{new QLabel(tr("Contacting GitHub.com..."), this)};
  lCurrentVersionStatusLabel->setObjectName(QString("current_version_status_label"));
  lMainLayout->addWidget(lCurrentVersionStatusLabel);

  // Current version's release notes
  auto lBrowserCurrentVersionReleaseNotes{new QTextBrowser(this)};
  lBrowserCurrentVersionReleaseNotes->setObjectName(QString("browser_current_version"));
  lBrowserCurrentVersionReleaseNotes->setOpenExternalLinks(true);
  lBrowserCurrentVersionReleaseNotes->hide();
  lMainLayout->addWidget(lBrowserCurrentVersionReleaseNotes);

  /*========*/
  /* STABLE */
  /*========*/
  // Stable version release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Latest stable release notes"), this->settings().display.font.pointSize));

  // Latest stable release notes
  auto lBrowserStableReleaseNotes{new QTextBrowser(this)};
  lBrowserStableReleaseNotes->setObjectName(QString("browser_stable"));
  lBrowserStableReleaseNotes->setOpenExternalLinks(true);
  lBrowserStableReleaseNotes->hide();
  lMainLayout->addWidget(lBrowserStableReleaseNotes);

  // Stable status label
  auto lStableStatusLabel{new QLabel(tr("Contacting GitHub.com..."), this)};
  lStableStatusLabel->setObjectName(QString("stable_status_label"));
  lMainLayout->addWidget(lStableStatusLabel);

  // Download stable update button
  auto lDownloadStableUpdate{ComponentFactory::CreateButton(this,
                                                            tr("Download the latest stable update"),
                                                            "",
                                                            "cloud-download",
                                                            this->getThemedResourcePath(),
                                                            "download_stable_update",
                                                            false,
                                                            true)};
  lMainLayout->addWidget(lDownloadStableUpdate);
  lDownloadStableUpdate->hide();

  /*======*/
  /* BETA */
  /*======*/
  // BETA version release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Latest BETA release notes"), this->settings().display.font.pointSize));

  // Latest BETA release notes
  auto lBrowserBetaReleaseNotes{new QTextBrowser(this)};
  lBrowserBetaReleaseNotes->setObjectName(QString("browser_beta"));
  lBrowserBetaReleaseNotes->setOpenExternalLinks(true);
  lBrowserBetaReleaseNotes->hide();
  lMainLayout->addWidget(lBrowserBetaReleaseNotes);

  // BETA status label
  auto lBetaStatusLabel{new QLabel(tr("Contacting GitHub.com..."), this)};
  lBetaStatusLabel->setObjectName(QString("beta_status_label"));
  lMainLayout->addWidget(lBetaStatusLabel);

  // Download BETA update button
  auto lDownloadBetaUpdate{ComponentFactory::CreateButton(this,
                                                          tr("Download the latest BETA update"),
                                                          "",
                                                          "cloud-download",
                                                          this->getThemedResourcePath(),
                                                          "download_beta_update",
                                                          false,
                                                          true)};
  lMainLayout->addWidget(lDownloadBetaUpdate);
  lDownloadBetaUpdate->hide();

  /*===================*/
  /* Incoming features */
  /*===================*/
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Incoming features"), this->settings().display.font.pointSize));

  auto lIncomingFeaturesLabel{new QLabel(tr("You can consult the list of incoming features and enhancements or ask for new features requests by clicking the button below:"), this)};
  lIncomingFeaturesLabel->setWordWrap(true);
  lMainLayout->addWidget(lIncomingFeaturesLabel);

  auto lOpenIncomingFeatures{ComponentFactory::CreateButton(this, tr("Incoming features list (github.com)"), "", "external", this->getThemedResourcePath(), "", false, true)};
  lMainLayout->addWidget(lOpenIncomingFeatures);

  /*==============*/
  /* Known issues */
  /*==============*/
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Known issues"), this->settings().display.font.pointSize));

  auto lKnownIssuesLabel{new QLabel(tr("You can consult the list of already reported bugs that are waiting for a fix or report a new issue by clicking the button below:"), this)};
  lKnownIssuesLabel->setWordWrap(true);
  lMainLayout->addWidget(lKnownIssuesLabel);

  auto lOpenKnownIssues{ComponentFactory::CreateButton(this, tr("Known issues list (github.com)"), "", "external", this->getThemedResourcePath(), "", false, true)};
  lMainLayout->addWidget(lOpenKnownIssues);

  /*=====================*/
  /* Guide and tutorials */
  /*=====================*/
  lMainLayout->addWidget(this->createTitleLabel(this, tr("User guide and tutorials"), this->settings().display.font.pointSize));

  auto lGuideLabel{new QLabel(tr("Whether it is your first time using the application or you're wondering how a particular feature works, you should check the user guide and detailed tutorials by clicking the button below:"), this)};
  lGuideLabel->setWordWrap(true);
  lMainLayout->addWidget(lGuideLabel);

  auto lOpenGuideTutorials{ComponentFactory::CreateButton(this, tr("User guide and tutorials (docs.google.com)"), "", "external", this->getThemedResourcePath(), "", false, true)};
  lMainLayout->addWidget(lOpenGuideTutorials);

  /*===============*/
  /* Event binding */
  /*===============*/
  QObject::connect(lDonateButton, &QPushButton::clicked, this, &WelcomeScreen::openKoFiPage);
  QObject::connect(lDownloadStableUpdate, &QPushButton::clicked, this, &WelcomeScreen::launchUpdateDialog);
  QObject::connect(lDownloadBetaUpdate, &QPushButton::clicked, this, &WelcomeScreen::launchUpdateDialog);
  QObject::connect(lOpenIncomingFeatures, &QPushButton::clicked, this, &WelcomeScreen::openIncomingFeatures);
  QObject::connect(lOpenKnownIssues, &QPushButton::clicked, this, &WelcomeScreen::openKnownIssues);
  QObject::connect(lOpenGuideTutorials, &QPushButton::clicked, this, &WelcomeScreen::openGoogleDriveGuide);
}

QLabel* WelcomeScreen::createTitleLabel(QWidget* aParent, const QString& aText, const int aAppFontSize)
{
  auto lLabel{new QLabel(aText, aParent)};

  // Hacky links' colors override for some themes
  QString lColorOverride;

  if (this->settings().display.applicationTheme == GUITheme::MITSURIOU_BLACK_THEME
      || this->settings().display.applicationTheme == GUITheme::MITSURIOU_DARK_THEME
      || this->settings().display.applicationTheme == GUITheme::MITSURIOU_LIGHT_THEME)
  {
    lColorOverride = this->settings().display.applicationTheme == GUITheme::MITSURIOU_BLACK_THEME ? QString("color:#3991ff;") : QString("color:#e95985;");
  }

  lLabel->setStyleSheet(QString("qproperty-indent:0; margin: 0; padding: 0; margin-top: 15px; font-size: %1pt; %2").arg(static_cast<int>(floor(aAppFontSize * 1.75))).arg(lColorOverride));

  return lLabel;
}

void WelcomeScreen::openKoFiPage()
{
  QDesktopServices::openUrl(QUrl("https://ko-fi.com/mitsuriou"));
}

void WelcomeScreen::launchUpdateDialog()
{
  const auto lEventSource{qobject_cast<QPushButton*>(this->sender())};
  if (lEventSource == this->findChild<QPushButton*>(QString("download_stable_update")))
  {
    new Update(this->parentWidget(), this->settings(), this->lastPaths(), true, false);
    this->close();
  }
  else if (lEventSource == this->findChild<QPushButton*>(QString("download_beta_update")))
  {
    new Update(this->parentWidget(), this->settings(), this->lastPaths(), false, true);
    this->close();
  }
}

void WelcomeScreen::openIncomingFeatures()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Mitsuriou/MFBO-Preset-Creator/issues?q=is%3Aissue+is%3Aopen+-label%3Abug"));
}

void WelcomeScreen::openKnownIssues()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Mitsuriou/MFBO-Preset-Creator/issues?q=is%3Aissue+is%3Aopen+label%3Abug"));
}

void WelcomeScreen::openGoogleDriveGuide()
{
  QDesktopServices::openUrl(QUrl("https://docs.google.com/document/d/1WpDKMk_WoPRrj0Lkst6TptUGEFAC2xYGd3HUBYxPQ-A/edit?usp=sharing"));
}

void WelcomeScreen::checkForUpdate()
{
  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  QObject::connect(lReply, &QNetworkReply::finished, this, &WelcomeScreen::updateCheckFinished);
}

void WelcomeScreen::updateCheckFinished()
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

void WelcomeScreen::displayUpdateMessage(const bool aSucceeded, const QString& aResult)
{
  auto lCurrentVersionStatusLabel{this->findChild<QLabel*>(QString("current_version_status_label"))};
  auto lStableStatusLabel{this->findChild<QLabel*>(QString("stable_status_label"))};
  auto lBetaStatusLabel{this->findChild<QLabel*>(QString("beta_status_label"))};

  // Display error messages to the user
  if (!aSucceeded)
  {
    auto lErrorText{tr("An error has occurred... Make sure your internet connection is operational and try again.")};
    lCurrentVersionStatusLabel->setText(lErrorText);
    lStableStatusLabel->setText(lErrorText);
    lBetaStatusLabel->setText(lErrorText);
    return;
  }

  auto lBrowserCurrentVersionReleaseNotes{this->findChild<QTextBrowser*>(QString("browser_current_version"))};
  auto lBrowserStableReleaseNotes{this->findChild<QTextBrowser*>(QString("browser_stable"))};
  auto lBrowserBetaReleaseNotes{this->findChild<QTextBrowser*>(QString("browser_beta"))};

  const auto lCurrentVersion{Utils::GetApplicationVersion()};
  const auto lVersionsInformation{Utils::ParseGitHubReleasesRequestResult(aResult)};

  /*=================*/
  /* CURRENT VERSION */
  /*=================*/
  // Set the release note in the text browser
  if (lVersionsInformation.sizeStableVersionsList() == 0 && lVersionsInformation.sizeBetaVersionsList() == 0)
  {
    lCurrentVersionStatusLabel->setText(tr("An error has occured while analyzing GitHub's API data. Please retry in a few seconds."));
  }
  else if (lVersionsInformation.isRunningBetaVersion(lCurrentVersion))
  {
    lBrowserCurrentVersionReleaseNotes->show();
    lCurrentVersionStatusLabel->setText(tr("Below are the release notes for the BETA version \"%1\":").arg(lCurrentVersion));
    lBrowserCurrentVersionReleaseNotes->setMarkdown(lVersionsInformation.getBetaReleaseNotes(lCurrentVersion));

    // Links color override
    auto lHTMLString{lBrowserCurrentVersionReleaseNotes->toHtml()};
    Utils::OverrideHTMLLinksColor(lHTMLString, this->settings().display.applicationTheme);
    lBrowserCurrentVersionReleaseNotes->setHtml(lHTMLString);
  }
  else if (lVersionsInformation.stableVersionsListContains(lCurrentVersion))
  {
    lBrowserCurrentVersionReleaseNotes->show();
    lCurrentVersionStatusLabel->setText(tr("Below are the release notes for the stable version \"%1\":").arg(lCurrentVersion));
    lBrowserCurrentVersionReleaseNotes->setMarkdown(lVersionsInformation.getStableReleaseNotes(lCurrentVersion));

    // Links color override
    auto lHTMLString{lBrowserCurrentVersionReleaseNotes->toHtml()};
    Utils::OverrideHTMLLinksColor(lHTMLString, this->settings().display.applicationTheme);
    lBrowserCurrentVersionReleaseNotes->setHtml(lHTMLString);
  }
  else
  {
    lCurrentVersionStatusLabel->setText(tr("The release notes for the version \"%1\" has not been found.").arg(lCurrentVersion));
  }

  /*========*/
  /* STABLE */
  /*========*/
  // Set the release note in the text browser
  lBrowserStableReleaseNotes->setMarkdown(lVersionsInformation.getLatestStableReleaseNotes());

  // Links color override
  auto lHTMLString{lBrowserStableReleaseNotes->toHtml()};
  Utils::OverrideHTMLLinksColor(lHTMLString, this->settings().display.applicationTheme);
  lBrowserStableReleaseNotes->setHtml(lHTMLString);

  // Parsing error
  if (lVersionsInformation.sizeStableVersionsList() == 0)
  {
    lStableStatusLabel->setText(tr("An error has occured while analyzing GitHub's API data. Please retry in a few seconds."));
  }
  // A new version is available
  else if (!lVersionsInformation.isRunningBetaVersion(lCurrentVersion) && Utils::CompareVersionNumbers(lVersionsInformation.getLatestStableVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::NEWER)
  {
    lBrowserStableReleaseNotes->show();
    lStableStatusLabel->setText(tr("The new stable version \"%1\" is available on GitHub.\nPress the button below to open the updater window:").arg(lVersionsInformation.getLatestStableVersionNumber()));
    this->findChild<QPushButton*>(QString("download_stable_update"))->show();
  }
  // Already running the latest version
  else if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestStableVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::EQUIVALENT)
  {
    lStableStatusLabel->setText(tr("You are already running the latest stable version \"%1\".").arg(lVersionsInformation.getLatestStableVersionNumber()));
  }
  // Using a BETA version
  else if (lVersionsInformation.isRunningBetaVersion(lCurrentVersion))
  {
    lBrowserStableReleaseNotes->show();
    lStableStatusLabel->setText(tr("You are running a BETA version. Above are displayed the latest stable version release notes.\nPress the button below to open the updater window:"));
    this->findChild<QPushButton*>(QString("download_stable_update"))->show();
  }
  // Running a developper version (since the current version number is higher than the latest one available on GitHub)
  else if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestStableVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::OLDER)
  {
    lBrowserStableReleaseNotes->show();
    lStableStatusLabel->setText(tr("You are running a developer version."));
  }

  /*======*/
  /* BETA */
  /*======*/
  // Set the release note in the text browser
  lBrowserBetaReleaseNotes->setMarkdown(lVersionsInformation.getLatestBetaReleaseNotes());

  // Links color override
  lHTMLString = lBrowserBetaReleaseNotes->toHtml();
  Utils::OverrideHTMLLinksColor(lHTMLString, this->settings().display.applicationTheme);
  lBrowserBetaReleaseNotes->setHtml(lHTMLString);

  // Parsing error
  if (lVersionsInformation.sizeStableVersionsList() == 0 && lVersionsInformation.sizeBetaVersionsList() == 0)
  {
    lBetaStatusLabel->setText(tr("An error has occured while analyzing GitHub's API data. Please retry in a few seconds."));
  }
  // There is not any BETA version available on GitHub
  else if (lVersionsInformation.sizeBetaVersionsList() == 0)
  {
    lBetaStatusLabel->setText(tr("No BETA version found on GitHub."));
  }
  // The BETA version is older than the current stable
  else if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lVersionsInformation.getLatestStableVersionNumber()) == ApplicationVersionRelative::OLDER)
  {
    lBetaStatusLabel->setText(tr("No newer BETA version is currently available."));
  }
  // Already running the latest version
  else if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::EQUIVALENT)
  {
    lBetaStatusLabel->setText(tr("You are already running the latest BETA version \"%1\".").arg(lVersionsInformation.getLatestBetaVersionNumber()));
  }
  // A newer BETA is available
  else if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::NEWER)
  {
    lBrowserBetaReleaseNotes->show();
    lBetaStatusLabel->setText(tr("The new BETA version \"%1\" is available on GitHub.\nPress the button below to open the updater window:").arg(lVersionsInformation.getLatestBetaVersionNumber()));
    this->findChild<QPushButton*>(QString("download_beta_update"))->show();
  }
  // Running a developper version (since the current version number is higher than the latest one available on GitHub)
  else if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::OLDER)
  {
    lBrowserBetaReleaseNotes->show();
    lBetaStatusLabel->setText(tr("You are running a developer version."));
  }
}
