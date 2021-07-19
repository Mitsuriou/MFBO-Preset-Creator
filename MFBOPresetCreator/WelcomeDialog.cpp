#include "WelcomeDialog.h"
#include "ComponentFactory.h"
#include "Update.h"
#include "Utils.h"
#include <QCloseEvent>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QScrollArea>
#include <QScrollBar>
#include <QTextBrowser>
#include <QTimer>
#include <QVBoxLayout>

WelcomeDialog::WelcomeDialog(QWidget* aParent, const Struct::Settings& aSettings)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();

  this->initializeGUI();
  //QTimer::singleShot(10000, this, [this]() { this->checkForUpdate(); }); // TODO: Remove this debug line
  this->checkForUpdate();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void WelcomeDialog::closeEvent(QCloseEvent* aEvent)
{
  aEvent->accept();
}

void WelcomeDialog::reject()
{
  this->close();
}

void WelcomeDialog::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setMinimumHeight(500);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Welcome screen"));
  this->setWindowIcon(QIcon(QPixmap(":/black/home")));
}

void WelcomeDialog::initializeGUI()
{
  // Keep a reference to the user theme
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Main layout
  auto lMainLayout{ComponentFactory::createScrollAreaWindowLayout(this)};

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

  // Beta version release notes
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Latest beta release notes"), this->mSettings.font.size));

  auto lBetaReleaseNotes{new QLabel(tr("Contacting GitHub.com..."), this)};
  lBetaReleaseNotes->setObjectName("beta_release_notes");
  lMainLayout->addWidget(lBetaReleaseNotes);

  // Incoming features
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Incoming features"), this->mSettings.font.size));

  auto lIncomingFeaturesText{QString("TODO")};

  auto lIncomingFeatures{new QLabel(this)};
  //lIncomingFeatures->setTextFormat(Qt::RichText);
  //lIncomingFeatures->setTextInteractionFlags(Qt::TextBrowserInteraction);
  //lIncomingFeatures->setOpenExternalLinks(true);
  //lIncomingFeatures->setText(lIncomingFeaturesText);
  //lIncomingFeatures->adjustSize();
  //lIncomingFeatures->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  lMainLayout->addWidget(lIncomingFeatures);

  // Known issues
  lMainLayout->addWidget(this->createTitleLabel(this, tr("Known issues"), this->mSettings.font.size));

  auto lKnownIssuesText{QString("TODO")};

  auto lKnownIssues{new QLabel(this)};
  //lKnownIssues->setTextFormat(Qt::RichText);
  //lKnownIssues->setTextInteractionFlags(Qt::TextBrowserInteraction);
  //lKnownIssues->setOpenExternalLinks(true);
  //lKnownIssues->setText(lKnownIssuesText);
  //lKnownIssues->adjustSize();
  //lKnownIssues->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  lMainLayout->addWidget(lKnownIssues);

  // Guide and tutorials
  lMainLayout->addWidget(this->createTitleLabel(this, tr("User guide and tutorials"), this->mSettings.font.size));

  // TODO: Make a better link to the guide
  //auto lOpenGuideTutorials{ComponentFactory::createButton(this, "User guide and tutorials (docs.google.com)", "", "text-file", lIconFolder, "", false, true)};
  //lMainLayout->addWidget(lOpenGuideTutorials);

  // Cursor change for the scroll bar
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &WelcomeDialog::scrollbarPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &WelcomeDialog::scrollbarReleased);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &WelcomeDialog::scrollbarPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &WelcomeDialog::scrollbarReleased);
}

QLabel* WelcomeDialog::createTitleLabel(QWidget* aParent, const QString& aText, const int aAppFontSize)
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

void WelcomeDialog::overrideHTMLLinksColor(QString& aHTMLString)
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

void WelcomeDialog::launchUpdateDialog()
{
  new Update(this, this->mSettings);
}

void WelcomeDialog::checkForUpdate()
{
  //QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  //QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  //connect(lReply, &QNetworkReply::finished, this, &WelcomeDialog::updateCheckFinished);

  // DEBUG
  // TODO: Remove
  this->displayUpdateMessage(Utils::readQRCFileContent(":/application/debug_request"));
}

void WelcomeDialog::displayUpdateMessage(const QString& aResult)
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
  lMainLayout->addWidget(lBrowserStableReleaseNotes, 2, 0);

  // Links color override
  auto lHTMLString{lBrowserStableReleaseNotes->toHtml()};
  this->overrideHTMLLinksColor(lHTMLString);
  lBrowserStableReleaseNotes->setHtml(lHTMLString);

  // BETA
  if (lBetaVersions.size() == 0
      || (lBetaVersions.size() > 0 && Utils::compareVersionNumbers(lBetaVersions.at(0), lCurrentVersion) != ApplicationVersionRelative::NEWER))
  {
    lBetaReleaseNotes->setText(tr("No newer BETA version available."));
    return;
  }

  // Delete the beta label
  delete lBetaReleaseNotes;
  lBetaReleaseNotes = nullptr;

  // Create a QTextBrowser instead
  auto lBrowserBetaReleaseNotes{new QTextBrowser(this)};
  lBrowserBetaReleaseNotes->setOpenExternalLinks(true);
  Utils::cleanBreaksString(lLatestBetaReleaseNote);
  lBrowserBetaReleaseNotes->setMarkdown(lLatestBetaReleaseNote);
  lMainLayout->addWidget(lBrowserBetaReleaseNotes, 4, 0);

  Utils::cleanBreaksString(lLatestBetaReleaseNote);
  lBrowserBetaReleaseNotes->setMarkdown(lLatestBetaReleaseNote);

  // Links color override
  lHTMLString = lBrowserBetaReleaseNotes->toHtml();
  this->overrideHTMLLinksColor(lHTMLString);
  lBrowserBetaReleaseNotes->setHtml(lHTMLString);
}

void WelcomeDialog::updateCheckFinished()
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

void WelcomeDialog::scrollbarPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void WelcomeDialog::scrollbarReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}

void WelcomeDialog::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
