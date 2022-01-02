#include "ReleaseNotesViewer.h"
#include "ComponentFactory.h"
#include "Enum.h"
#include "Utils.h"
#include <QDesktopServices>
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
  this->setWindowIcon(QIcon(QPixmap(":/black/text-snippet")));
}

void ReleaseNotesViewer::initializeGUI()
{
  // Set a layout for this dialog box
  this->setLayout(new QVBoxLayout(this));
  this->layout()->setAlignment(Qt::AlignTop);

  // Main title
  auto lMainTitle{new QLabel(tr("Current version's release notes"), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->mSettings.display.font.size * 2));
  this->layout()->addWidget(lMainTitle);

  // Fetch status
  auto lFetchStatus{new QLabel(this)};
  lFetchStatus->setObjectName(QString("fetch_status"));
  lFetchStatus->hide();
  this->layout()->addWidget(lFetchStatus);

  // Viewer
  auto lViewer{new QTextBrowser(this)};
  lViewer->setObjectName(QString("viewer"));
  lViewer->setOpenExternalLinks(true);
  this->layout()->addWidget(lViewer);

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Open in default browser button
  auto lButton{ComponentFactory::CreateButton(this, tr("View in default browser"), "", "external", lIconFolder)};
  this->layout()->addWidget(lButton);

  // Event binding
  this->connect(lButton, &QPushButton::clicked, this, &ReleaseNotesViewer::viewInDefaultBrowser);
}

void ReleaseNotesViewer::checkForUpdate()
{
  auto lFetchStatus{this->findChild<QLabel*>(QString("fetch_status"))};
  lFetchStatus->setText(tr("Contacting GitHub.com..."));
  lFetchStatus->show();

  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  connect(lReply, &QNetworkReply::finished, this, &ReleaseNotesViewer::updateCheckFinished);
}

void ReleaseNotesViewer::updateCheckFinished()
{
  auto lReply{qobject_cast<QNetworkReply*>(this->sender())};

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
  auto lFetchStatus{this->findChild<QLabel*>(QString("fetch_status"))};
  auto lViewer{this->findChild<QTextBrowser*>(QString("viewer"))};

  // Display error messages to the user
  if (aResult == "fetch_error")
  {
    lFetchStatus->setText(tr("An error has occurred while searching for a new version.\nPlease make sure your internet connection is working correctly and try again."));
    return;
  }

  const auto lCurrentVersion{Utils::GetApplicationVersion()};
  const auto lVersionsInformation{Utils::ParseGitHubReleasesRequestResult(aResult)};

  // Set the release note in the text browser
  if (lVersionsInformation.sizeStableVersionsList() == 0 && lVersionsInformation.sizeBetaVersionsList() == 0)
  {
    lFetchStatus->setText(tr("An error has occured while analyzing GitHub's API data. Please retry in a few seconds."));
  }
  else if (lVersionsInformation.isRunningBetaVersion(lCurrentVersion))
  {
    lFetchStatus->setText(tr("Below are the release notes for the BETA version \"%1\":").arg(lCurrentVersion));
    lViewer->setMarkdown(lVersionsInformation.getBetaReleaseNotes(lCurrentVersion));
  }
  else if (lVersionsInformation.stableVersionsListContains(lCurrentVersion))
  {
    lFetchStatus->setText(tr("Below are the release notes for the stable version \"%1\":").arg(lCurrentVersion));
    lViewer->setMarkdown(lVersionsInformation.getStableReleaseNotes(lCurrentVersion));
  }
  else
  {
    lFetchStatus->setText(tr("The release notes for the version \"%1\" has not been found.").arg(lCurrentVersion));
    return;
  }

  // Links color override
  auto lHTMLString{lViewer->toHtml()};
  Utils::OverrideHTMLLinksColor(lHTMLString, this->mSettings.display.applicationTheme);
  lViewer->setHtml(lHTMLString);
}

void ReleaseNotesViewer::viewInDefaultBrowser()
{
  const auto lCurrentVersion{Utils::GetApplicationVersion()};
  QDesktopServices::openUrl(QUrl(QString("https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/tag/%1").arg(lCurrentVersion)));
}
