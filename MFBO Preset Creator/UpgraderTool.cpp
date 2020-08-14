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

void UpgraderTool::closeEvent(QCloseEvent* aEvent)
{
  auto lResult{ QMessageBox::question(this, tr("Closing"),
    tr("Are you sure you want to close the CBBE 3BBB Version [Up/Down]grader Tool?"),
    QMessageBox::Yes | QMessageBox::No, QMessageBox::No) };

  if (lResult != QMessageBox::Yes)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
  }
}

void UpgraderTool::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setMinimumWidth(650);
  this->setWindowTitle(tr("CBBE 3BBB Version [Up/Down]grader Tool"));
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
  auto lCbbe3BBBVersionLabel{ new QLabel(tr("Targeted CBBE 3BBB version:")) };
  aLayout.addWidget(lCbbe3BBBVersionLabel, 0, 0);

  QStringList lVersions;
  lVersions.append(QString("1.40"));
  lVersions.append(QString("1.50"));
  lVersions.append(QString("1.51"));

  auto lCbbe3BBBVersionSelector{ new QComboBox() };
  lCbbe3BBBVersionSelector->addItems(lVersions);
  //lCbbe3BBBVersionSelector->setCurrentIndex(lCbbe3BBBVersionSelector->count() - 1);
  lCbbe3BBBVersionSelector->setObjectName(QString("cbbe_3bbb_version"));
  aLayout.addWidget(lCbbe3BBBVersionSelector, 0, 1, 1, 2);

  // Second line
  auto lInputPathLabel{ new QLabel(tr("Input path:")) };
  aLayout.addWidget(lInputPathLabel, 1, 0);

  auto lInputPathLineEdit{ new QLineEdit("") };
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  aLayout.addWidget(lInputPathLineEdit, 1, 1);

  auto lInputPathChooser{ new QPushButton(tr("Choose a directory...")) };
  aLayout.addWidget(lInputPathChooser, 1, 2);

  // Third line
  auto lKeepBackupLabel{ new QLabel(tr("Keep a backup?")) };
  aLayout.addWidget(lKeepBackupLabel, 2, 0);

  auto lKeepBackup{ new QCheckBox(tr("You should always check this box to avoid any data loss or corruption.")) };
  lKeepBackup->setObjectName("keep_backup");
  aLayout.addWidget(lKeepBackup, 2, 1, 1, 2);

  // Fourth line
  auto lBackupPathLabel{ new QLabel(tr("Backup directory path:")) };
  lBackupPathLabel->setObjectName("backup_path_label");
  aLayout.addWidget(lBackupPathLabel, 3, 0);

  auto lBackupPathLineEdit{ new QLineEdit("") };
  lBackupPathLineEdit->setReadOnly(true);
  lBackupPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lBackupPathLineEdit->setObjectName("backup_path_directory");
  aLayout.addWidget(lBackupPathLineEdit, 3, 1);

  auto lBackupPathChooser{ new QPushButton(tr("Choose a directory...")) };
  lBackupPathChooser->setObjectName("backup_dir_chooser");
  aLayout.addWidget(lBackupPathChooser, 3, 2);

  // Fifth line
  auto lLabelSubDirectoryBackupPath{ new QLabel(tr("Backup subdirectory name/path:")) };
  lLabelSubDirectoryBackupPath->setObjectName("backup_subdir_label");
  aLayout.addWidget(lLabelSubDirectoryBackupPath, 4, 0);

  auto lBackupSubpathLineEdit{ new QLineEdit("") };
  lBackupSubpathLineEdit->setObjectName("backup_path_subdirectory");
  aLayout.addWidget(lBackupSubpathLineEdit, 4, 1);

  // Sixth line
  auto lBackupPathPreviewLabel{ new QLabel(tr("Preview:")) };
  lBackupPathPreviewLabel->setObjectName("backup_path_preview_label");
  aLayout.addWidget(lBackupPathPreviewLabel, 5, 0);

  auto lBackupPathsPreview{ new QLabel("") };
  lBackupPathsPreview->setObjectName("backup_path_preview");
  aLayout.addWidget(lBackupPathsPreview, 5, 1, 1, 2);

  // Seventh line
  auto lReformatPresetNamesLabel{ new QLabel(tr("Reformat presets names?")) };
  aLayout.addWidget(lReformatPresetNamesLabel, 6, 0);

  auto lShouldReformatPresetNames{ new QCheckBox(tr("If checked, \"* - Feet\" would become \"* - CBBE 3BBB Feet\" when upgrading from 1.40 to 1.51, for example.")) };
  aLayout.addWidget(lShouldReformatPresetNames, 6, 1, 1, 2);

  // Generate button
  auto lGenerateButton{ new QPushButton(tr("[Up/Down]grade all the files under the input path")) };
  aLayout.addWidget(lGenerateButton, 7, 0, 1, 3, Qt::AlignBottom);

  // Event binding
  connect(lInputPathChooser, SIGNAL(clicked()), this, SLOT(chooseInputDirectory()));
  connect(lKeepBackup, SIGNAL(clicked()), this, SLOT(switchBackupState()));
  connect(lBackupPathChooser, SIGNAL(clicked()), this, SLOT(chooseBackupDirectory()));
  connect(lBackupSubpathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBackupPreview()));
  connect(lGenerateButton, SIGNAL(clicked()), this, SLOT(launchUpDownGradeProcess()));

  // Pre-filled data
  lKeepBackup->setChecked(true);
  this->switchBackupState();
  this->updateBackupPreview();
}

void UpgraderTool::chooseInputDirectory()
{
  auto lLineEdit{ this->findChild<QLineEdit*>("input_path_directory") };
  auto lPath{ QFileDialog::getExistingDirectory(this, "", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)) };
  lLineEdit->setText(lPath);
}

void UpgraderTool::chooseBackupDirectory()
{
  auto lLineEdit{ this->findChild<QLineEdit*>("backup_path_directory") };
  auto lPath{ QFileDialog::getExistingDirectory(this, "", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)) };
  lLineEdit->setText(lPath);

  this->updateBackupPreview();
}

void UpgraderTool::updateBackupPreview()
{
  // Get main directory
  auto lMainDirTextEdit{ this->findChild<QLineEdit*>("backup_path_directory") };
  auto lMainDirectory{ lMainDirTextEdit->text().trimmed() };
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{ this->findChild<QLineEdit*>("backup_path_subdirectory")->text().trimmed() };
  Utils::cleanPathString(lSubDirectory);

  // Construct full path
  auto lFullPath(QString(""));
  if (lMainDirectory.length() > 0 && lSubDirectory.length() > 0)
  {
    lFullPath = lMainDirectory + "/" + lSubDirectory;
    lMainDirTextEdit->setDisabled(false);
  }
  else if (lMainDirectory.length() > 0 && lSubDirectory.length() == 0)
  {
    lFullPath = lMainDirectory;
    lMainDirTextEdit->setDisabled(false);
  }
  else
  {
    lFullPath = tr("No path given or invalid path given.");
    lMainDirTextEdit->setDisabled(true);
  }

  // Set the full path value in the preview label
  auto lOutputPathsPreview{ this->findChild<QLabel*>("backup_path_preview") };
  lOutputPathsPreview->setText(lFullPath);
}

void UpgraderTool::switchBackupState()
{
  auto lKeepBackup{ this->findChild<QCheckBox*>("keep_backup") };

  auto lBackupPathLabel{ this->findChild<QLabel*>("backup_path_label") };
  auto lBackupPathLineEdit{ this->findChild<QLineEdit*>("backup_path_directory") };
  auto lBackupPathChooser{ this->findChild<QPushButton*>("backup_dir_chooser") };
  auto lLabelSubDirectoryBackupPath{ this->findChild<QLabel*>("backup_subdir_label") };
  auto lBackupSubpathLineEdit{ this->findChild<QLineEdit*>("backup_path_subdirectory") };
  auto lOutputPathsPreviewLabel{ this->findChild<QLabel*>("backup_path_preview_label") };
  auto lOutputPathsPreview{ this->findChild<QLabel*>("backup_path_preview") };

  if (lKeepBackup->isChecked())
  {
    lBackupPathLabel->setDisabled(false);
    lBackupPathLineEdit->setDisabled(false);
    lBackupPathChooser->setDisabled(false);
    lLabelSubDirectoryBackupPath->setDisabled(false);
    lBackupSubpathLineEdit->setDisabled(false);
    lOutputPathsPreviewLabel->setDisabled(false);
    lOutputPathsPreview->setDisabled(false);
    this->updateBackupPreview();
  }
  else
  {
    this->updateBackupPreview();
    lBackupPathLabel->setDisabled(true);
    lBackupPathLineEdit->setDisabled(true);
    lBackupPathChooser->setDisabled(true);
    lLabelSubDirectoryBackupPath->setDisabled(true);
    lBackupSubpathLineEdit->setDisabled(true);
    lOutputPathsPreviewLabel->setDisabled(true);
    lOutputPathsPreview->setDisabled(true);
  }
}

void UpgraderTool::launchUpDownGradeProcess()
{
  auto lRootDir{ this->findChild<QLineEdit*>("input_path_directory")->text() };

  // Check if the input path has been given by the user
  if (lRootDir.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no path path given to [up/down]grade."));
    return;
  }

  // Manage the backup part
  auto lMustKeepBackup{ this->findChild<QCheckBox*>("keep_backup")->isChecked() };

  if (lMustKeepBackup)
  {
    // Backup paths
    auto lMainDirectory{ this->findChild<QLineEdit*>("backup_path_directory")->text().trimmed() };
    auto lSubDirectory{ this->findChild<QLineEdit*>("backup_path_subdirectory")->text().trimmed() };
    Utils::cleanPathString(lSubDirectory);

    // Full extract path
    auto lFullBackupDirectory{ (lSubDirectory.length() == 0 ? lMainDirectory : (lMainDirectory + "/" + lSubDirectory)) };

    // Check if the full extract path has been given by the user
    if (lFullBackupDirectory.length() == 0)
    {
      Utils::displayWarningMessage(tr("Error: no path given to backup the files."));
      return;
    }

    // Check if the path could be valid
    if (lFullBackupDirectory.startsWith("/"))
    {
      Utils::displayWarningMessage(tr("Error: the path given to backup the files seems to be invalid."));
      return;
    }

    // Copy the directory and its content
    auto lBackupDoneSuccessfully{ Utils::copyRecursively(lRootDir, lFullBackupDirectory) };

    if (!lBackupDoneSuccessfully)
    {
      return;
    }
  }

  // Scan the number of files to treat
  // TODO: ignore the fomod directory here!! (maybe not needed since it does not have the same structure, but it can be long to parse)
  auto lNumberOSPFiles{ Utils::getNumberFilesByExtension(lRootDir, "osp") };
  auto lNumberXMLFiles{ Utils::getNumberFilesByExtension(lRootDir, "xml") };
  auto lFilesToTreat{ lNumberOSPFiles + lNumberXMLFiles };

  // Cancel button
  auto lCancelButton{ new QPushButton(tr("Cancel treatment")) };

  // Progress bar
  auto lProgressbar{ new QProgressBar() };
  lProgressbar->setFormat("%v / %m");
  lProgressbar->setRange(0, lFilesToTreat);
  lProgressbar->setValue(0);
  lProgressbar->setTextVisible(true);

  // Progress dialog
  auto lProgressDialog{ new QProgressDialog(tr("Scanning, parsing and building the new BodySlide files. Please wait..."), "", 0, 0, this) };
  lProgressDialog->setCancelButton(lCancelButton);
  lProgressDialog->setBar(lProgressbar);
  lProgressDialog->setWindowFlags(lProgressDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  lProgressDialog->exec();
}
