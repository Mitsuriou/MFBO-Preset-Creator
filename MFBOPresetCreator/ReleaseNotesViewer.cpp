#include "ReleaseNotesViewer.h"
#include "Enum.h"
#include "Utils.h"
#include <QNetworkReply>
#include <QTextBrowser>

ReleaseNotesViewer::ReleaseNotesViewer(QWidget* aParent, const Struct::Settings& aSettings)
  : QDialog(aParent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();

  this->checkForUpdate();
}

void ReleaseNotesViewer::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setMinimumHeight(500);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Current version's release notes"));
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowIcon(QIcon(QPixmap(":/black/text-snippet")));
}

void ReleaseNotesViewer::initializeGUI()
{
  // Set a layout for this dialog box
  this->setLayout(new QVBoxLayout(this));
  this->layout()->setAlignment(Qt::AlignTop);

  /*============*/
  /* Main title */
  /*============*/
  auto lMainTitle{new QLabel(tr("Current version's release notes"), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->mSettings.font.size * 2));
  this->layout()->addWidget(lMainTitle);

  auto lViewer{new QTextBrowser(this)};
  lViewer->setObjectName("viewer");
  lViewer->setOpenExternalLinks(true);
  this->layout()->addWidget(lViewer);
}

void ReleaseNotesViewer::overrideHTMLLinksColor(QString& aHTMLString)
{
  // If no color change is needed
  if (this->mSettings.appTheme != GUITheme::MITSURIOU_BLACK_THEME
      && this->mSettings.appTheme != GUITheme::MITSURIOU_DARK_THEME
      && this->mSettings.appTheme != GUITheme::MITSURIOU_LIGHT_THEME)
  {
    return;
  }

  // Hacky links' colors override for some themes
  const auto lLinksColorOverride{this->mSettings.appTheme == GUITheme::MITSURIOU_BLACK_THEME ? QString("color:#3991ff") : QString("color:#e95985")};

  // Go through the string to find the link colors
  auto i{0};
  while ((i = aHTMLString.indexOf("color:#0000ff", i)) != -1)
  {
    aHTMLString.replace(i, lLinksColorOverride.size(), lLinksColorOverride);
  }
}

void ReleaseNotesViewer::checkForUpdate()
{
  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  connect(lReply, &QNetworkReply::finished, this, &ReleaseNotesViewer::updateCheckFinished);
}

void ReleaseNotesViewer::updateCheckFinished()
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

void ReleaseNotesViewer::displayUpdateMessage(const QString& aResult)
{
  auto lViewer{this->findChild<QTextBrowser*>(QString("viewer"))};

  // Display error messages to the user
  if (aResult == "fetch_error")
  {
    // TODO
    return;
  }

  const auto lCurrentVersion{Utils::getApplicationVersion()};
  const auto lVersionsInformation{Utils::parseGitHubReleasesRequestResult(aResult)};

  // Set the release note in the text browser
  lViewer->setMarkdown(lVersionsInformation.getLatestStableReleaseNotes());

  // Links color override
  auto lHTMLString{lViewer->toHtml()};
  this->overrideHTMLLinksColor(lHTMLString);
  lViewer->setHtml(lHTMLString);
}
