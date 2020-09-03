#include "UpgraderTool.h"

UpgraderTool::UpgraderTool(QWidget* parent)
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

void UpgraderTool::closeEvent(QCloseEvent* aEvent)
{
  auto lResult{QMessageBox::question(this, tr("Closing"), tr("Are you sure you want to close the CBBE 3BBB Version Retargeting Tool?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)};

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
  this->setWindowTitle(tr("CBBE 3BBB Version Retargeting Tool"));
}

void UpgraderTool::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QGridLayout(this)};

  this->setupInterface(*lMainVertical);
}

void UpgraderTool::setupInterface(QGridLayout& aLayout)
{
  // First line
  auto lCbbe3BBBVersionLabel{new QLabel(tr("Targeted CBBE 3BBB version:"))};
  aLayout.addWidget(lCbbe3BBBVersionLabel, 0, 0);

  auto lCbbe3BBBVersionSelector{new QComboBox()};
  lCbbe3BBBVersionSelector->addItems(Utils::getCBBE3BBBVersions());
  lCbbe3BBBVersionSelector->setCurrentIndex(static_cast<int>(mSettings.defaultRetargetingToolCBBE3BBBVersion));
  lCbbe3BBBVersionSelector->setObjectName(QString("cbbe_3bbb_version"));
  aLayout.addWidget(lCbbe3BBBVersionSelector, 0, 1, 1, 2);

  // Second line
  auto lInputPathLabel{new QLabel(tr("Input path:"))};
  aLayout.addWidget(lInputPathLabel, 1, 0);

  auto lInputPathLineEdit{new QLineEdit("")};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  aLayout.addWidget(lInputPathLineEdit, 1, 1);

  auto lInputPathChooser{new QPushButton(tr("Choose a directory..."))};
  aLayout.addWidget(lInputPathChooser, 1, 2);

  // Third line
  auto lKeepBackupLabel{new QLabel(tr("Keep a backup?"))};
  aLayout.addWidget(lKeepBackupLabel, 2, 0);

  auto lKeepBackup{new QCheckBox(tr("You should always check this box to avoid any data loss or corruption."))};
  lKeepBackup->setObjectName("keep_backup");
  aLayout.addWidget(lKeepBackup, 2, 1, 1, 2);

  // Fourth line
  auto lBackupPathLabel{new QLabel(tr("Backup directory path:"))};
  lBackupPathLabel->setObjectName("backup_path_label");
  aLayout.addWidget(lBackupPathLabel, 3, 0);

  auto lBackupPathLineEdit{new QLineEdit("")};
  lBackupPathLineEdit->setReadOnly(true);
  lBackupPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lBackupPathLineEdit->setObjectName("backup_path_directory");
  aLayout.addWidget(lBackupPathLineEdit, 3, 1);

  auto lBackupPathChooser{new QPushButton(tr("Choose a directory..."))};
  lBackupPathChooser->setObjectName("backup_dir_chooser");
  aLayout.addWidget(lBackupPathChooser, 3, 2);

  // Fifth line
  auto lLabelSubDirectoryBackupPath{new QLabel(tr("Backup subdirectory name/path:"))};
  lLabelSubDirectoryBackupPath->setObjectName("backup_subdir_label");
  aLayout.addWidget(lLabelSubDirectoryBackupPath, 4, 0);

  auto lBackupSubpathLineEdit{new QLineEdit("")};
  lBackupSubpathLineEdit->setObjectName("backup_path_subdirectory");
  aLayout.addWidget(lBackupSubpathLineEdit, 4, 1);

  // Sixth line
  auto lBackupPathPreviewLabel{new QLabel(tr("Preview:"))};
  lBackupPathPreviewLabel->setObjectName("backup_path_preview_label");
  aLayout.addWidget(lBackupPathPreviewLabel, 5, 0);

  auto lBackupPathsPreview{new QLabel("")};
  lBackupPathsPreview->setObjectName("backup_path_preview");
  aLayout.addWidget(lBackupPathsPreview, 5, 1, 1, 2);

  // Generate button
  auto lGenerateButton{new QPushButton(tr("Retarget all the files under the input path"))};
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

void UpgraderTool::refreshUI()
{
  // Set the font properties
  QFont lFont(mSettings.fontFamily, mSettings.fontSize, -1, false);
  this->setFont(lFont);
  this->setStyleSheet("font-family: \"" + mSettings.fontFamily + "\"; font-size: " + QString::number(mSettings.fontSize) + "px;");
}

void UpgraderTool::chooseInputDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};
  auto lPath{QFileDialog::getExistingDirectory(this, "", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);
}

void UpgraderTool::chooseBackupDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lPath{QFileDialog::getExistingDirectory(this, "", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);

  this->updateBackupPreview();
}

void UpgraderTool::updateBackupPreview()
{
  // Get main directory
  auto lMainDirTextEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lMainDirectory{lMainDirTextEdit->text().trimmed()};
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{this->findChild<QLineEdit*>("backup_path_subdirectory")->text().trimmed()};
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
  auto lOutputPathsPreview{this->findChild<QLabel*>("backup_path_preview")};
  lOutputPathsPreview->setText(lFullPath);
}

void UpgraderTool::switchBackupState()
{
  auto lKeepBackup{this->findChild<QCheckBox*>("keep_backup")};

  auto lBackupPathLabel{this->findChild<QLabel*>("backup_path_label")};
  auto lBackupPathLineEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lBackupPathChooser{this->findChild<QPushButton*>("backup_dir_chooser")};
  auto lLabelSubDirectoryBackupPath{this->findChild<QLabel*>("backup_subdir_label")};
  auto lBackupSubpathLineEdit{this->findChild<QLineEdit*>("backup_path_subdirectory")};
  auto lOutputPathsPreviewLabel{this->findChild<QLabel*>("backup_path_preview_label")};
  auto lOutputPathsPreview{this->findChild<QLabel*>("backup_path_preview")};

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
  auto lCBBE3BBBVersionSelected{this->findChild<QComboBox*>("cbbe_3bbb_version")->currentIndex()};
  auto lRootDir{this->findChild<QLineEdit*>("input_path_directory")->text()};

  // Check if the input path has been given by the user
  if (lRootDir.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no path path given for the retargeting."));
    return;
  }

  // Manage the backup part
  auto lMustKeepBackup{this->findChild<QCheckBox*>("keep_backup")->isChecked()};

  if (lMustKeepBackup)
  {
    // Backup paths
    auto lMainDirectory{this->findChild<QLineEdit*>("backup_path_directory")->text().trimmed()};
    auto lSubDirectory{this->findChild<QLineEdit*>("backup_path_subdirectory")->text().trimmed()};
    Utils::cleanPathString(lSubDirectory);

    // Full extract path
    auto lFullBackupDirectory{(lSubDirectory.length() == 0 ? lMainDirectory : (lMainDirectory + "/" + lSubDirectory))};

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
    auto lBackupDoneSuccessfully{Utils::copyRecursively(lRootDir, lFullBackupDirectory)};

    if (!lBackupDoneSuccessfully)
    {
      Utils::displayWarningMessage(tr("Error: the backup could not be created. Please try again."));
      return;
    }
  }

  // Scan the number of files to treat
  auto lNumberOSPFiles{Utils::getNumberFilesByExtension(lRootDir, "osp")};
  auto lNumberXMLFiles{Utils::getNumberFilesByExtension(lRootDir, "xml")};
  auto lTreatedFiles{0};

  // Progress bar
  auto lProgressbar{new QProgressBar()};
  lProgressbar->setFormat("%v / %m");
  lProgressbar->setValue(0);
  lProgressbar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog("", tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressbar);
  lProgressDialog.setWindowFlags(lProgressDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
  lProgressDialog.show();

  // Iterate through all the files
  std::vector<QPair<QString, QString>> lNamesBuffer;

  auto lAbsFilePath{QString("")};
  auto lRelativeDirs{QString("")};
  std::vector<Struct::SliderSet> lParsedSliderSets;

  auto lRessourcesFolder{QString("")};
  switch (lCBBE3BBBVersionSelected)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      lRessourcesFolder = "cbbe_3bbb_1.40";
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      lRessourcesFolder = "cbbe_3bbb_1.50";
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      lRessourcesFolder = "cbbe_3bbb_1.51";
      break;
    default:
      Utils::displayWarningMessage(tr("Error while searching for the CBBE 3BBB version. If it happens, try restarting the program. If the error is still here after restarting the program, contact the developer."));
      return;
  }

  lProgressDialog.setLabelText(tr("Parsing XML files. Please wait."));
  lProgressbar->setRange(0, lNumberXMLFiles);
  lTreatedFiles = 0;

  QDirIterator it(lRootDir, QStringList() << QString("*.xml"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by user."));
      return;
    }

    // Navigate to the next file
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    // Build the names buffer
    lNamesBuffer.push_back(QPair<QString, QString>(it.fileInfo().completeBaseName(), Utils::getPresetNameFromXMLFile(lAbsFilePath)));

    if (lNamesBuffer.at(lNamesBuffer.size() - 1).second == "")
    {
      Utils::displayWarningMessage(tr("Error while parsing the XML file \"") + lAbsFilePath + tr("\". Aborting process."));
      return;
    }

    lProgressDialog.setValue(++lTreatedFiles);
    // Update the GUI
    qApp->processEvents();
  }

  lProgressDialog.setLabelText(tr("Parsing and patching OSP files. Please wait."));
  lProgressbar->setRange(0, lNumberOSPFiles);
  lTreatedFiles = 0;

  std::vector<QPair<QString, QString>> lOSPBuffer;
  for (auto lPair : lNamesBuffer)
  {
    lOSPBuffer.push_back(lPair);
  }

  QDirIterator it2(lRootDir, QStringList() << QString("*.osp"), QDir::Files, QDirIterator::Subdirectories);
  while (it2.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by user."));
      return;
    }

    // Navigate to the next file
    it2.next();

    // Ignore FOMOD directory
    lAbsFilePath = it2.fileInfo().absoluteFilePath();

    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    auto lMustUseBeastHands{Utils::isPresetUsingBeastHands(lAbsFilePath)};

    // Check the file extension
    auto lFileName{it2.fileInfo().completeBaseName()};

    lParsedSliderSets = Utils::getOutputPathsFromOSPFile(lAbsFilePath);

    if (lParsedSliderSets.size() == 0)
    {
      Utils::displayWarningMessage(tr("Error while parsing the OSP file \"") + lAbsFilePath + tr("\". Aborting process."));
      return;
    }

    // Search for the preset name in the buffer
    auto lPresetName{QString("")};
    auto lBufferLocationToRemove{-1};

    for (int i = 0; i < lOSPBuffer.size(); i++)
    {
      if (lFileName == lOSPBuffer.at(i).first)
      {
        lPresetName = lOSPBuffer.at(i).second;
        lBufferLocationToRemove = i;
        break;
      }
    }

    if (lPresetName == "")
    {
      Utils::displayWarningMessage(tr("No data found from the associated XML file. The file ") + it2.fileInfo().absoluteFilePath() + tr(" was not modified."));
      continue;
    }

    // Remove the file once all data has been read
    if (QFile::exists(lAbsFilePath))
    {
      QFile::remove(lAbsFilePath);
    }

    // Copy the OSP file
    if (lMustUseBeastHands)
    {
      if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_osp", lAbsFilePath))
      {
        Utils::displayWarningMessage(tr("The OSP file could not be created. Did you execute the program with limited permissions?"));
        return;
      }
    }
    else
    {
      if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_osp", lAbsFilePath))
      {
        Utils::displayWarningMessage(tr("The OSP file could not be created. Did you execute the program with limited permissions?"));
        return;
      }
    }

    // Read the created OSP file
    QFile lOSPFile(lAbsFilePath);
    lOSPFile.setPermissions(QFile::WriteUser);

    QByteArray lOSPFileContent;

    if (lOSPFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      lOSPFileContent = lOSPFile.readAll();
      lOSPFile.close();
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lAbsFilePath + tr("\"."));
      return;
    }

    // Replace the custom tags in the file
    if (lOSPFileContent.length() > 0)
    {
      if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
      {
        auto lTextToParse{static_cast<QString>(lOSPFileContent)};
        lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lPresetName);

        for (auto lSliderSet : lParsedSliderSets)
        {
          if (lSliderSet.meshpart == "Body")
          {
            lTextToParse.replace(QString("{%%body_output_path%%}"), lSliderSet.outputpath);
            lTextToParse.replace(QString("{%%body_output_file%%}"), lSliderSet.outputfile);
          }
          else if (lSliderSet.meshpart == "Feet")
          {
            lTextToParse.replace(QString("{%%feet_output_path%%}"), lSliderSet.outputpath);
            lTextToParse.replace(QString("{%%feet_output_file%%}"), lSliderSet.outputfile);
          }
          else if (lSliderSet.meshpart == "Hands")
          {
            lTextToParse.replace(QString("{%%hands_output_path%%}"), lSliderSet.outputpath);
            lTextToParse.replace(QString("{%%hands_output_file%%}"), lSliderSet.outputfile);
          }
        }

        QTextStream lTextStream(&lOSPFile);
        lTextStream << lTextToParse;
        lTextStream.flush();

        lOSPFile.close();
      }
      else
      {
        Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lAbsFilePath + tr("\"."));
        return;
      }
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to parse the OSP BodySlide file."));
      return;
    }

    if (lBufferLocationToRemove != -1)
    {
      lOSPBuffer.erase(lOSPBuffer.begin() + lBufferLocationToRemove);
    }

    lProgressDialog.setValue(++lTreatedFiles);
    // Update the GUI
    qApp->processEvents();
  }

  lProgressDialog.setLabelText(tr("Patching XML files. Please wait."));
  lProgressbar->setRange(0, lNumberOSPFiles);
  lTreatedFiles = 0;

  QDirIterator it3(lRootDir, QStringList() << QString("*.xml"), QDir::Files, QDirIterator::Subdirectories);
  while (it3.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by user."));
      return;
    }

    // Navigate to the next file
    it3.next();

    // Ignore FOMOD directory
    lAbsFilePath = it3.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    auto lMustUseBeastHands{Utils::isPresetUsingBeastHands(lAbsFilePath)};

    // Check the file extension
    auto lFileName{it3.fileInfo().completeBaseName()};

    // Check if the OSP file has skiped the parsing
    for (int i = 0; i < lOSPBuffer.size(); i++)
    {
      if (lFileName == lOSPBuffer.at(i).first)
      {
        Utils::displayWarningMessage(tr("Since the associated OSP file has not been modified, the file ") + it3.fileInfo().absoluteFilePath() + tr(" has not been modified."));
        continue;
      }
    }

    // Searching for the preset name in the buffer
    auto lPresetName{QString("")};

    for (int i = 0; i < lNamesBuffer.size(); i++)
    {
      if (lFileName == lNamesBuffer.at(i).first)
      {
        lPresetName = lNamesBuffer.at(i).second;
        break;
      }
    }

    // Remove the file once all data has been read
    if (QFile::exists(lAbsFilePath))
    {
      QFile::remove(lAbsFilePath);
    }

    // Copy the XML file
    if (lMustUseBeastHands)
    {
      if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_xml", lAbsFilePath))
      {
        Utils::displayWarningMessage(tr("The XML file could not be created. Did you execute the program with limited permissions?"));
        return;
      }
    }
    else
    {
      if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_xml", lAbsFilePath))
      {
        Utils::displayWarningMessage(tr("The XML file could not be created. Did you execute the program with limited permissions?"));
        return;
      }
    }

    // Read the created XML file
    QFile lXMLFile(lAbsFilePath);
    lXMLFile.setPermissions(QFile::WriteUser);

    QByteArray lXMLFileContent;

    if (lXMLFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      lXMLFileContent = lXMLFile.readAll();
      lXMLFile.close();
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lAbsFilePath + tr("\"."));
      return;
    }

    // Replace the custom tags in the file
    if (lXMLFileContent.length() > 0)
    {
      if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
      {
        auto lTextToParse{static_cast<QString>(lXMLFileContent)};
        lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lPresetName);

        QTextStream lTextStream(&lXMLFile);
        lTextStream << lTextToParse;
        lTextStream.flush();

        lXMLFile.close();
      }
      else
      {
        Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lAbsFilePath + tr("\"."));
        return;
      }
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to parse the XML BodySlide file."));
      return;
    }

    lProgressDialog.setValue(++lTreatedFiles);
    // Update the GUI
    qApp->processEvents();
  }

  // Message when the downgrade/upgrade has been completed successfully
  auto lSuccessText{QString("")};

  switch (lCBBE3BBBVersionSelected)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      lSuccessText = tr("All the files have been re-targeted for the version 1.40 and lower of CBBE 3BBB. You can now exit this window! :)");
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      lSuccessText = tr("All the files have been re-targeted for the version 1.50 of CBBE 3BBB. You can now exit this window! :)");
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      lSuccessText = tr("All the files have been re-targeted for the version 1.51 and 1.52 of CBBE 3BBB. You can now exit this window! :)");
      break;
    default:
      lSuccessText = tr("All the files have been re-targeted for the selected CBBE 3BBB version. You can now exit this window! :)");
      break;
  }

  QMessageBox::information(this, tr("Upgrade or downgarde successful"), lSuccessText, QMessageBox::Ok, QMessageBox::Ok);
}
