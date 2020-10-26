#include "Update.h"

Update::Update(QWidget* parent)
  : QDialog(parent)
  , mSettings(Utils::loadSettingsFromFile())
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  this->refreshUI();

  // Show the window when it's completely built
  this->show();
}

void Update::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Check for updates"));
}

void Update::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QGridLayout(this)};

  this->setupInterface(*lMainVertical);
}

void Update::setupInterface(QGridLayout& aLayout)
{
  QLabel* lLastVersion{new QLabel(this->getLastAvailableVersion(), this)};
  aLayout.addWidget(lLastVersion);
}

void Update::refreshUI()
{
  // Set the font properties
  QFont lFont(mSettings.fontFamily, mSettings.fontSize, -1, false);
  this->setFont(lFont);
  this->setStyleSheet("font-family: \"" + mSettings.fontFamily + "\"; font-size: " + QString::number(mSettings.fontSize) + "px;");
}

QString Update::getLastAvailableVersion()
{
  // Fetch HTTP url
  HTTPDownloader downloader;
  std::string content{downloader.download("https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/latest")};
  QString lFetchedHTTPData{QString::fromStdString(content)};

  // Create a JSON from the fetched string and parse the "tag_name" data
  QJsonDocument doc{QJsonDocument::fromJson(lFetchedHTTPData.toUtf8())};
  QJsonObject obj{doc.object()};
  return obj["tag_name"].toString();
}
