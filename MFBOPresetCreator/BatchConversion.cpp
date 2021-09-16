#include "BatchConversion.h"
#include "BatchConversionPicker.h"
#include "BodySlideFiltersEditor.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include "Utils.h"
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDirIterator>
#include <QFileDialog>
#include <QProgressBar>
#include <QProgressDialog>
#include <QRadioButton>
#include <QScrollArea>
#include <QStyledItemDelegate>

BatchConversion::BatchConversion(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mMinimumFirstColumnWidth(300)
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
  this->setupRemainingGUI(*lMainLayout);
  this->setupButtons(*lButtonLayout);

  this->mHasUserDoneSomething = false;

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.batchConversionDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void BatchConversion::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the batch conversion window"),
                                    "",
                                    this->mSettings.dangerColor,
                                    this->mSettings.successColor,
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
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // General group box
  auto lGeneralGroupBox{new QGroupBox(tr("General").append("  "), this)};
  Utils::AddIconToGroupBox(lGeneralGroupBox, lIconFolder, "tune", this->mSettings.font.size);
  this->connect(lGeneralGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::SetGroupBoxState(lGeneralGroupBox, false);
  aLayout.addWidget(lGeneralGroupBox, 0, 0);

  // Grid layout
  auto lGeneralGridLayout{new QGridLayout(lGeneralGroupBox)};
  lGeneralGridLayout->setColumnStretch(0, 0);
  lGeneralGridLayout->setColumnStretch(1, 1);
  lGeneralGridLayout->setColumnStretch(2, 1);
  lGeneralGridLayout->setColumnStretch(3, 2);
  lGeneralGridLayout->setColumnStretch(4, 0);
  lGeneralGridLayout->setSpacing(10);
  lGeneralGridLayout->setContentsMargins(15, 20, 15, 15);
  lGeneralGridLayout->setAlignment(Qt::AlignTop);
  lGeneralGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // First line
  lGeneralGridLayout->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QString("input_path_directory"));
  lInputPathLineEdit->setDisabled(true);
  lGeneralGridLayout->addWidget(lInputPathLineEdit, 0, 1, 1, 3);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", lIconFolder)};
  lGeneralGridLayout->addWidget(lInputPathChooser, 0, 4);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &BatchConversion::chooseInputDirectory);
}

void BatchConversion::setupSkeletonGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Custom skeleton group box
  auto lSkeletonGroupBox{new QGroupBox(tr("Skeleton").append("  "), this)};
  Utils::AddIconToGroupBox(lSkeletonGroupBox, lIconFolder, "skeleton", this->mSettings.font.size);
  this->connect(lSkeletonGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::SetGroupBoxState(lSkeletonGroupBox, false);
  aLayout.addWidget(lSkeletonGroupBox, 1, 0);

  auto lSkeletonGridLayout{new QGridLayout(lSkeletonGroupBox)};
  lSkeletonGridLayout->setColumnStretch(0, 0);
  lSkeletonGridLayout->setColumnStretch(1, 1);
  lSkeletonGridLayout->setColumnStretch(2, 0);
  lSkeletonGridLayout->setSpacing(10);
  lSkeletonGridLayout->setContentsMargins(15, 20, 15, 15);
  lSkeletonGridLayout->setAlignment(Qt::AlignTop);
  lSkeletonGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Human skeleton file
  lSkeletonGridLayout->addWidget(new QLabel(tr("Skeleton file (human):"), this), 1, 0);

  auto lSkeletonChooserHuman{new QComboBox(this)};
  lSkeletonChooserHuman->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserHuman->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserHuman->setObjectName(QString("skeleton_chooser_human"));
  lSkeletonGridLayout->addWidget(lSkeletonChooserHuman, 1, 1);

  auto lSkeletonRefresherHuman{ComponentFactory::CreateButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lSkeletonGridLayout->addWidget(lSkeletonRefresherHuman, 1, 2);

  // Beast skeleton file
  lSkeletonGridLayout->addWidget(new QLabel(tr("Skeleton file (beast):"), this), 2, 0);

  auto lSkeletonChooserBeast{new QComboBox(this)};
  lSkeletonChooserBeast->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserBeast->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserBeast->setObjectName(QString("skeleton_chooser_beast"));
  lSkeletonGridLayout->addWidget(lSkeletonChooserBeast, 2, 1);

  auto lSkeletonRefresherBeast{ComponentFactory::CreateButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lSkeletonGridLayout->addWidget(lSkeletonRefresherBeast, 2, 2);

  this->populateSkeletonChoosers();

  // Event binding
  this->connect(lSkeletonRefresherHuman, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChoosers);
  this->connect(lSkeletonRefresherBeast, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChoosers);
}

void BatchConversion::setupBodySlideGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // BodySlide output settings group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide").append("  "), this)};
  Utils::AddIconToGroupBox(lBodyslideGroupBox, lIconFolder, "bodyslide-logo", this->mSettings.font.size);
  this->connect(lBodyslideGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::SetGroupBoxState(lBodyslideGroupBox, false);
  aLayout.addWidget(lBodyslideGroupBox, 2, 0);

  // Grid layout
  auto lBodyslideGridLayout{new QGridLayout(lBodyslideGroupBox)};
  lBodyslideGridLayout->setColumnStretch(0, 0);
  lBodyslideGridLayout->setColumnStretch(1, 1);
  lBodyslideGridLayout->setColumnStretch(2, 1);
  lBodyslideGridLayout->setColumnStretch(3, 2);
  lBodyslideGridLayout->setColumnStretch(4, 0);
  lBodyslideGridLayout->setSpacing(10);
  lBodyslideGridLayout->setContentsMargins(15, 20, 15, 15);
  lBodyslideGridLayout->setAlignment(Qt::AlignTop);
  lBodyslideGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Targeted body and version
  auto lDefaultBodyVersionSettings{DataLists::GetSplittedNameVersionFromBodyVersion(mSettings.defaultBatchConversionBody)};

  lBodyslideGridLayout->addWidget(new QLabel(tr("Targeted body and version:"), this), 0, 0);

  // Body Name
  auto lBodyNameSelector{new QComboBox(this)};
  lBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lBodyNameSelector->addItems(DataLists::GetBodiesNames());
  lBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lBodyNameSelector->setObjectName(QString("body_selector_name"));
  lBodyslideGridLayout->addWidget(lBodyNameSelector, 0, 1);

  // Body Version
  auto lBodyVersionSelector{new QComboBox(this)};
  lBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lBodyVersionSelector->addItems(DataLists::GetVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lBodyVersionSelector->setObjectName(QString("body_selector_version"));
  lBodyslideGridLayout->addWidget(lBodyVersionSelector, 0, 2);

  // Feet mod chooser
  auto lFeetSelector{new QComboBox(this)};
  lFeetSelector->setItemDelegate(new QStyledItemDelegate());
  lFeetSelector->setCursor(Qt::PointingHandCursor);
  lFeetSelector->addItems(DataLists::GetFeetModsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lFeetSelector->setCurrentIndex(mSettings.defaultMainFeetMod);
  lFeetSelector->setObjectName(QString("feet_mod_selector"));
  lBodyslideGridLayout->addWidget(lFeetSelector, 0, 3);

  // BodySlide filters
  lBodyslideGridLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 3, 0);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lBodyslideGridLayout->addWidget(lFiltersListChooser, 3, 1);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName(QString("bodyslide_filters"));
  lFiltersList->setWordWrap(true);
  lBodyslideGridLayout->addWidget(lFiltersList, 3, 2, 1, 2);

  auto lEditFilters{ComponentFactory::CreateButton(this, tr("Edit BodySlide filters sets"), "", "filter", lIconFolder, "edit_filters")};
  lBodyslideGridLayout->addWidget(lEditFilters, 3, 4);

  // Event binding
  this->connect(lEditFilters, &QPushButton::clicked, this, &BatchConversion::openBodySlideFiltersEditor);
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &BatchConversion::updateBodySlideFiltersListPreview);
  this->connect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &BatchConversion::updateBodySlideFiltersListPreview);
  this->connect(lFeetSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &BatchConversion::updateBodySlideFiltersListPreview);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &BatchConversion::updateBodySlideFiltersListPreview);

  // Post-bind initialization functions
  this->initBodySlideFiltersList();
}

void BatchConversion::setupOutputGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Create the group box
  ComponentFactory::CreateOutputBox(this, aLayout, 3, 0, lIconFolder, mSettings.batchConversionOutputPath, this->mMinimumFirstColumnWidth, this->mSettings.font.size);
  auto lOutputGroupBox{this->findChild<QGroupBox*>(QString("output_group_box"))};
  this->connect(lOutputGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);

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

void BatchConversion::setupRemainingGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Group box
  auto lScanTweaksGroupBox{new QGroupBox(tr("Scan tweaks").append("  "), this)};
  Utils::AddIconToGroupBox(lScanTweaksGroupBox, lIconFolder, "cog", this->mSettings.font.size);
  this->connect(lScanTweaksGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::SetGroupBoxState(lScanTweaksGroupBox, false);
  aLayout.addWidget(lScanTweaksGroupBox, 4, 0);

  // Grid layout
  auto lScanTweaksGridLayout{new QVBoxLayout(lScanTweaksGroupBox)};
  lScanTweaksGridLayout->setSpacing(10);
  lScanTweaksGridLayout->setContentsMargins(15, 20, 15, 15);
  lScanTweaksGridLayout->setAlignment(Qt::AlignTop);

  // Soft search selector
  auto lSoftSearch{new QRadioButton(tr("Smart search (only files with a name matching a certain pattern are listed)"), this)};
  lSoftSearch->setCursor(Qt::PointingHandCursor);
  lSoftSearch->setObjectName(QString("scan_soft_search"));
  lScanTweaksGridLayout->addWidget(lSoftSearch);

  auto lHeavierSearch{new QRadioButton(tr("Advanced search (every single .nif file is listed)"), this)};
  lHeavierSearch->setCursor(Qt::PointingHandCursor);
  lHeavierSearch->setObjectName(QString("scan_advanced_search"));
  lScanTweaksGridLayout->addWidget(lHeavierSearch);

  // Scan only "meshes" directory
  auto lScanMeshesSubdirsOnly{ComponentFactory::CreateCheckBox(this, tr("Only scan the \"meshes\" subdirectories of each mod"), "", "only_scan_meshes_dir", true)};
  lScanTweaksGridLayout->addWidget(lScanMeshesSubdirsOnly);

  // Clear the irrelevant entries
  auto lMustClearIrreleventEntries{ComponentFactory::CreateCheckBox(this, tr("Clear the irrelevent entries (mods which do not contain a boby, hands or feet mesh)"), "", "clear_irrelevent_entries", true)};
  lScanTweaksGridLayout->addWidget(lMustClearIrreleventEntries);

  lSoftSearch->setChecked(true);
}

void BatchConversion::setupButtons(QHBoxLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Batch generate button
  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Launch the scan of the directory"), "", "build", lIconFolder)};
  aLayout.addWidget(lGenerateButton);

  // Event binding
  this->connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversion::launchBatchGenerationProcess);
}

void BatchConversion::clearScannedDataFromUselessEntries(std::map<QString, std::set<QString>>& aScannedData)
{
  auto lUsefulDataFound{false};

  auto lIt = aScannedData.begin();
  for (lIt; lIt != aScannedData.end(); ++lIt)
  {
    lUsefulDataFound = Utils::ContainsBodyOrHandsOrFeetMesh(lIt->second);

    // If no useful data has been found, delete the current entry from the map
    if (!lUsefulDataFound)
    {
      lIt = aScannedData.erase(lIt);
      lIt--;
    }

    // Reset the state
    lUsefulDataFound = false;
  }
}

void BatchConversion::launchPicker(const std::map<QString, std::set<QString>>& aScannedData)
{
  if (aScannedData.size() == 0)
  {
    Utils::DisplayWarningMessage(tr("No data found for the given input directory. Please try to change it before retrying again.\n\nNote: If you want to convert a single mod only, please use the \"Assisted Conversion Tool\" instead.\n\nNote of ModOrganizer2 users: select your \"mods\" directory as the input path."));
    return;
  }

  // TODO: double check from PresetCreator class, if the calls are totally identical
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{DataLists::GetBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};
  auto lFeetModIndex{this->findChild<QComboBox*>(QString("feet_mod_selector"))->currentIndex()};

  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lUserFilters{Utils::GetFiltersForExport(this->mFiltersList, lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()), lBodySelected, lFeetModIndex)};

  auto lSkeletonChooserHuman{this->findChild<QComboBox*>(QString("skeleton_chooser_human"))};
  auto lSkeletonPathHuman{QString("%1assets/skeletons/%2").arg(Utils::GetAppDataPathFolder()).arg(lSkeletonChooserHuman->currentText())};

  auto lSkeletonChooserBeast{this->findChild<QComboBox*>(QString("skeleton_chooser_beast"))};
  auto lSkeletonPathBeast{QString("%1assets/skeletons/%2").arg(Utils::GetAppDataPathFolder()).arg(lSkeletonChooserBeast->currentText())};

  // Output paths
  auto lMainDirectory{this->findChild<QLineEdit*>(QString("output_path_directory"))->text().trimmed()};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  Utils::CleanPathString(lSubDirectory);

  // Does the user want to define the path only through the secondary path?
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};

  // Full extract path
  auto lEntryDirectory{lSubDirectory};
  if (!lUseOnlySubdir)
  {
    lEntryDirectory = (lSubDirectory.isEmpty() ? lMainDirectory : (lMainDirectory + "/" + lSubDirectory));
  }

  auto lData{Struct::BatchConversionData()};
  lData.scannedData = aScannedData;
  lData.humanSkeletonPath = lSkeletonPathHuman;
  lData.beastSkeletonPath = lSkeletonPathBeast;
  lData.bodyMod = lBodySelected;
  lData.feetModIndex = lFeetModIndex;
  lData.filters = lUserFilters;
  lData.fullOutputPath = lEntryDirectory;

  new BatchConversionPicker(this, this->mSettings, lData);
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

// TODO: Reconnect the function below:
void BatchConversion::updateAvailableBodyVersions()
{
  auto lBodyName{static_cast<BodyName>(this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex())};

  // Version
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  lBodyVersionSelector->clear();
  lBodyVersionSelector->addItems(DataLists::GetVersionsFromBodyName(lBodyName));
  lBodyVersionSelector->setCurrentIndex(0);

  // Feet mod
  auto lFeetSelector{this->findChild<QComboBox*>(QString("feet_mod_selector"))};
  lFeetSelector->clear();
  lFeetSelector->addItems(DataLists::GetFeetModsFromBodyName(lBodyName));
  lFeetSelector->setCurrentIndex(0);
  // TODO: Make the modifications to take into account the new way the selector updates
}

void BatchConversion::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "batchConversionInput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "batchConversionInput", lPath);

  if (!this->mHasUserDoneSomething && lPath.compare("") != 0)
  {
    this->mHasUserDoneSomething = true;
  }

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.compare("", Qt::CaseInsensitive) == 0};
  lLineEdit->setDisabled(lMustDisableButton);
}

void BatchConversion::launchBatchGenerationProcess()
{
  // Progress bar
  auto lProgressbar{new QProgressBar(this)};
  lProgressbar->setFormat("");
  lProgressbar->setMinimum(0);
  lProgressbar->setMaximum(0);
  lProgressbar->setValue(0);
  lProgressbar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Scanning the directory. Please wait..."), tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressbar);
  lProgressDialog.setWindowFlags(lProgressDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  qApp->processEvents();

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
  auto lMustClearIrreleventEntries{this->findChild<QCheckBox*>(QString("clear_irrelevent_entries"))->isChecked()};
  auto lHeavierSearchEnabled{this->findChild<QRadioButton*>(QString("scan_advanced_search"))->isChecked()};
  auto lMeshesFilesToFind{QStringList({"femalebody_0.nif",
                                       "femalebody_1.nif",
                                       "femalehands_0.nif",
                                       "femalehands_1.nif",
                                       "femalehandsargonian_0.nif",
                                       "femalehandsargonian_1.nif",
                                       "femalefeet_0.nif",
                                       "femalefeet_1.nif",
                                       "skeleton_female.nif",
                                       "skeletonbeast_female.nif"})};

  const auto& lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};
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
      lSecondArgument = QString("%1/%2").arg(lRelativeDirPath.mid(lFirstSlashPosition + 1)).arg(lFileName);

      lMapPosition = lScannedData.find(lKey);
      if (lMapPosition != lScannedData.end())
      {
        // Insert the data in the already existing map entry
        lMapPosition->second.insert(lSecondArgument);
      }
      else
      {
        // Create a new map entry
        lScannedData.insert(std::make_pair(lKey, std::set<QString>({lSecondArgument})));
      }
    }

    qApp->processEvents();
  }

  // Clear the mods entries that do not have any useful entries
  if (lMustClearIrreleventEntries)
    this->clearScannedDataFromUselessEntries(lScannedData);

  // Launch the BatchConversionPicker dialog
  this->launchPicker(lScannedData);
}

void BatchConversion::populateSkeletonChoosers()
{
  auto lRootDir{Utils::GetAppDataPathFolder() + "assets/skeletons/"};
  Utils::CleanPathString(lRootDir);
  auto lAvailableSkeletons{QStringList()};

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList() << QString("*.nif"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lAvailableSkeletons.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
  }

  // Clear the combo box and add the found files to it
  auto lSkeletonChooserHuman{this->findChild<QComboBox*>(QString("skeleton_chooser_human"))};
  lSkeletonChooserHuman->clear();
  lSkeletonChooserHuman->addItems(lAvailableSkeletons);

  auto lSkeletonChooserBeast{this->findChild<QComboBox*>(QString("skeleton_chooser_beast"))};
  lSkeletonChooserBeast->clear();
  lSkeletonChooserBeast->addItems(lAvailableSkeletons);
}

void BatchConversion::useOnlySubdirStateChanged(int)
{
  this->updateOutputPreview();
}

void BatchConversion::updateOutputPreview()
{
  this->mHasUserDoneSomething = true;

  auto lMainDirTextEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("output_path_preview"))};

  Utils::UpdateOutputPreview(lMainDirTextEdit, lSubDirectory, lUseOnlySubdir, this->mSettings.successColor, this->mSettings.warningColor, this->mSettings.dangerColor, lOutputPathsPreview);
}

void BatchConversion::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "batchConversionOutput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "batchConversionOutput", lPath);
  this->updateOutputPreview();
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
  this->mFiltersList = aFilterList;
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};
  Utils::UpdateComboBoxBodyslideFiltersList(this->mFiltersList, lFiltersListChooser, lFiltersList);
}

void BatchConversion::updateBodySlideFiltersListPreview(int)
{
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{DataLists::GetBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};
  auto lFeetModIndex{this->findChild<QComboBox*>(QString("feet_mod_selector"))->currentIndex()};

  // Take custom MSF filter
  auto lAdditionalFilter{Utils::GetAdditionalFeetFilter(lBodySelected, lFeetModIndex)};

  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};

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

void BatchConversion::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::SetGroupBoxState(lGroupBox, !aIsChecked);
}
