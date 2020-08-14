#include "UpgraderTool.h"

UpgraderTool::UpgraderTool(QWidget* parent)
  : QDialog(parent)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->show();
}

void UpgraderTool::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);

  this->setMinimumSize(700, 500);
  this->setFixedSize(700, 500);
  this->setWindowTitle(tr("CBBE 3BBB Version Upgrader Tool"));
}

void UpgraderTool::initializeGUI()
{
  // Main window container
  auto lMainVertical{ new QGridLayout(this) };

  this->setupInterface(*lMainVertical);
}

void UpgraderTool::setupInterface(QGridLayout& aLayout)
{
  // First line
  auto lCbbe3BBBVersionLabel{ new QLabel(tr("CBBE 3BBB version to upgrade the existing preset(s) to:")) };
  aLayout.addWidget(lCbbe3BBBVersionLabel, 0, 0);

  QStringList lVersions;
  lVersions.append(QString("1.40"));
  lVersions.append(QString("1.50"));
  lVersions.append(QString("1.51"));

  auto lCbbe3BBBVersionSelector{ new QComboBox() };
  lCbbe3BBBVersionSelector->addItems(lVersions);
  lCbbe3BBBVersionSelector->setCurrentIndex(lCbbe3BBBVersionSelector->count() - 1);
  lCbbe3BBBVersionSelector->setObjectName(QString("cbbe_3bbb_version"));
  aLayout.addWidget(lCbbe3BBBVersionSelector, 0, 1);

  // TODO:
  // qfilechooser Root folder to scan and rename
  // qcheckbox Keep a backup
    // -> qfilechooser Folder to store the backup
    // -> qlineedit Subfolder if needed (because it will not be possible to move a folder to an existing one)
  // qcheckbox Reformat the preset names (keep the name and reformat the CBBE 3BBB name to match the chosen export version)
}
