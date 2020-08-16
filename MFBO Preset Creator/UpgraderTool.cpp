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

  // Generate button
  auto lGenerateButton{ new QPushButton(tr("[Up/Down]grade all the files under the input path")) };
  aLayout.addWidget(lGenerateButton, 6, 0, 1, 3, Qt::AlignBottom);

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
  auto lCBBE3BBBVersionSelected{ this->findChild<QComboBox*>("cbbe_3bbb_version")->currentIndex() };
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
  auto lTreatedFiles{ 0 };

  // Progress bar
  auto lProgressbar{ new QProgressBar() };
  lProgressbar->setFormat("%v / %m");
  lProgressbar->setRange(0, lFilesToTreat);
  lProgressbar->setValue(0);
  lProgressbar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Scanning, parsing and building the new BodySlide files. Please wait..."), tr("Cancel Treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressbar);
  lProgressDialog.setWindowFlags(lProgressDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
  lProgressDialog.show();

  // Iterate through all the files
  QString lPresetName;
  QDirIterator it(lRootDir, QStringList() << QString("*.xml") << QString("*.osp"), QDir::Files, QDirIterator::Subdirectories);

  while (it.hasNext())
  {
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by user."));
      return;
    }

    it.next();

    auto lFileSuffix{ it.fileInfo().completeSuffix() };

    if (lFileSuffix == "xml")
    {
      auto lXMLPathName{ it.fileInfo().absoluteFilePath() };

      lPresetName = Utils::getPresetNameFromXMLFile(lXMLPathName);
      bool lMustUseBeastHands{ Utils::isPresetUsingBeastHands(lXMLPathName) };

      auto lRessourcesFolder{ QString("") };

      switch (lCBBE3BBBVersionSelected)
      {
      case CBBE3BBBVersion::Version1_40:
        lRessourcesFolder = "cbbe_3bbb_1.40";
        break;
      case CBBE3BBBVersion::Version1_50:
        lRessourcesFolder = "cbbe_3bbb_1.50";
        break;
      case CBBE3BBBVersion::Version1_51:
        lRessourcesFolder = "cbbe_3bbb_1.51";
        break;
      default:
        Utils::displayWarningMessage(tr("Error while searching for the CBBE 3BBB version. If it happens, try restarting the program. If the error is still here after restarting the program, contact the developer."));
        return;
      }

      // Remove the file once all data has been read
      if (QFile::exists(lXMLPathName))
      {
        QFile::remove(lXMLPathName);
      }

      // Copy the XML file
      if (lMustUseBeastHands)
      {
        if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_xml", lXMLPathName))
        {
          Utils::displayWarningMessage(tr("The XML file could not be created. Did you execute the program with limited permissions?"));
          return;
        }
      }
      else
      {
        if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_xml", lXMLPathName))
        {
          Utils::displayWarningMessage(tr("The XML file could not be created. Did you execute the program with limited permissions?"));
          return;
        }
      }

      QFile lXMLFile(lXMLPathName);
      lXMLFile.setPermissions(QFile::WriteUser);

      QByteArray lTempXMLContent;

      if (lXMLFile.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        lTempXMLContent = lXMLFile.readAll();
        lXMLFile.close();
      }
      else
      {
        Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lXMLPathName + tr("\"."));
        return;
      }

      if (lTempXMLContent.length() > 0)
      {
        if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
          auto lTextToParse{ static_cast<QString>(lTempXMLContent) };
          lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lPresetName);

          QTextStream lTextStream(&lXMLFile);
          lTextStream << lTextToParse;
          lTextStream.flush();

          lXMLFile.close();
        }
        else
        {
          Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lXMLPathName + tr("\"."));
          return;
        }
      }
      else
      {
        Utils::displayWarningMessage(tr("Error while trying to parse the XML Bodyslide file."));
        return;
      }
    }
    else if (lFileSuffix == "osp")
    {
      // TODO
    }

    lProgressDialog.setValue(++lTreatedFiles);
    // Update the GUI
    qApp->processEvents();
  }

  // Message when the downgrade/upgrade has been completed successfully
  auto lSuccessText{ QString("") };

  switch (lCBBE3BBBVersionSelected)
  {
  case CBBE3BBBVersion::Version1_40:
    lSuccessText = tr("All the files have been re-targeted for the version 1.40 and lower of CBBE 3BBB. You can now exit this window! :)");
    break;
  case CBBE3BBBVersion::Version1_50:
    lSuccessText = tr("All the files have been re-targeted for the version 1.50 of CBBE 3BBB. You can now exit this window! :)");
    break;
  case CBBE3BBBVersion::Version1_51:
    lSuccessText = tr("All the files have been re-targeted for the version 1.51 of CBBE 3BBB. You can now exit this window! :)");
    break;
  default:
    lSuccessText = tr("All the files have been re-targeted for the selected CBBE 3BBB version. You can now exit this window! :)");
    break;
  }

  QMessageBox lMessageBox(QMessageBox::Icon::Information, tr("Upgrade or downgarde successful"), lSuccessText);
  lMessageBox.exec();
}
