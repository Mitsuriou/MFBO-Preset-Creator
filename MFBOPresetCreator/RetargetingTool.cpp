#include "RetargetingTool.h"

RetargetingTool::RetargetingTool(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void RetargetingTool::closeEvent(QCloseEvent* aEvent)
{
  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Closing"), tr("Do you want to close the window?"), QMessageBox::StandardButton::NoButton, this);

  auto lCloseButton{lConfirmationBox.addButton(tr("Close the window"), QMessageBox::ButtonRole::YesRole)};
  lCloseButton->setCursor(Qt::PointingHandCursor);
  lCloseButton->setStyleSheet("color: hsl(4, 90%, 58%);");

  auto lStayButton{lConfirmationBox.addButton(tr("Go back to the retargeting tool window"), QMessageBox::ButtonRole::NoRole)};
  lStayButton->setCursor(Qt::PointingHandCursor);
  lStayButton->setStyleSheet("color: hsl(141, 53%, 53%)");

  lConfirmationBox.setDefaultButton(lStayButton);
  lConfirmationBox.exec();

  if (lConfirmationBox.clickedButton() != lCloseButton)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
  }
}

void RetargetingTool::reject()
{
  this->close();
}

void RetargetingTool::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("CBBE 3BBB 3BA BodySlide Presets' Version Retargeting Tool"));
  this->setWindowIcon(QIcon(QPixmap(":/black/arrow_up")));
}

void RetargetingTool::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QGridLayout(this)};
  this->setupInterface(*lMainVertical);
}

void RetargetingTool::setupInterface(QGridLayout& aLayout)
{
  // First line
  auto lCbbe3BBBVersionLabel{new QLabel(tr("Targeted CBBE 3BBB 3BA version:"), this)};
  aLayout.addWidget(lCbbe3BBBVersionLabel, 0, 0);

  auto lCbbe3BBBVersionSelector{new QComboBox(this)};
  lCbbe3BBBVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lCbbe3BBBVersionSelector->setCursor(Qt::PointingHandCursor);
  lCbbe3BBBVersionSelector->addItems(DataLists::getBodiesNameVersions());
  lCbbe3BBBVersionSelector->setCurrentIndex(static_cast<int>(mSettings.defaultRetargetingToolBody));
  lCbbe3BBBVersionSelector->setObjectName(QString("body_selector"));
  aLayout.addWidget(lCbbe3BBBVersionSelector, 0, 1, 1, 2);

  // Second line
  auto lInputPathLabel{new QLabel(tr("Input path:"), this)};
  aLayout.addWidget(lInputPathLabel, 1, 0);

  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  aLayout.addWidget(lInputPathLineEdit, 1, 1);

  auto lInputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lInputPathChooser->setCursor(Qt::PointingHandCursor);
  lInputPathChooser->setAutoDefault(false);
  lInputPathChooser->setDefault(false);
  aLayout.addWidget(lInputPathChooser, 1, 2);

  // Third line
  auto lKeepBackupLabel{new QLabel(tr("Keep a backup?"), this)};
  aLayout.addWidget(lKeepBackupLabel, 2, 0);

  auto lKeepBackup{new QCheckBox(tr("You should always check this box to avoid any data loss or corruption."), this)};
  lKeepBackup->setCursor(Qt::PointingHandCursor);
  lKeepBackup->setObjectName("keep_backup");
  aLayout.addWidget(lKeepBackup, 2, 1, 1, 2);

  // Fourth line
  auto lBackupPathLabel{new QLabel(tr("Backup directory path:"), this)};
  lBackupPathLabel->setObjectName("backup_path_label");
  aLayout.addWidget(lBackupPathLabel, 3, 0);

  auto lBackupPathLineEdit{new QLineEdit("", this)};
  lBackupPathLineEdit->setReadOnly(true);
  lBackupPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lBackupPathLineEdit->setObjectName("backup_path_directory");
  aLayout.addWidget(lBackupPathLineEdit, 3, 1);

  auto lBackupPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lBackupPathChooser->setCursor(Qt::PointingHandCursor);
  lBackupPathChooser->setAutoDefault(false);
  lBackupPathChooser->setDefault(false);
  lBackupPathChooser->setObjectName("backup_dir_chooser");
  aLayout.addWidget(lBackupPathChooser, 3, 2);

  // Fifth line
  auto lLabelSubDirectoryBackupPath{new QLabel(tr("Backup subdirectory name/path:"), this)};
  lLabelSubDirectoryBackupPath->setObjectName("backup_subdir_label");
  aLayout.addWidget(lLabelSubDirectoryBackupPath, 4, 0);

  auto lBackupSubpathLineEdit{new QLineEdit("", this)};
  lBackupSubpathLineEdit->setObjectName("backup_path_subdirectory");
  aLayout.addWidget(lBackupSubpathLineEdit, 4, 1);

  // Sixth line
  auto lBackupPathPreviewLabel{new QLabel(tr("Preview:"), this)};
  lBackupPathPreviewLabel->setObjectName("backup_path_preview_label");
  lBackupPathPreviewLabel->setAlignment(Qt::AlignTop);
  aLayout.addWidget(lBackupPathPreviewLabel, 5, 0);

  auto lBackupPathsPreview{new QLabel("", this)};
  lBackupPathsPreview->setObjectName("backup_path_preview");
  lBackupPathsPreview->setAlignment(Qt::AlignTop);
  aLayout.addWidget(lBackupPathsPreview, 5, 1, 1, 2);

  // Generate button
  auto lGenerateButton{new QPushButton(tr("Retarget all the files under the input path"), this)};
  lGenerateButton->setCursor(Qt::PointingHandCursor);
  lGenerateButton->setAutoDefault(false);
  lGenerateButton->setDefault(false);
  aLayout.addWidget(lGenerateButton, 6, 0, 1, 3, Qt::AlignBottom);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseInputDirectory);
  this->connect(lKeepBackup, &QCheckBox::stateChanged, this, &RetargetingTool::updateBackupState);
  lKeepBackup->setChecked(true);

  this->connect(lBackupPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseBackupDirectory);
  this->connect(lBackupSubpathLineEdit, &QLineEdit::textChanged, this, &RetargetingTool::updateBackupPreview);
  this->connect(lGenerateButton, &QPushButton::clicked, this, &RetargetingTool::launchUpDownGradeProcess);
}

int RetargetingTool::getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension)
{
  auto lNumber{0};
  auto lAbsFilePath{QString("")};
  auto lRelativeDirs{QString("")};

  QDirIterator it(aRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.remove(aRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    lNumber++;
  }

  return lNumber;
}

void RetargetingTool::chooseInputDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lLineEdit->text().size() > 0 ? lLineEdit->text() : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);
}

void RetargetingTool::chooseBackupDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lLineEdit->text().size() > 0 ? lLineEdit->text() : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);

  this->updateBackupPreview();
}

void RetargetingTool::updateBackupState(int aState)
{
  auto lBackupPathLabel{this->findChild<QLabel*>("backup_path_label")};
  auto lBackupPathLineEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lBackupPathChooser{this->findChild<QPushButton*>("backup_dir_chooser")};
  auto lLabelSubDirectoryBackupPath{this->findChild<QLabel*>("backup_subdir_label")};
  auto lBackupSubpathLineEdit{this->findChild<QLineEdit*>("backup_path_subdirectory")};
  auto lOutputPathsPreviewLabel{this->findChild<QLabel*>("backup_path_preview_label")};
  auto lOutputPathsPreview{this->findChild<QLabel*>("backup_path_preview")};

  switch (aState)
  {
    case Qt::Unchecked:
      lOutputPathsPreview->setStyleSheet("");

      lBackupPathLabel->setDisabled(true);
      lBackupPathLineEdit->setDisabled(true);
      lBackupPathChooser->setDisabled(true);
      lLabelSubDirectoryBackupPath->setDisabled(true);
      lBackupSubpathLineEdit->setDisabled(true);
      lOutputPathsPreviewLabel->setDisabled(true);
      lOutputPathsPreview->setDisabled(true);
      break;
    case Qt::Checked:
      lBackupPathLabel->setDisabled(false);
      lBackupPathLineEdit->setDisabled(false);
      lBackupPathChooser->setDisabled(false);
      lLabelSubDirectoryBackupPath->setDisabled(false);
      lBackupSubpathLineEdit->setDisabled(false);
      lOutputPathsPreviewLabel->setDisabled(false);
      lOutputPathsPreview->setDisabled(false);

      this->updateBackupPreview();
  }
}

void RetargetingTool::updateBackupPreview()
{
  // Get main directory
  auto lMainDirTextEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lMainDirectory{lMainDirTextEdit->text().trimmed()};
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{this->findChild<QLineEdit*>("backup_path_subdirectory")->text().trimmed()};
  Utils::cleanPathString(lSubDirectory);
  auto lIsValidPath{true};

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
    lIsValidPath = false;
  }

  // Set the full path value in the preview label
  auto lOutputPathsPreview{this->findChild<QLabel*>("backup_path_preview")};

  auto lNewTextColor{QString("hsl(141, 53%, 53%)")};

  if (lIsValidPath)
  {
    // Check if the user is trying to backup the directory into itself
    if (lFullPath.startsWith(this->findChild<QLineEdit*>("input_path_directory")->text()))
    {
      lNewTextColor = "hsl(4, 90%, 58%)";
    }
    // Check if the wanted backup directory already exists
    else if (QDir(lFullPath).exists())
    {
      lNewTextColor = "hsl(33, 100%, 71%)";
    }
  }
  else
  {
    lNewTextColor = "hsl(4, 90%, 58%)";
  }

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lFullPath);
}

void RetargetingTool::launchUpDownGradeProcess()
{
  auto lBodySelected{this->findChild<QComboBox*>("body_selector")->currentIndex()};
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
    const auto& lDirtyFullBackupDirectory{(lSubDirectory.length() == 0 ? lMainDirectory : (lMainDirectory + QDir::separator() + lSubDirectory))};
    auto lFullBackupDirectory{Utils::cleanPathString(lDirtyFullBackupDirectory)};

    // Check if the full extract path has been given by the user
    if (lFullBackupDirectory.length() == 0)
    {
      Utils::displayWarningMessage(tr("Error: no path given to backup the files."));
      return;
    }

    // Check if the user is trying to backup the directory into itself
    auto lInputPath{this->findChild<QLineEdit*>("input_path_directory")->text()};

    if (lFullBackupDirectory.startsWith(lInputPath))
    {
      Utils::displayWarningMessage(tr("Error: it is not possible to backup a directory inside itself. Choose another backup location."));
      return;
    }

    // Check if the path could be valid
    if (lFullBackupDirectory.startsWith("/"))
    {
      Utils::displayWarningMessage(tr("Error: the path given to backup the files seems to be invalid."));
      return;
    }

    // Copy the directory and its content
    if (!Utils::copyRecursively(lRootDir, lFullBackupDirectory))
    {
      Utils::displayWarningMessage(tr("Error: the backup could not be created. Please try again."));
      return;
    }
  }

  // Scan the number of files to treat
  auto lNumberOSPFiles{this->getNumberFilesByExtension(lRootDir, "*.osp")};
  auto lNumberXMLFiles{this->getNumberFilesByExtension(lRootDir, "*.xml")};
  auto lTreatedFiles{0};

  // Progress bar
  auto lProgressbar{new QProgressBar(this)};
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
  auto lRessourcesFolder{Utils::getBodyRessourceFolder(static_cast<BodyNameVersion>(lBodySelected))};
  if (lRessourcesFolder.length() == 0)
  {
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
  for (const auto& lPair : lNamesBuffer)
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

    auto lOSPBufSize{lOSPBuffer.size()};
    for (int i = 0; i < lOSPBufSize; i++)
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
        Utils::displayWarningMessage(tr("The OSP file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
        return;
      }
    }
    else
    {
      if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_osp", lAbsFilePath))
      {
        Utils::displayWarningMessage(tr("The OSP file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
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

        for (const auto& lSliderSet : lParsedSliderSets)
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
    auto lOSPBufSize{lOSPBuffer.size()};
    for (int i = 0; i < lOSPBufSize; i++)
    {
      if (lFileName == lOSPBuffer.at(i).first)
      {
        Utils::displayWarningMessage(tr("Since the associated OSP file has not been modified, the file ") + it3.fileInfo().absoluteFilePath() + tr(" has not been modified."));
        continue;
      }
    }

    // Searching for the preset name in the buffer
    auto lPresetName{QString("")};
    auto lNamesBufSize{lNamesBuffer.size()};

    for (int i = 0; i < lNamesBufSize; i++)
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
        Utils::displayWarningMessage(tr("The XML file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
        return;
      }
    }
    else
    {
      if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_xml", lAbsFilePath))
      {
        Utils::displayWarningMessage(tr("The XML file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
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

  switch (lBodySelected)
  {
    case static_cast<int>(BodyNameVersion::CBBE_3BBB_3BA_1_40):
      lSuccessText = tr("All the files have been re-targeted for the version 1.40 and lower of CBBE 3BBB. You can now exit this window! :)");
      break;
    case static_cast<int>(BodyNameVersion::CBBE_3BBB_3BA_1_50):
      lSuccessText = tr("All the files have been re-targeted for the version 1.50 of CBBE 3BBB. You can now exit this window! :)");
      break;
    case static_cast<int>(BodyNameVersion::CBBE_3BBB_3BA_1_51_and_1_52):
      lSuccessText = tr("All the files have been re-targeted for the version 1.51 and 1.52 of CBBE 3BBB. You can now exit this window! :)");
      break;
    default:
      lSuccessText = tr("All the files have been re-targeted for the selected CBBE 3BBB version. You can now exit this window! :)");
      break;
  }

  QMessageBox lConfirmationBox(QMessageBox::Icon::Information, tr("Upgrade or downgarde successful"), lSuccessText, QMessageBox::StandardButton::NoButton, this);

  auto lOKButton{lConfirmationBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
  lOKButton->setCursor(Qt::PointingHandCursor);
  lConfirmationBox.setDefaultButton(lOKButton);
  lConfirmationBox.exec();
}
