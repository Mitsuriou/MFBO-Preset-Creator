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
  this->checkForUpdate();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
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
  this->setMinimumHeight(670);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Welcome screen"));
  this->setWindowIcon(QIcon(QPixmap(":/black/home")));
}

void WelcomeScreen::initializeGUI()
{
  // Keep a reference to the user theme
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Main layout
  auto lMainLayout{ComponentFactory::createScrollAreaWindowLayout(this)};

  // Show/Hide the welcome screen
  auto lShowHideWelcomeScreen{new QCheckBox(tr("Always show the welcome screen at application startup"), this)};
  lShowHideWelcomeScreen->setObjectName("always_show_welcome_screen");
  lShowHideWelcomeScreen->setChecked(this->mSettings.showWelcomeScreen);
  lMainLayout->addWidget(lShowHideWelcomeScreen);

  // Main title
  auto lMainTitle{new QLabel(tr("MFBO: Preset Creator v.%1").arg(Utils::getApplicationVersion()), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->mSettings.font.size * 2));
  lMainLayout->addWidget(lMainTitle);

  // Stable version release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Latest stable release notes"), this->mSettings.font.size));

  auto lStableReleaseNotes{new QLabel(tr("Contacting GitHub.com..."), this)};
  lStableReleaseNotes->setObjectName("stable_release_notes");
  lMainLayout->addWidget(lStableReleaseNotes);

  // Download stable update button
  auto lDownloadStableUpdate{ComponentFactory::createButton(this, tr("Download the latest stable update"), "", "cloud-download", lIconFolder, "download_stable_update", false, true)};
  lMainLayout->addWidget(lDownloadStableUpdate);
  lDownloadStableUpdate->hide();

  // Beta version release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Latest beta release notes"), this->mSettings.font.size));

  auto lBetaReleaseNotes{new QLabel(tr("Contacting GitHub.com..."), this)};
  lBetaReleaseNotes->setObjectName("beta_release_notes");
  lMainLayout->addWidget(lBetaReleaseNotes);

  // Download BETA update button
  auto lDownloadBetaUpdate{ComponentFactory::createButton(this, tr("Download the latest BETA update"), "", "cloud-download", lIconFolder, "download_beta_update", false, true)};
  lMainLayout->addWidget(lDownloadBetaUpdate);
  lDownloadBetaUpdate->hide();

  // Incoming features
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Incoming features"), this->mSettings.font.size));

  auto lIncomingFeaturesText{QString("<p style='padding: 0px; margin: 0px;'>"
                                     "<span>Batch Conversion tool.</span><br />"
                                     "<span>Portable installer archive.</span>"
                                     "</p>")};

  auto lIncomingFeatures{new QLabel(this)};
  lIncomingFeatures->setTextFormat(Qt::RichText);
  lIncomingFeatures->setText(lIncomingFeaturesText);
  lMainLayout->addWidget(lIncomingFeatures);

  // Known issues
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Known issues"), this->mSettings.font.size));

  auto lKnownIssuesText{QString("<p style='padding: 0px; margin: 0px;'>"
                                "<span>Collapsing a groupbox on a small resolution window makes the GUI blink.</span>"
                                "</p>")};

  auto lKnownIssues{new QLabel(this)};
  lKnownIssues->setTextFormat(Qt::RichText);
  lKnownIssues->setText(lKnownIssuesText);
  lMainLayout->addWidget(lKnownIssues);

  // Guide and tutorials
  lMainLayout->addWidget(this->createTitleLabel(this, tr("User guide and tutorials"), this->mSettings.font.size));

  auto lGuideLabel{new QLabel(tr("Whether it is your first time using the application or you're wondering how a particular feature works, you should check the user guide and detailed tutorials by cliking the link below:"), this)};
  lGuideLabel->setWordWrap(true);
  lMainLayout->addWidget(lGuideLabel);

  auto lOpenGuideTutorials{ComponentFactory::createButton(this, "User guide and tutorials (docs.google.com)", "", "external", lIconFolder, "", false, true)};
  lMainLayout->addWidget(lOpenGuideTutorials);

  // Event binding
  this->connect(lDownloadStableUpdate, &QPushButton::clicked, this, &WelcomeScreen::launchUpdateDialog);
  this->connect(lDownloadBetaUpdate, &QPushButton::clicked, this, &WelcomeScreen::launchUpdateDialog);
  this->connect(lOpenGuideTutorials, &QPushButton::clicked, this, &WelcomeScreen::openGoogleDriveGuide);

  // Cursor change for the scroll bar
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &WelcomeScreen::scrollbarPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &WelcomeScreen::scrollbarReleased);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &WelcomeScreen::scrollbarPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &WelcomeScreen::scrollbarReleased);
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
    new Update(this, this->mSettings, false);
  }
  else if (lEventSource == this->findChild<QPushButton*>("download_beta_update"))
  {
    new Update(this, this->mSettings, true);
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
  auto lStableReleaseNotes{this->findChild<QLabel*>("stable_release_notes")};
  auto lBetaReleaseNotes{this->findChild<QLabel*>("beta_release_notes")};
  auto lMainLayout{this->findChild<QGridLayout*>("main_layout")};

  auto lStableVersions{QStringList()};
  auto lLatestStableReleaseNote{QString()};
  auto lBetaVersions{QStringList()};
  auto lLatestBetaReleaseNote{QString()};
  const auto lCurrentVersion{Utils::getApplicationVersion()};

  if (aResult == "fetch_error")
  {
    lStableReleaseNotes->setText(tr("An error has occurred... Make sure your internet connection is operational and try again."));
    lBetaReleaseNotes->setText(tr("An error has occurred... Make sure your internet connection is operational and try again."));
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
        lLatestBetaReleaseNote = lTagsArray.at(i)["body"].toString();

      // Add this version name to the beta versions list
      lBetaVersions.push_back(lTagName);
    }
    else
    {
      // Save the latest stable's body tag value
      if (lLatestStableReleaseNote.length() == 0)
        lLatestStableReleaseNote = lTagsArray.at(i)["body"].toString();

      // Add this version name to the stable versions list
      lStableVersions.push_back(lTagName);
    }
  }

  // Delete the stable label
  delete lStableReleaseNotes;
  lStableReleaseNotes = nullptr;

  // Create a QTextBrowser instead
  auto lBrowserStableReleaseNotes{new QTextBrowser(this)};
  lBrowserStableReleaseNotes->setOpenExternalLinks(true);
  Utils::cleanBreaksString(lLatestStableReleaseNote);
  lBrowserStableReleaseNotes->setMarkdown(lLatestStableReleaseNote);
  lMainLayout->addWidget(lBrowserStableReleaseNotes, 3, 0);

  // Links color override
  auto lHTMLString{lBrowserStableReleaseNotes->toHtml()};
  this->overrideHTMLLinksColor(lHTMLString);
  lBrowserStableReleaseNotes->setHtml(lHTMLString);

  // Show the download button if the version is newer than the current one
  if (lStableVersions.size() > 0 && Utils::compareVersionNumbers(lStableVersions.at(0), lCurrentVersion) == ApplicationVersionRelative::NEWER)
  {
    this->findChild<QPushButton*>("download_stable_update")->show();
  }

  // BETA versions messages
  if (lBetaVersions.size() > 0 && Utils::compareVersionNumbers(lBetaVersions.at(0), lCurrentVersion) == ApplicationVersionRelative::EQUIVALENT)
  {
    lBetaReleaseNotes->setText(tr("You are already running the latest BETA version."));
    return;
  }

  if (lBetaVersions.size() == 0 || (lBetaVersions.size() > 0 && Utils::compareVersionNumbers(lBetaVersions.at(0), lStableVersions.at(0)) == ApplicationVersionRelative::OLDER))
  {
    lBetaReleaseNotes->setText(tr("No newer BETA version available."));
    return;
  }

  // Delete the beta label
  delete lBetaReleaseNotes;
  lBetaReleaseNotes = nullptr;

  // Show the download button
  this->findChild<QPushButton*>("download_beta_update")->show();

  // Create a QTextBrowser instead
  auto lBrowserBetaReleaseNotes{new QTextBrowser(this)};
  lBrowserBetaReleaseNotes->setOpenExternalLinks(true);
  Utils::cleanBreaksString(lLatestBetaReleaseNote);
  lBrowserBetaReleaseNotes->setMarkdown(lLatestBetaReleaseNote);
  lMainLayout->addWidget(lBrowserBetaReleaseNotes, 6, 0);

  Utils::cleanBreaksString(lLatestBetaReleaseNote);
  lBrowserBetaReleaseNotes->setMarkdown(lLatestBetaReleaseNote);

  // Links color override
  lHTMLString = lBrowserBetaReleaseNotes->toHtml();
  this->overrideHTMLLinksColor(lHTMLString);
  lBrowserBetaReleaseNotes->setHtml(lHTMLString);
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

void WelcomeScreen::scrollbarPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void WelcomeScreen::scrollbarReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}

void WelcomeScreen::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
