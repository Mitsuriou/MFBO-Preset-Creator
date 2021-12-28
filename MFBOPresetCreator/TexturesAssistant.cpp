#include "TexturesAssistant.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Utils.h"
#include <QAbstractSlider>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QScrollArea>
#include <QStyledItemDelegate>

TexturesAssistant::TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mHasUserDoneSomething(false)
  , mMinimumFirstColumnWidth(300)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.texturesAssistantDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void TexturesAssistant::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the textures assistant window"),
                                    "",
                                    this->mSettings.display.dangerColor,
                                    this->mSettings.display.successColor,
                                    "",
                                    false)
      == ButtonClicked::YES)
  {
    aEvent->accept();
  }
  else
  {
    aEvent->ignore();
  }
}

void TexturesAssistant::reject()
{
  this->close();
}

void TexturesAssistant::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Textures Assistant"));
  this->setWindowIcon(QIcon(QPixmap(":/black/textures")));
}

void TexturesAssistant::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  // Main window layout
  auto lMainGrid{new QGridLayout(this)};
  lMainGrid->setSpacing(10);
  lMainGrid->setContentsMargins(10, 10, 10, 10);
  lMainGrid->setAlignment(Qt::AlignTop);
  lMainGrid->setRowStretch(2, 1);
  this->setLayout(lMainGrid);

  // First line
  lMainGrid->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit(this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QString("input_path_directory"));
  lInputPathLineEdit->setDisabled(true);
  lMainGrid->addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "", false, true)};
  lMainGrid->addWidget(lInputPathChooser, 0, 2);

  // Launch search button
  auto lLaunchSearchButton{ComponentFactory::CreateButton(this, tr("Launch the scan of the mod"), "", "search", lIconFolder, "launch_search_button", true, true)};
  lMainGrid->addWidget(lLaunchSearchButton, 1, 0, 1, 3, Qt::AlignTop);

  // Setup all the different GUI components
  this->displayHintZone();
  this->setupTexturesSetGUI(*lMainGrid);
  this->setupOutputBox(*lMainGrid);
  this->setupButtons(*lMainGrid);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &TexturesAssistant::chooseInputDirectory);
  this->connect(lLaunchSearchButton, &QPushButton::clicked, this, &TexturesAssistant::launchSearchProcess);
}

void TexturesAssistant::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  // Get the window's layout
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setObjectName(QString("hint_zone"));
  lHintZone->setAlignment(Qt::AlignCenter);
  qobject_cast<QGridLayout*>(this->layout())->addWidget(lHintZone, 2, 0, 1, 3);

  // Disable the groupboxes
  auto lTexturesSetGroupBox{this->findChild<GroupBox*>(QString("textures_set_groupbox"))};
  if (lTexturesSetGroupBox != nullptr)
    lTexturesSetGroupBox->setDisabled(true);

  auto lOutputGroupBox{this->findChild<GroupBox*>(QString("output_group_box"))};
  if (lOutputGroupBox != nullptr)
    lOutputGroupBox->setDisabled(true);

  auto lGenerateButton{this->findChild<QPushButton*>(QString("generate_set"))};
  if (lGenerateButton != nullptr)
    lGenerateButton->setDisabled(true);
}

void TexturesAssistant::setupTexturesSetGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  // Group box
  auto lGroupBox{ComponentFactory::CreateGroupBox(this, tr("Textures set"), "textures", lIconFolder, this->mSettings.display.font.size, "textures_set_groupbox", true)};
  aLayout.addWidget(lGroupBox, 3, 0, 1, 3);

  auto lLayout{new QGridLayout(lGroupBox)};
  lLayout->setColumnStretch(0, 0);
  lLayout->setColumnStretch(1, 1);
  lLayout->setColumnStretch(2, 0);
  lLayout->setSpacing(10);
  lLayout->setContentsMargins(15, 20, 15, 15);
  lLayout->setAlignment(Qt::AlignTop);
  lLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Human skeleton file
  lLayout->addWidget(new QLabel(tr("Textures set:"), this), 0, 0);

  auto lTexturesSetChooser{new QComboBox(this)};
  lTexturesSetChooser->setItemDelegate(new QStyledItemDelegate());
  lTexturesSetChooser->setCursor(Qt::PointingHandCursor);
  lTexturesSetChooser->setObjectName(QString("textures_set_chooser"));
  lLayout->addWidget(lTexturesSetChooser, 0, 1);

  // Refresh button
  auto lTexturesSetRefresher{ComponentFactory::CreateButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lLayout->addWidget(lTexturesSetRefresher, 0, 2);

  // Open assets directory
  auto lOpenAssetsDirectory{ComponentFactory::CreateButton(this, "View in explorer", "", "folder-search", lIconFolder)};
  lLayout->addWidget(lOpenAssetsDirectory, 0, 3);

  this->populateTexturesSetChooser();

  // Event binding
  this->connect(lTexturesSetRefresher, &QPushButton::clicked, this, &TexturesAssistant::populateTexturesSetChooser);
  this->connect(lOpenAssetsDirectory, &QPushButton::clicked, this, &TexturesAssistant::openTexturesSetsAssetsDirectory);
}

void TexturesAssistant::setupOutputBox(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  // Create the group box
  ComponentFactory::CreateOutputBox(this, aLayout, 4, 0, lIconFolder, this->mMinimumFirstColumnWidth, this->mSettings.display.font.size, 1, 3);
  auto lOutputGroupBox{this->findChild<GroupBox*>(QString("output_group_box"))};
  lOutputGroupBox->setDisabled(true);

  // Event binding
  auto lOutputPathChooser{this->findChild<QPushButton*>(QString("output_path_chooser"))};
  this->connect(lOutputPathChooser, &QPushButton::clicked, this, &TexturesAssistant::chooseExportDirectory);

  auto lOutputSubpathLineEdit{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))};
  this->connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &TexturesAssistant::updateOutputPreview);

  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))};
  this->connect(lUseOnlySubdir, &QCheckBox::stateChanged, this, &TexturesAssistant::useOnlySubdirStateChanged);

  // Pre-filled data
  this->updateOutputPreview();
}

void TexturesAssistant::setupButtons(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  // Generate button
  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Create the files structure on my computer"), "", "build", lIconFolder, "generate_set")};
  lGenerateButton->setDisabled(true);
  aLayout.addWidget(lGenerateButton, 5, 0, 1, 3);

  // Event binding
  this->connect(lGenerateButton, &QPushButton::clicked, this, &TexturesAssistant::generateTexturesStructure);
}

void TexturesAssistant::deleteAlreadyExistingWindowBottom() const
{
  auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
  if (lHintZone != nullptr)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lScrollZone{this->findChild<QScrollArea*>(QString("scrollable_zone"))};
  if (lScrollZone != nullptr)
  {
    delete lScrollZone;
    lScrollZone = nullptr;
  }
}

void TexturesAssistant::scanForTexturesFiles(const QString& aRootDir, const QString& aFileExtension)
{
  // Progress bar
  auto lProgressBar{new QProgressBar(this->parentWidget())};
  lProgressBar->setFormat("");
  lProgressBar->setMinimum(0);
  lProgressBar->setMaximum(0);
  lProgressBar->setValue(0);
  lProgressBar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Scanning the directory. Please wait..."), tr("Cancel treatment"), 0, 0, this->parentWidget());
  lProgressDialog.setBar(lProgressBar);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  qApp->processEvents();

  this->mScannedFiles.groupedTextures.clear();
  this->mScannedFiles.otherTextures.clear();

  const QStringList lTexturesFilesToFind{DataLists::GetKnownTexturesFilesNames()};

  // Suffix the root directory to earch only in the "textures" subfolder
  const auto lRootDir{QString("%1/textures").arg(aRootDir)};

  QDirIterator it(lRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    it.next();

    // Get the current directory
    const auto lRelativeDirPath{it.fileInfo().absolutePath().remove(aRootDir + "/", Qt::CaseInsensitive)};

    const auto lFileName = it.fileInfo().fileName();

    // Check if the file is relative to a texture for a known mesh type
    if (lTexturesFilesToFind.contains(lFileName))
    {
      this->mScannedFiles.groupedTextures.push_back(std::make_pair(lRelativeDirPath, lFileName));
    }
    else
    {
      this->mScannedFiles.otherTextures.push_back(std::make_pair(lRelativeDirPath, lFileName));
    }
  }
}

void TexturesAssistant::displayTexturesFilesList()
{
  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area wrapper
  auto lDataContainer{ComponentFactory::CreateScrollAreaComponentLayout(this)};
  qobject_cast<QGridLayout*>(this->layout())->addLayout(lDataContainer, 2, 0, 1, 3);

  // Parse the grouped textures to split them in multiple storages
  TexturesAssistant::GroupedData lGroupedPaths;

  for (const auto& lNifFile : this->mScannedFiles.groupedTextures)
  {
    std::map<std::string, std::vector<QString>>* lTargetMapToFill{nullptr};

    if (lNifFile.second.contains("head"))
    {
      lTargetMapToFill = &lGroupedPaths.headTextures;
    }
    else if (lNifFile.second.contains("hands"))
    {
      lTargetMapToFill = &lGroupedPaths.handsTextures;
    }
    else if (lNifFile.second.contains("body_etc"))
    {
      lTargetMapToFill = &lGroupedPaths.extraBodyTextures;
    }
    else if (lNifFile.second.contains("body"))
    {
      lTargetMapToFill = &lGroupedPaths.bodyTextures;
    }
    else if (lNifFile.second.contains("mouth"))
    {
      lTargetMapToFill = &lGroupedPaths.mouthTextures;
    }

    if (lTargetMapToFill != nullptr)
    {
      (*lTargetMapToFill)[lNifFile.first.toStdString()].push_back(lNifFile.second);
    }
  }

  // Parse the other textures to change their storage type to be compatible with the display function
  auto lOtherPaths{std::map<std::string, std::vector<QString>>()};

  for (const auto& lNifFile : this->mScannedFiles.otherTextures)
  {
    lOtherPaths[lNifFile.first.toStdString()].push_back(lNifFile.second);
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};
  auto lRowIndex{0};

  // Head ressources blocks
  auto lHeadGroup{ComponentFactory::CreateGroupBox(this, tr("Head textures"), "woman-head", lIconFolder, this->mSettings.display.font.size)};

  auto lHeadGroupContainer{new QGridLayout(this)};
  lHeadGroupContainer->setSpacing(16);
  lHeadGroup->setLayout(lHeadGroupContainer);
  this->createRessourceBlock(lGroupedPaths.headTextures, lHeadGroupContainer);
  lDataContainer->addWidget(lHeadGroup, lRowIndex++, 0);

  // Mouth ressources blocks
  auto lMouthGroup{ComponentFactory::CreateGroupBox(this, tr("Mouth textures"), "mouth", lIconFolder, this->mSettings.display.font.size)};

  auto lMouthGroupContainer{new QGridLayout(this)};
  lMouthGroupContainer->setSpacing(16);
  lMouthGroup->setLayout(lMouthGroupContainer);
  this->createRessourceBlock(lGroupedPaths.mouthTextures, lMouthGroupContainer);
  lDataContainer->addWidget(lMouthGroup, lRowIndex++, 0);

  // Body ressources blocks
  auto lBodyGroup{ComponentFactory::CreateGroupBox(this, tr("Body textures"), "body", lIconFolder, this->mSettings.display.font.size)};

  auto lBodyGroupContainer{new QGridLayout(this)};
  lBodyGroupContainer->setSpacing(16);
  lBodyGroup->setLayout(lBodyGroupContainer);
  this->createRessourceBlock(lGroupedPaths.bodyTextures, lBodyGroupContainer);
  lDataContainer->addWidget(lBodyGroup, lRowIndex++, 0);

  // Extra body ressources blocks
  auto lBodyExtraGroup{ComponentFactory::CreateGroupBox(this, tr("Extra body textures"), "more", lIconFolder, this->mSettings.display.font.size)};

  auto lBodyExtraGroupContainer{new QGridLayout(this)};
  lBodyExtraGroupContainer->setSpacing(16);
  lBodyExtraGroup->setLayout(lBodyExtraGroupContainer);
  this->createRessourceBlock(lGroupedPaths.extraBodyTextures, lBodyExtraGroupContainer);
  lDataContainer->addWidget(lBodyExtraGroup, lRowIndex++, 0);

  // Hands ressources blocks
  auto lHandsGroup{ComponentFactory::CreateGroupBox(this, tr("Hands textures"), "hand", lIconFolder, this->mSettings.display.font.size)};

  auto lHandsGroupContainer{new QGridLayout(this)};
  lHandsGroupContainer->setSpacing(16);
  lHandsGroup->setLayout(lHandsGroupContainer);
  this->createRessourceBlock(lGroupedPaths.handsTextures, lHandsGroupContainer);
  lDataContainer->addWidget(lHandsGroup, lRowIndex++, 0);

  // Other texture files
  auto lOtherGroup{ComponentFactory::CreateGroupBox(this, tr("Other .DDS textures"), "textures", lIconFolder, this->mSettings.display.font.size)};

  auto lOtherGroupContainer{new QGridLayout(this)};
  lOtherGroupContainer->setSpacing(16);
  lOtherGroup->setLayout(lOtherGroupContainer);
  this->createRessourceBlock(lOtherPaths, lOtherGroupContainer);
  lDataContainer->addWidget(lOtherGroup, lRowIndex++, 0);
}

void TexturesAssistant::createRessourceBlock(const std::map<std::string, std::vector<QString>>& aMap, QGridLayout* aLayout)
{
  auto lRowIndex{0};
  for (const auto& lRootPath : aMap)
  {
    auto lConcatenatedFileNames{QString()};

    for (const auto& lFileName : lRootPath.second)
    {
      lConcatenatedFileNames.append(QString("%1\n").arg(lFileName));
    }
    lConcatenatedFileNames = lConcatenatedFileNames.left(lConcatenatedFileNames.length() - 1);

    aLayout->addWidget(new QLabel(QString::fromStdString(lRootPath.first), this), lRowIndex, 0, Qt::AlignLeft);
    aLayout->addWidget(new QLabel(lConcatenatedFileNames, this), lRowIndex++, 1, Qt::AlignLeft);
  }
}

void TexturesAssistant::updateOutputPreview()
{
  auto lMainDirTextEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("output_path_preview"))};

  Utils::UpdateOutputPreview(lMainDirTextEdit, lSubDirectory, lUseOnlySubdir, this->mSettings.display.successColor, this->mSettings.display.warningColor, this->mSettings.display.dangerColor, lOutputPathsPreview);
}

void TexturesAssistant::generateTexturesStructure()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  // Textures set
  auto lTexturesSetChooser{this->findChild<QComboBox*>(QString("textures_set_chooser"))};
  if (lTexturesSetChooser->currentIndex() == -1 || lTexturesSetChooser->count() == 0)
  {
    Utils::DisplayWarningMessage(tr("Error: no textures set chosen."));
    return;
  }

  // Output paths
  auto lMainDirectory{this->findChild<QLineEdit*>(QString("output_path_directory"))->text().trimmed()};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  Utils::CleanPathString(lSubDirectory);

  // Does the user want to define the path only through the secondary path?
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};

  // Full extract path
  QString lEntryDirectory;
  if (lUseOnlySubdir)
    lEntryDirectory = lSubDirectory;
  else if (!lMainDirectory.isEmpty())
    lEntryDirectory = (lSubDirectory.isEmpty() ? lMainDirectory : (lMainDirectory + "/" + lSubDirectory));

  // Check if the full extract path has been given by the user
  if (lEntryDirectory.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no path given to export the files."));
    return;
  }

  // Check if the path could be valid
  if (lEntryDirectory.startsWith('/') || lEntryDirectory.startsWith('\\'))
  {
    Utils::DisplayWarningMessage(tr("Error: the path given to export the files seems to be invalid."));
    return;
  }

  // Create main directory
  if (!QDir(lEntryDirectory).exists())
  {
    // Wait to know the result of the mkdir()
    if (!QDir().mkpath(lEntryDirectory))
    {
      Utils::DisplayWarningMessage(tr("Error while creating the main directory: \"%1\" could not be created on your computer.\nBe sure to not create the files in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths.").arg(lEntryDirectory));
      return;
    }
  }
  else
  {
    // Since the directory already exist, ask the user to put other files in it
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Already existing directory"),
                                      tr("The directory \"%1\" already exists on your computer. Do you still want to create the files in this directory?").arg(lEntryDirectory),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the files creation"),
                                      tr("Cancel the files creation"),
                                      "",
                                      this->mSettings.display.dangerColor,
                                      this->mSettings.display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  // Read location
  auto lSourceTexturesFolderName{lTexturesSetChooser->currentText()};
  auto lSourceTexturesReadPath{QString("%1assets/textures/%2").arg(Utils::GetAppDataPathFolder(), lSourceTexturesFolderName)};

  // Iterate through the texture files lists
  for (const auto& lFoundTextureFile : this->mScannedFiles.groupedTextures)
  {
    Utils::CreateTextureFile(lSourceTexturesReadPath, lEntryDirectory, lFoundTextureFile.first, lFoundTextureFile.second);
  }

  // Update the color of the output directory preview
  this->updateOutputPreview();

  auto lTitle{tr("Files creation successful")};
  auto lMessage{tr("The texture files have been correctly created.")};

  // Open the directory where the file structure has been created
  if (Utils::DisplayQuestionMessage(this, lTitle, lMessage, "icons", "green-info", tr("Open the created directory"), tr("OK"), "", "", "", "", false) == ButtonClicked::YES)
  {
    QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
  }
}

void TexturesAssistant::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "texturesAssistantInput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "texturesAssistantInput", lPath);

  if (!this->mHasUserDoneSomething && lPath.compare("") != 0)
  {
    this->mHasUserDoneSomething = true;
  }

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.compare("", Qt::CaseInsensitive) == 0};
  lLineEdit->setDisabled(lMustDisableButton);
  lLaunchSearchButton->setDisabled(lMustDisableButton);

  this->displayHintZone();
}

void TexturesAssistant::launchSearchProcess()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  const auto& lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

  // Warn the user if the scan found a BSA file
  if (Utils::GetNumberFilesByExtensions(lInputPath, QStringList("*.bsa")) > 0)
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("BSA file found"),
                                      tr("At least one BSA file was found in the scanned directory. Please note that the application cannot read the data contained in the BSA files, so it is advisable to decompress the BSA file before continuing the scan. Do you still want to continue the scan?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the scan"),
                                      tr("Cancel the scan"),
                                      "",
                                      this->mSettings.display.warningColor,
                                      this->mSettings.display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  // The root directory should at least contain a DDS file to be scanned.
  if (!QDir(QString("%1/textures").arg(lInputPath)).exists())
  {
    Utils::DisplayWarningMessage(tr("No \"textures\" directory has been found in the scanned directory."));
  }

  // Fetch all the "*dds" files
  this->scanForTexturesFiles(lInputPath, "*.dds");

  // No file found
  if (this->mScannedFiles.groupedTextures.size() == 0 && this->mScannedFiles.otherTextures.size() == 0)
  {
    this->displayHintZone();

    auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
    if (lHintZone != nullptr)
    {
      lHintZone->setText(tr("No DDS file was found in the scanned directory."));
    }
    return;
  }

  // Display the data
  this->displayTexturesFilesList();

  // Re-enable the groupboxes
  auto lTexturesSetGroupBox{this->findChild<GroupBox*>(QString("textures_set_groupbox"))};
  if (lTexturesSetGroupBox != nullptr)
    lTexturesSetGroupBox->setDisabled(false);

  auto lOutputGroupBox{this->findChild<GroupBox*>(QString("output_group_box"))};
  if (lOutputGroupBox != nullptr)
    lOutputGroupBox->setDisabled(false);

  auto lGenerateButton{this->findChild<QPushButton*>(QString("generate_set"))};
  if (lGenerateButton != nullptr)
    lGenerateButton->setDisabled(false);
}

void TexturesAssistant::populateTexturesSetChooser()
{
  auto lRootDir{Utils::GetAppDataPathFolder() + "assets/textures/"};
  Utils::CleanPathString(lRootDir);
  QStringList lAvailableTexturesSets;

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList(), QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
  while (it.hasNext())
  {
    it.next();
    lAvailableTexturesSets.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
  }

  // Clear the combo box and add the found files to it
  auto lTexturesSetChooser{this->findChild<QComboBox*>(QString("textures_set_chooser"))};
  lTexturesSetChooser->clear();
  lTexturesSetChooser->addItems(lAvailableTexturesSets);
}

void TexturesAssistant::openTexturesSetsAssetsDirectory()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(QString("%1assets/textures").arg(Utils::GetAppDataPathFolder())));
}

void TexturesAssistant::useOnlySubdirStateChanged(int)
{
  this->updateOutputPreview();
}

void TexturesAssistant::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "texturesAssistantOutput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "texturesAssistantOutput", lPath);
  this->updateOutputPreview();
}
