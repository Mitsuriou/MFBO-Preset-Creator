#include "BatchConversion.h"
#include "BatchConversionPicker.h"
#include "BodySlideFiltersEditor.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include "TargetMeshesPicker.h"
#include "Utils.h"
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QProgressBar>
#include <QProgressDialog>
#include <QRadioButton>
#include <QScrollArea>
#include <QStyledItemDelegate>

BatchConversion::BatchConversion(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mFileWatcher(new QFileSystemWatcher())
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mMinimumFirstColumnWidth(300)
  , mTargetBodyMesh(aSettings.batchConversion.defaultBodyFeet.bodyMesh)
  , mTargetFeetMesh(aSettings.batchConversion.defaultBodyFeet.feetMesh)
{
  // Build the window's interface
  this->setWindowProperties();

  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this)};
  auto lButtonLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  // Setup all the different GUI components
  this->setupGeneralGUI(*lMainLayout);
  this->setupSkeletonGUI(*lMainLayout);
  this->setupBodySlideGUI(*lMainLayout);
  this->setupOutputGUI(*lMainLayout);
  this->setupScanTweaksGUI(*lMainLayout);
  this->setupGenerationAdjustmentGUI(*lMainLayout);
  this->setupButtons(*lButtonLayout);

  QObject::connect(this->mFileWatcher, &QFileSystemWatcher::directoryChanged, this, &BatchConversion::updateOutputPreview);

  this->mHasUserDoneSomething = false;

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.batchConversionDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

BatchConversion::~BatchConversion()
{
  this->mFileWatcher->removePaths(this->mFileWatcher->files());
  delete this->mFileWatcher;
}

void BatchConversion::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    emit modalClosed();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the batch conversion window"),
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

  emit modalClosed();
}

void BatchConversion::reject()
{
  this->close();
}

void BatchConversion::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(1000);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Batch Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/reorder")));
}

void BatchConversion::setupGeneralGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Group box
  auto lGroupBox{ComponentFactory::CreateGroupBox(this, tr("Input location"), "tune", lIconFolder, this->mSettings.display.font.pointSize)};
  aLayout.addWidget(lGroupBox, 0, 0);

  // Grid layout
  auto lLayout{new QGridLayout(lGroupBox)};
  lLayout->setColumnStretch(0, 0);
  lLayout->setColumnStretch(1, 1);
  lLayout->setColumnStretch(2, 0);
  lLayout->setSpacing(10);
  lLayout->setContentsMargins(15, 20, 15, 15);
  lLayout->setAlignment(Qt::AlignTop);
  lLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // First line
  lLayout->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit(this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QString("input_path_directory"));
  lInputPathLineEdit->setDisabled(true);
  lLayout->addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", lIconFolder)};
  lLayout->addWidget(lInputPathChooser, 0, 2);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &BatchConversion::chooseInputDirectory);
}

void BatchConversion::setupSkeletonGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Group box
  auto lGroupBox{ComponentFactory::CreateGroupBox(this, tr("Skeleton"), "skeleton", lIconFolder, this->mSettings.display.font.pointSize)};
  aLayout.addWidget(lGroupBox, 1, 0);

  auto lLayout{new QGridLayout(lGroupBox)};
  lLayout->setColumnStretch(0, 0);
  lLayout->setColumnStretch(1, 1);
  lLayout->setColumnStretch(2, 0);
  lLayout->setSpacing(10);
  lLayout->setContentsMargins(15, 20, 15, 15);
  lLayout->setAlignment(Qt::AlignTop);
  lLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Human skeleton file
  lLayout->addWidget(new QLabel(tr("Skeleton file (human):"), this), 0, 0);

  auto lSkeletonChooserHuman{new QComboBox(this)};
  lSkeletonChooserHuman->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserHuman->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserHuman->setObjectName(QString("skeleton_chooser_human"));
  lLayout->addWidget(lSkeletonChooserHuman, 0, 1);

  // Refresh button
  auto lSkeletonRefresherHuman{ComponentFactory::CreateButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lLayout->addWidget(lSkeletonRefresherHuman, 0, 2);

  // Open assets directory
  auto lOpenAssetsDirectoryHuman{ComponentFactory::CreateButton(this, "View in explorer", "", "folder-search", lIconFolder)};
  lLayout->addWidget(lOpenAssetsDirectoryHuman, 0, 3);

  // Beast skeleton file
  lLayout->addWidget(new QLabel(tr("Skeleton file (beast):"), this), 1, 0);

  auto lSkeletonChooserBeast{new QComboBox(this)};
  lSkeletonChooserBeast->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserBeast->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserBeast->setObjectName(QString("skeleton_chooser_beast"));
  lLayout->addWidget(lSkeletonChooserBeast, 1, 1);

  // Refresh button
  auto lSkeletonRefresherBeast{ComponentFactory::CreateButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lLayout->addWidget(lSkeletonRefresherBeast, 1, 2);

  this->populateSkeletonChoosers();

  // Open assets directory
  auto lOpenAssetsDirectoryBeast{ComponentFactory::CreateButton(this, "View in explorer", "", "folder-search", lIconFolder)};
  lLayout->addWidget(lOpenAssetsDirectoryBeast, 1, 3);

  // Event binding
  this->connect(lSkeletonRefresherHuman, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChoosers);
  this->connect(lOpenAssetsDirectoryHuman, &QPushButton::clicked, this, &BatchConversion::openSkeletonsAssetsDirectory);
  this->connect(lSkeletonRefresherBeast, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChoosers);
  this->connect(lOpenAssetsDirectoryBeast, &QPushButton::clicked, this, &BatchConversion::openSkeletonsAssetsDirectory);
}

void BatchConversion::setupBodySlideGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Group box
  auto lGroupBox{ComponentFactory::CreateGroupBox(this, tr("BodySlide"), "bodyslide-logo", lIconFolder, this->mSettings.display.font.pointSize)};
  aLayout.addWidget(lGroupBox, 2, 0);

  // Grid layout
  auto lLayout{new QGridLayout(lGroupBox)};
  lLayout->setSpacing(10);
  lLayout->setContentsMargins(15, 20, 15, 15);
  lLayout->setAlignment(Qt::AlignTop);
  lLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Targeted body and version
  auto lTargetMeshesPicker{ComponentFactory::CreateTargetMeshesPickerLine(this,
                                                                          *lLayout,
                                                                          true,
                                                                          0,
                                                                          lIconFolder,
                                                                          QString("target_meshes_picker_button"),
                                                                          QString("currently_targeted_body_feet"))};

  // Filters
  lLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 1, 0);

  auto lFiltersWrapper{new QHBoxLayout(lGroupBox)};
  lFiltersWrapper->setMargin(0);
  lLayout->addLayout(lFiltersWrapper, 1, 1);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lFiltersListChooser->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
  lFiltersWrapper->addWidget(lFiltersListChooser);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName(QString("bodyslide_filters"));
  lFiltersList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  lFiltersList->setWordWrap(true);
  lFiltersWrapper->addWidget(lFiltersList);

  auto lEditFilters{ComponentFactory::CreateButton(this, tr("Edit BodySlide filters sets"), "", "filter", lIconFolder, "edit_filters")};
  lFiltersWrapper->addWidget(lEditFilters);

  // Pre-bind initialization functions
  this->targetMeshesChanged(this->mTargetBodyMesh, this->mTargetFeetMesh);

  // Event binding
  this->connect(lTargetMeshesPicker, &QPushButton::clicked, this, &BatchConversion::openTargetMeshesPicker);
  this->connect(lEditFilters, &QPushButton::clicked, this, &BatchConversion::openBodySlideFiltersEditor);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &BatchConversion::updateBodySlideFiltersListPreview);

  // Post-bind initialization functions
  this->initBodySlideFiltersList();
}

void BatchConversion::setupOutputGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Create the group box
  ComponentFactory::CreateOutputBox(this, aLayout, 3, 0, lIconFolder, this->mMinimumFirstColumnWidth, this->mSettings.display.font.pointSize);

  // Event binding
  auto lOutputPathChooser{this->findChild<QPushButton*>(QString("output_path_chooser"))};
  this->connect(lOutputPathChooser, &QPushButton::clicked, this, &BatchConversion::chooseExportDirectory);

  auto lOutputSubpathLineEdit{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))};
  this->connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &BatchConversion::updateOutputPreview);

  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))};
  this->connect(lUseOnlySubdir, &QCheckBox::stateChanged, this, &BatchConversion::useOnlySubdirStateChanged);

  // Pre-filled data
  this->updateOutputPreview();
}

void BatchConversion::setupScanTweaksGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Group box
  auto lGroupBox{ComponentFactory::CreateGroupBox(this, tr("Scan tweaks"), "cog", lIconFolder, this->mSettings.display.font.pointSize)};
  aLayout.addWidget(lGroupBox, 4, 0);

  // Layout
  auto lLayout{new QVBoxLayout(lGroupBox)};
  lLayout->setSpacing(10);
  lLayout->setContentsMargins(15, 20, 15, 15);
  lLayout->setAlignment(Qt::AlignTop);

  // Search mode selector
  auto lSoftSearch{new QRadioButton(tr("Smart search (only files with a name matching a certain pattern are listed)"), this)};
  lSoftSearch->setCursor(Qt::PointingHandCursor);
  lSoftSearch->setObjectName(QString("scan_soft_search"));
  lLayout->addWidget(lSoftSearch);

  auto lHeavierSearch{new QRadioButton(tr("Advanced search (every single .nif file is listed)"), this)};
  lHeavierSearch->setCursor(Qt::PointingHandCursor);
  lHeavierSearch->setObjectName(QString("scan_advanced_search"));
  lLayout->addWidget(lHeavierSearch);

  // Scan only "meshes" directory
  auto lScanMeshesSubdirsOnly{ComponentFactory::CreateCheckBox(this, tr("Only scan the \"meshes\" subdirectories of each mod"), "", "only_scan_meshes_dir", true)};
  lLayout->addWidget(lScanMeshesSubdirsOnly);

  // Clear the irrelevant entries
  auto lMustClearIrrelevantEntries{ComponentFactory::CreateCheckBox(this, tr("Clear the irrelevant entries (mods which do not contain any body, hands and feet mesh)"), "", "clear_irrelevant_entries", true)};
  lLayout->addWidget(lMustClearIrrelevantEntries);

  // Post-bind events
  lSoftSearch->setChecked(true);
}

void BatchConversion::setupGenerationAdjustmentGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Group box
  auto lGroupBox{ComponentFactory::CreateGroupBox(this, tr("Generation tweaks"), "cog", lIconFolder, this->mSettings.display.font.pointSize)};
  aLayout.addWidget(lGroupBox, 5, 0);

  // Layout
  auto lLayout{new QVBoxLayout(lGroupBox)};
  lLayout->setSpacing(10);
  lLayout->setContentsMargins(15, 20, 15, 15);
  lLayout->setAlignment(Qt::AlignTop);

  // Generation method selector
  auto lGenerateDataArchitecture{new QRadioButton(tr("Generate everything for the \"Data\" directory directly"), this)};
  lGenerateDataArchitecture->setCursor(Qt::PointingHandCursor);
  lGenerateDataArchitecture->setObjectName(QString("generate_data_architecture"));
  lLayout->addWidget(lGenerateDataArchitecture);

  auto lGenerateDedicatedModArchitecture{new QRadioButton(tr("Generate each preset in a dedicated directory"), this)};
  lGenerateDedicatedModArchitecture->setCursor(Qt::PointingHandCursor);
  lGenerateDedicatedModArchitecture->setObjectName(QString("generate_dedicated_mod_architecture"));
  lLayout->addWidget(lGenerateDedicatedModArchitecture);

  // Post-bind events
  lGenerateDataArchitecture->setChecked(true);
}

void BatchConversion::setupButtons(QHBoxLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Batch generate button
  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Launch the scan of the directory"), "", "build", lIconFolder)};
  aLayout.addWidget(lGenerateButton);

  // Event binding
  this->connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversion::launchSearchProcess);
}

void BatchConversion::launchPicker(const std::map<QString, std::set<QString>>& aScannedData, const bool aMustGenerateFilesInExistingDirectory)
{
  // Filters list
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lUserFilters{Utils::GetFiltersForExport(this->mFiltersList, lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()), this->mTargetBodyMesh, this->mTargetFeetMesh)};

  // Human skeleton
  auto lSkeletonChooserHuman{this->findChild<QComboBox*>(QString("skeleton_chooser_human"))};
  auto lSkeletonPathHuman{QString("%1assets/skeletons/%2").arg(Utils::GetAppDataPathFolder(), lSkeletonChooserHuman->currentText())};

  // Beast skeleton
  auto lSkeletonChooserBeast{this->findChild<QComboBox*>(QString("skeleton_chooser_beast"))};
  auto lSkeletonPathBeast{QString("%1assets/skeletons/%2").arg(Utils::GetAppDataPathFolder(), lSkeletonChooserBeast->currentText())};

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

  // Generation method
  auto lGenerateEachPresetInDedicatedDir{this->findChild<QRadioButton*>(QString("generate_dedicated_mod_architecture"))->isChecked()};

  // Construct the data container
  auto lData{Struct::BatchConversionData(lSkeletonPathHuman,
                                         lSkeletonPathBeast,
                                         this->mTargetBodyMesh,
                                         this->mTargetFeetMesh,
                                         lUserFilters,
                                         lEntryDirectory,
                                         aMustGenerateFilesInExistingDirectory,
                                         lGenerateEachPresetInDedicatedDir,
                                         Utils::ToMapQsVecQs(aScannedData))};

  auto lBCPicker{new BatchConversionPicker(this, this->mSettings, lData)};
  this->connect(lBCPicker, &BatchConversionPicker::presetsCreationValidated, this, &BatchConversion::batchCreatePresets);
}

void BatchConversion::userHasDoneAnAction()
{
  if (!this->mHasUserDoneSomething)
  {
    this->mHasUserDoneSomething = true;
  }
}

void BatchConversion::userHasDoneAnAction(int)
{
  this->mHasUserDoneSomething = true;

  // Body name selector
  auto lBodyNameSelector{this->findChild<QComboBox*>(QString("body_selector_name"))};
  this->disconnect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BatchConversion::userHasDoneAnAction));

  // Body version selector
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  this->disconnect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BatchConversion::userHasDoneAnAction));
}

void BatchConversion::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "batchConversionInput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "batchConversionInput", lPath);

  if (!this->mHasUserDoneSomething && lPath.compare("") != 0)
  {
    this->userHasDoneAnAction();
  }

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.compare("", Qt::CaseInsensitive) == 0};
  lLineEdit->setDisabled(lMustDisableButton);
}

void BatchConversion::launchSearchProcess()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Input path
  const auto& lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

  // Check if the input path has been given by the user
  if (lInputPath.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no path given to search for files."));
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
  auto lMustGenerateFilesInExistingDirectory{false};
  if (QDir(lEntryDirectory).exists())
  {
    // Since the directory already exist, ask the user to generate another preset in it
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Already existing directory"),
                                      tr("The directory \"%1\" already exists on your computer. Do you still want to generate the files in this directory?").arg(lEntryDirectory),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the search"),
                                      tr("Cancel the search"),
                                      "",
                                      this->mSettings.display.warningColor,
                                      this->mSettings.display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }

    lMustGenerateFilesInExistingDirectory = true;
  }

  // Progress bar
  auto lProgressBar{new QProgressBar(this)};
  lProgressBar->setFormat("");
  lProgressBar->setMinimum(0);
  lProgressBar->setMaximum(0);
  lProgressBar->setValue(0);
  lProgressBar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Scanning the directory. Please wait..."), tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressBar);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  QCoreApplication::processEvents();

  // The map is storing map<original folder path, set<meshes/.../.../fileName>>
  std::map<QString, std::set<QString>> lScannedData;
  std::map<QString, std::set<QString>>::iterator lMapPosition;

  auto lRelativeDirPath{QString()};
  auto lFirstSlashPosition{-1};
  auto lSecondSlashPosition{-1};
  auto lCurrentModSubDirName{QString()};
  auto lFileName{QString()};
  auto lKey{QString()};
  auto lSecondArgument{QString()};

  auto lScanMeshesSubdirsOnly{this->findChild<QCheckBox*>(QString("only_scan_meshes_dir"))->isChecked()};
  auto lMustClearIrrelevantEntries{this->findChild<QCheckBox*>(QString("clear_irrelevant_entries"))->isChecked()};
  auto lHeavierSearchEnabled{this->findChild<QRadioButton*>(QString("scan_advanced_search"))->isChecked()};
  QStringList lMeshesFilesToFind{"femalebody_0.nif",
                                 "femalebody_1.nif",
                                 "femalehands_0.nif",
                                 "femalehands_1.nif",
                                 "femalehandsargonian_0.nif",
                                 "femalehandsargonian_1.nif",
                                 "femalefeet_0.nif",
                                 "femalefeet_1.nif",
                                 "skeleton_female.nif",
                                 "skeletonbeast_female.nif"};

  QDirIterator it(lInputPath, QStringList() << "*.nif", QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    it.next();

    // Check if the file is relative to a body, hands or head textures
    if (lHeavierSearchEnabled || (lMeshesFilesToFind.contains(it.fileInfo().fileName()) || it.fileInfo().fileName().toLower().contains("skeleton")))
    {
      // Get the current directory
      lRelativeDirPath = it.fileInfo().absolutePath().remove(lInputPath + "/", Qt::CaseInsensitive);
      lFirstSlashPosition = lRelativeDirPath.indexOf("/");
      lSecondSlashPosition = lRelativeDirPath.indexOf("/", lFirstSlashPosition + 1);

      // Only scan the subdirs of "meshes" dir
      if (lScanMeshesSubdirsOnly)
      {
        lCurrentModSubDirName = lRelativeDirPath.mid(lFirstSlashPosition + 1, lSecondSlashPosition - (lFirstSlashPosition + 1));
        if (lCurrentModSubDirName.compare("meshes", Qt::CaseInsensitive) != 0)
        {
          continue;
        }
      }

      lFileName = it.fileInfo().fileName();

      // Clean the file name from any artifact
      lFileName.remove("_0.nif", Qt::CaseInsensitive);
      lFileName.remove("_1.nif", Qt::CaseInsensitive);
      lFileName.remove(".nif", Qt::CaseInsensitive);

      // Construct the key of the map
      lKey = lRelativeDirPath.left(lFirstSlashPosition);
      Utils::CleanPathString(lKey);
      lSecondArgument = QString("%1/%2").arg(lRelativeDirPath.mid(lFirstSlashPosition + 1), lFileName);

      // Insert the data
      lScannedData[lKey].insert(lSecondArgument);
    }

    QCoreApplication::processEvents();
  }

  // Hide the progress dialog
  lProgressDialog.hide();

  // Check if some some data has been found
  if (lScannedData.size() == 0)
  {
    Utils::DisplayWarningMessage(tr("No data found for the given input directory. Please try to change it before retrying again.\n\nNote: If you want to convert a single mod only, please use the \"Assisted Conversion\" tool instead.\n\nNote of ModOrganizer2 users: select your \"mods\" directory as the input path."));
    return;
  }

  // Clear the mods entries that do not have any useful entries, if the user wanted to
  if (lMustClearIrrelevantEntries)
  {
    Utils::ClearUselessEntries(lScannedData);
  }

  // Launch the BatchConversionPicker dialog
  this->launchPicker(lScannedData, lMustGenerateFilesInExistingDirectory);
}

void BatchConversion::batchCreatePresets(const Struct::BatchConversionData& aPresetsData)
{
  // Selected body
  const auto& lBodySelected{static_cast<int>(aPresetsData.getBodyMod())};

  // Selected feet
  const auto& lFeetModIndex{aPresetsData.getFeetModIndex()};

  // Output paths
  const auto lEntryDirectory{aPresetsData.getFullOutputPath()};

  // Create main directory
  auto lGenerateFilesInExistingMainDirectory{aPresetsData.getMustGenerateFilesInExistingDirectory()};
  if (!QDir(lEntryDirectory).exists())
  {
    // Wait to know the result of the mkdir()
    if (!QDir().mkpath(lEntryDirectory))
    {
      if (!lGenerateFilesInExistingMainDirectory)
      {
        Utils::DisplayWarningMessage(tr("Error while creating the main directory: \"%1\" could not be created on your computer.\nBe sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths.").arg(lEntryDirectory));
        return;
      }
    }
  }

  // Iterate in the presets list
  for (const auto& lPreset : aPresetsData.presets)
  {
    // Beast hands
    auto lMustUseBeastHands{lPreset.getHandsData().mustUseAlternativeModel()};

    // Body meshes path and name
    const auto lBodyLastSlashPosition{lPreset.getBodyData().second.lastIndexOf('/')};
    const auto lMeshesPathBody{lPreset.getBodyData().second.left(lBodyLastSlashPosition)};
    const auto lBodyName{lPreset.getBodyData().second.mid(lBodyLastSlashPosition + 1)};

    // Feet meshes path and name
    const auto lFeetLastSlashPosition{lPreset.getFeetData().second.lastIndexOf('/')};
    const auto lMeshesPathFeet{lPreset.getFeetData().second.left(lFeetLastSlashPosition)};
    const auto lFeetName{lPreset.getFeetData().second.mid(lFeetLastSlashPosition + 1)};

    // Hands meshes path and name
    const auto lHandsLastSlashPosition{lPreset.getHandsData().getResourcePath().lastIndexOf('/')};
    const auto lMeshesPathHands{lPreset.getHandsData().getResourcePath().left(lHandsLastSlashPosition)};
    const auto lHandsName{lPreset.getHandsData().getResourcePath().mid(lHandsLastSlashPosition + 1)};

    // Analyze the data that needs to be generated
    unsigned char lOptions{0};

    // Body
    if (!lMeshesPathBody.isEmpty() && !lBodyName.isEmpty())
    {
      lOptions += 100;
    }

    // Feet
    if (!lMeshesPathFeet.isEmpty() && !lFeetName.isEmpty())
    {
      lOptions += 10;
    }

    // Hands
    if (!lMeshesPathHands.isEmpty() && !lHandsName.isEmpty())
    {
      lOptions += 1;
    }

    // BodySlide names
    auto lOSPXMLNames{lPreset.getNames().first};
    auto lBodyslideSlidersetsNames{lPreset.getNames().second};

    // If the user wants to generate each preset in a dedicated directory,
    // update the path by adding the
    auto lPresetEntryDirectory{lEntryDirectory};
    if (aPresetsData.getMustGenerateEachPresetInADedicatedDirectory())
    {
      lPresetEntryDirectory.append('/').append(lOSPXMLNames);
    }

    // Export the meshes
    // XML file
    auto lSelectedBodyName{static_cast<BodyNameVersion>(lBodySelected)};

    if (!Utils::CreateXMLFile(lPresetEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, lSelectedBodyName, lFeetModIndex, lBodyslideSlidersetsNames, aPresetsData.getFiltersList(), true, lOptions))
    {
      // Remove the directory since the generation is incomplete
      if (!lGenerateFilesInExistingMainDirectory)
      {
        Utils::RemoveDirectoryAndSubDirs(lPresetEntryDirectory);
      }

      return;
    }

    // OSP file
    if (!Utils::CreateOSPFile(lPresetEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, lSelectedBodyName, lFeetModIndex, lBodyslideSlidersetsNames, lMeshesPathBody, lMeshesPathFeet, lMeshesPathHands, lBodyName, lFeetName, lHandsName, true, lOptions))
    {
      // Remove the directory since the generation is incomplete
      if (!lGenerateFilesInExistingMainDirectory)
      {
        Utils::RemoveDirectoryAndSubDirs(lPresetEntryDirectory);
      }

      return;
    }

    // Skeleton
    auto lMustCopySkeleton{!lPreset.getSkeletonData().getResourcePath().isEmpty()};

    if (lMustCopySkeleton)
    {
      // Read location
      QString lSourceSkeletonReadPath;
      if (lPreset.mustSkeletonUseAlternativeModel())
      {
        // Beast skeleton
        lSourceSkeletonReadPath = aPresetsData.getBeastSkeletonPath();
      }
      else
      {
        // Human skeleton
        lSourceSkeletonReadPath = aPresetsData.getHumanSkeletonPath();
      }

      // Write location
      const auto lSkeletonLastSlashPosition{lPreset.getSkeletonData().getResourcePath().lastIndexOf('/')};
      const auto lDestinationSkeletonRelativePath{lPreset.getSkeletonData().getResourcePath().left(lSkeletonLastSlashPosition)};
      const auto lDestinationSkeletonFileName{lPreset.getSkeletonData().getResourcePath().mid(lSkeletonLastSlashPosition + 1)};

      if (!Utils::CreateSkeletonFile(lSourceSkeletonReadPath, lPresetEntryDirectory, lDestinationSkeletonRelativePath, lDestinationSkeletonFileName))
      {
        // Remove the directory since the generation is incomplete
        if (!lGenerateFilesInExistingMainDirectory)
        {
          Utils::RemoveDirectoryAndSubDirs(lPresetEntryDirectory);
        }

        return;
      }
    }
  }

  // Update the color of the output directory preview
  this->updateOutputPreview();

  auto lTitle{tr("Batch generation successful")};
  auto lMessage{tr("Every preset has been correctly generated.")};

  // Open the directory where the file structure has been created
  if (this->mSettings.batchConversion.automaticallyOpenFinalDirectory)
  {
    Utils::DisplayInfoMessage(this, lTitle, lMessage, "icons", "green-info", tr("Open the batch generated directory"));
    QDesktopServices::openUrl(QUrl::fromLocalFile(aPresetsData.getFullOutputPath()));
  }
  else if (Utils::DisplayQuestionMessage(this, lTitle, lMessage, "icons", "green-info", tr("Open the batch generated directory"), tr("OK"), "", "", "", "", false) == ButtonClicked::YES)
  {
    QDesktopServices::openUrl(QUrl::fromLocalFile(aPresetsData.getFullOutputPath()));
  }
}

void BatchConversion::populateSkeletonChoosers()
{
  auto lRootDir{Utils::GetAppDataPathFolder() + "assets/skeletons/"};
  Utils::CleanPathString(lRootDir);
  QStringList lAvailableSkeletons;

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList() << QString("*.nif"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lAvailableSkeletons.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
  }

  /*=======*/
  /* Human */
  /*=======*/
  // Get the combobox
  const auto lSkeletonChooserHuman{this->findChild<QComboBox*>(QString("skeleton_chooser_human"))};

  // Save the selected skeleton file name
  const auto lPreviousIndexHuman{lSkeletonChooserHuman->currentIndex()};
  const auto lPreviousHumanSkeleton{lSkeletonChooserHuman->itemText(lPreviousIndexHuman)};

  // Clear the combo box and add the found files to it
  lSkeletonChooserHuman->clear();
  lSkeletonChooserHuman->addItems(lAvailableSkeletons);

  // Reselect the previously selected skeleton, if it still exists
  if (lPreviousIndexHuman != -1)
    lSkeletonChooserHuman->setCurrentIndex(std::max(lAvailableSkeletons.indexOf(lPreviousHumanSkeleton), 0));
  else if (lSkeletonChooserHuman->count() > 0)
    lSkeletonChooserHuman->setCurrentIndex(0);

  /*=======*/
  /* Beast */
  /*=======*/
  // Get the combobox
  const auto lSkeletonChooserBeast{this->findChild<QComboBox*>(QString("skeleton_chooser_beast"))};

  // Save the selected skeleton file name
  const auto lPreviousIndexBeast{lSkeletonChooserBeast->currentIndex()};
  const auto lPreviousBeastSkeleton{lSkeletonChooserBeast->itemText(lPreviousIndexBeast)};

  // Clear the combo box and add the found files to it
  lSkeletonChooserBeast->clear();
  lSkeletonChooserBeast->addItems(lAvailableSkeletons);

  // Reselect the previously selected skeleton, if it still exists
  if (lPreviousIndexBeast != -1)
    lSkeletonChooserBeast->setCurrentIndex(std::max(lAvailableSkeletons.indexOf(lPreviousBeastSkeleton), 0));
  else if (lSkeletonChooserBeast->count() > 0)
    lSkeletonChooserBeast->setCurrentIndex(0);
}

void BatchConversion::openSkeletonsAssetsDirectory()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(QString("%1assets/skeletons").arg(Utils::GetAppDataPathFolder())));
}

void BatchConversion::useOnlySubdirStateChanged(int)
{
  this->updateOutputPreview();
}

void BatchConversion::updateOutputPreview()
{
  this->userHasDoneAnAction();

  auto lMainDirTextEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("output_path_preview"))};

  Utils::UpdateOutputPreview(this->mFileWatcher, lMainDirTextEdit, lSubDirectory, lUseOnlySubdir, this->mSettings.display.successColor, this->mSettings.display.warningColor, this->mSettings.display.dangerColor, lOutputPathsPreview);
}

void BatchConversion::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "batchConversionOutput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "batchConversionOutput", lPath);
  this->updateOutputPreview();
}

void BatchConversion::openTargetMeshesPicker()
{
  auto lDialog{new TargetMeshesPicker(this, this->mSettings, this->mTargetBodyMesh, this->mTargetFeetMesh)};
  this->connect(lDialog, &TargetMeshesPicker::valuesChosen, this, &BatchConversion::targetMeshesChanged);
}

void BatchConversion::targetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet)
{
  // Update the class members
  this->mTargetBodyMesh = aBody;
  this->mTargetFeetMesh = aFeet;

  // Update the "targeted body mesh" text content
  const auto lBodyText{
    QString("%1 [v.%2]").arg(DataLists::GetBodyVariantsList(DataLists::GetName(aBody), DataLists::GetVersionIndex(aBody)).at(DataLists::GetVariantIndex(aBody)), DataLists::GetVersionString(aBody))};

  // Update the "targeted feet mesh" text content
  const auto lFeetText{
    QString("%1 [v.%2]").arg(DataLists::GetFeetVariantsList(DataLists::GetName(aFeet), DataLists::GetVersionIndex(aFeet), Utils::IsCBBEBasedBody(aBody)).at(DataLists::GetVariantIndex(aFeet)), DataLists::GetVersionString(aBody, aFeet))};

  auto lCurrentlyTargetedBody{this->findChild<QLabel*>("currently_targeted_body_feet")};
  lCurrentlyTargetedBody->setText(tr("Targeted body: %1\nTargeted feet: %2").arg(lBodyText, lFeetText));

  // Force the refresh of the filters list preview
  this->updateBodySlideFiltersListPreview();
}

void BatchConversion::openBodySlideFiltersEditor()
{
  auto lEditor{new BodySlideFiltersEditor(this, this->mSettings, this->mFiltersList)};
  this->connect(lEditor, &BodySlideFiltersEditor::listEdited, this, &BatchConversion::updateBodySlideFiltersList);
}

void BatchConversion::initBodySlideFiltersList()
{
  // Load and save the filters list
  this->mFiltersList = Utils::LoadFiltersFromFile();

  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};

  // Disable the combobox if there is not any available filter
  if (this->mFiltersList.size() == 0)
  {
    lFiltersListChooser->setDisabled(true);
    return;
  }

  // Fill the combobox
  for (const auto& lPair : this->mFiltersList)
  {
    lFiltersListChooser->addItem(lPair.first);
  }

  lFiltersListChooser->setCurrentIndex(0);
}

void BatchConversion::updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList)
{
  // Do not update anything if the list is the same as the one already set
  if (this->mFiltersList == aFilterList)
  {
    return;
  }

  this->mFiltersList = aFilterList;
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};
  Utils::UpdateComboBoxBodyslideFiltersList(this->mFiltersList, lFiltersListChooser, lFiltersList);
}

void BatchConversion::updateBodySlideFiltersListPreview()
{
  // Get the GUI widgets
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};

  // Get any eventual additional filters
  auto lAdditionalFilter{Utils::GetAdditionalFeetFilter(this->mTargetBodyMesh, this->mTargetFeetMesh)};
  auto lText{QString()};

  if (lFiltersListChooser->currentIndex() != -1)
  {
    lText = this->mFiltersList.find(lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()))->second.join(QString(" ; "));

    if (!lAdditionalFilter.isEmpty())
    {
      lText.append(" ; ");
      lText.append(lAdditionalFilter);
      lText.append(tr(" (feet only)"));
    }
  }

  lFiltersList->setText(lText);

  this->userHasDoneAnAction();
}
