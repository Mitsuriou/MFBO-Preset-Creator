#include "AssistedConversion.h"

AssistedConversion::AssistedConversion(QWidget* parent, const Struct::Settings& aSettings)
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

void AssistedConversion::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Assisted Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/pencil")));
}

void AssistedConversion::initializeGUI()
{
  // Main window container
  auto lMainGrid{new QGridLayout(this)};
  lMainGrid->setObjectName("main_layout");
  lMainGrid->setAlignment(Qt::AlignTop);
  this->setupInterface(*lMainGrid);
}

void AssistedConversion::setupInterface(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // First line
  auto lInputPathLabel{new QLabel(tr("Input path:"), this)};
  aLayout.addWidget(lInputPathLabel, 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  aLayout.addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lInputPathChooser->setCursor(Qt::PointingHandCursor);
  lInputPathChooser->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/folder")));
  lInputPathChooser->setAutoDefault(false);
  lInputPathChooser->setDefault(false);
  aLayout.addWidget(lInputPathChooser, 0, 2);

  // Launch search button
  auto lLaunchSearchButton{new QPushButton(tr("Launch the scan of the mod"), this)};
  lLaunchSearchButton->setCursor(Qt::PointingHandCursor);
  lLaunchSearchButton->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/search")));
  lLaunchSearchButton->setObjectName("launch_search_button");
  lLaunchSearchButton->setAutoDefault(false);
  lLaunchSearchButton->setDefault(false);
  lLaunchSearchButton->setDisabled(true);
  aLayout.addWidget(lLaunchSearchButton, 1, 0, 1, 3, Qt::AlignTop);

  // Hint zone
  this->displayHintZone();

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &AssistedConversion::chooseInputDirectory);
  this->connect(lLaunchSearchButton, &QPushButton::clicked, this, &AssistedConversion::launchSearchProcess);
}

void AssistedConversion::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  auto lMainLayout{this->findChild<QGridLayout*>("main_layout")};
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setMinimumHeight(300);
  lHintZone->setObjectName("hint_zone");
  lHintZone->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lHintZone, 2, 0, 1, 3);
}

void AssistedConversion::deleteAlreadyExistingWindowBottom()
{
  auto lHintZone{this->findChild<QPushButton*>("hint_zone")};
  if (lHintZone)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lOldValidationButton{this->findChild<QPushButton*>("validate_selection")};
  if (lOldValidationButton)
  {
    delete lOldValidationButton;
    lOldValidationButton = nullptr;
  }

  auto lOldScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  if (lOldScrollArea)
  {
    delete lOldScrollArea;
    lOldScrollArea = nullptr;
  }
}

std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> AssistedConversion::scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension)
{
  // The map is storing <path+fileName, <path, fileName>>
  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> lScannedValues;

  auto lAbsFilePath{QString("")};
  auto lRelativeDirPath{QString("")};
  auto lFileName{QString("")};

  QDirIterator it(aRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Get the current directory
    lAbsFilePath = it.fileInfo().absolutePath();
    lRelativeDirPath = lAbsFilePath.remove(aRootDir + "/", Qt::CaseInsensitive);

    lFileName = it.fileInfo().fileName();

    // Clean the file name from any artifact
    lFileName.remove("_0.nif", Qt::CaseInsensitive);
    lFileName.remove("_1.nif", Qt::CaseInsensitive);
    lFileName.remove(".nif", Qt::CaseInsensitive);

    auto lKey{lRelativeDirPath.toStdString() + lFileName.toStdString()};
    auto lValue{std::make_pair(lRelativeDirPath, lFileName)};
    lScannedValues.insert(std::make_pair(lKey, lValue));
  }

  return lScannedValues;
}

void AssistedConversion::createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int& aRowIndex)
{
  aLayout.addWidget(new QLabel(aPath, this), aRowIndex, 0, Qt::AlignLeft);
  aLayout.addWidget(new QLabel(aFileName, this), aRowIndex, 1, Qt::AlignLeft);

  auto lChoiceCombo{new QComboBox(this)};
  lChoiceCombo->addItems(DataLists::getAssistedConversionActions());
  lChoiceCombo->setCursor(Qt::PointingHandCursor);
  lChoiceCombo->setMouseTracking(true);
  lChoiceCombo->view()->setMouseTracking(true);
  lChoiceCombo->setItemDelegate(new QStyledItemDelegate());
  aLayout.addWidget(lChoiceCombo, aRowIndex, 2, Qt::AlignRight);

  // Add a new index store to the list
  this->mBoxSelectedIndexes.push_back(0);

  // Event binding
  this->connect(lChoiceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AssistedConversion::modifyComboBoxLockState);
}

std::vector<Struct::AssistedConversionResult> AssistedConversion::getChosenValuesFromInterface()
{
  // Fetch the grid layout
  auto lDataContainer{this->findChild<QGridLayout*>("data_container")};

  // Iterate in the layout
  auto lLinesToTreat{lDataContainer->rowCount()};

  if (lLinesToTreat <= 1)
  {
    return std::vector<Struct::AssistedConversionResult>();
  }

  std::vector<Struct::AssistedConversionResult> lResults;
  QComboBox* lComboBox{nullptr};
  auto lFilePath{QString("")};
  auto lFileName{QString("")};

  // For each row (skip the row 0 because it is a "header")
  for (int i = 1; i < lLinesToTreat; i++)
  {
    lComboBox = qobject_cast<QComboBox*>(lDataContainer->itemAtPosition(i, 2)->widget());

    // Third column is the chosen action on the line
    if (lComboBox->currentIndex() == 0)
    {
      continue;
    }

    // First column is the file path
    lFilePath = qobject_cast<QLabel*>(lDataContainer->itemAtPosition(i, 0)->widget())->text();

    // Second column is the file name
    lFileName = qobject_cast<QLabel*>(lDataContainer->itemAtPosition(i, 1)->widget())->text();

    // Save the gotten values
    Struct::AssistedConversionResult lResult;
    lResult.path = lFilePath;
    lResult.name = lFileName;
    lResult.role = lComboBox->currentIndex();

    lResults.push_back(lResult);
  }

  return lResults;
}

void AssistedConversion::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>("launch_search_button")};
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};

  // Open a directory chooser dialog
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lLineEdit->text().size() > 0 ? lLineEdit->text() : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.compare("", Qt::CaseInsensitive) == 0};
  lLineEdit->setDisabled(lMustDisableButton);
  lLaunchSearchButton->setDisabled(lMustDisableButton);

  this->displayHintZone();
}

void AssistedConversion::launchSearchProcess()
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  auto lInputPath{this->findChild<QLineEdit*>("input_path_directory")->text()};

  // The root directory should at least contain an ESP, ESL or ESM file to be scanned.
  // If it does not contain one of the wanted files, ask the user to start or cancel the scan
  auto lFilesExtensions{QStringList()};
  lFilesExtensions.append("*.esl");
  lFilesExtensions.append("*.esm");
  lFilesExtensions.append("*.esp");

  auto lIsESPPresent{Utils::getNumberFilesByExtensions(lInputPath, lFilesExtensions) > 0};
  if (!lIsESPPresent)
  {
    QMessageBox lConfirmationBox(QMessageBox::Icon::Question,
                                 tr("No root file has been found"),
                                 tr("No ESL, ESM or ESP file has been found in the scanned directory. Do you still want to continue the scan?"),
                                 QMessageBox::StandardButton::NoButton,
                                 this);

    auto lContinueButton{lConfirmationBox.addButton(tr("Continue the scan"), QMessageBox::ButtonRole::YesRole)};
    lContinueButton->setCursor(Qt::PointingHandCursor);
    lContinueButton->setStyleSheet("color: hsl(141, 53%, 53%)");

    auto lStopButton{lConfirmationBox.addButton(tr("Cancel the scan"), QMessageBox::ButtonRole::NoRole)};
    lStopButton->setCursor(Qt::PointingHandCursor);
    lStopButton->setStyleSheet("color: hsl(4, 90%, 58%);");

    lConfirmationBox.setDefaultButton(lContinueButton);
    lConfirmationBox.exec();

    if (lConfirmationBox.clickedButton() != lContinueButton)
    {
      return;
    }
  }

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area chooser
  auto lMainLayout{this->findChild<QGridLayout*>("main_layout")};

  auto lScrollArea{new QScrollArea(this)};
  lScrollArea->setObjectName("scrollable_zone");
  lScrollArea->setWidgetResizable(true);

  auto lMainWidget{new QFrame(this)};

  auto lDataContainer = new QGridLayout(lMainWidget);
  lDataContainer->setObjectName("data_container");
  lDataContainer->setAlignment(Qt::AlignTop);
  lDataContainer->setContentsMargins(0, 0, 0, 0);

  lScrollArea->setWidget(lMainWidget);
  lMainLayout->addWidget(lScrollArea, 2, 0, 1, 3);

  // Columns header
  lDataContainer->addWidget(new QLabel(tr("File path"), this), 0, 0, Qt::AlignCenter);
  lDataContainer->addWidget(new QLabel(tr("*.nif file name"), this), 0, 1, Qt::AlignCenter);
  lDataContainer->addWidget(new QLabel(tr("Action"), this), 0, 2, Qt::AlignCenter);

  // Fetch all the "*.nif" files
  auto lFoundNifFiles{this->scanForFilesByExtension(lInputPath, "*.nif")};
  this->mScannedDirName = QDir(lInputPath).dirName();

  auto lNextRow{1};
  this->mBoxSelectedIndexes.clear();

  for (const auto& lNifFile : lFoundNifFiles)
  {
    this->createSelectionBlock(*lDataContainer, lNifFile.second.second, lNifFile.second.first, lNextRow++);
  }

  // Create the validation button
  auto lValidateSelection{new QPushButton(tr("Validate the selection(s) above and go back to the main window"), this)};
  lValidateSelection->setCursor(Qt::PointingHandCursor);
  lValidateSelection->setIcon(QIcon(QPixmap(QString(":/%1/playlist-check").arg(lIconFolder))));
  lValidateSelection->setObjectName("validate_selection");
  lMainLayout->addWidget(lValidateSelection, 3, 0, 1, 3);
  this->connect(lValidateSelection, &QPushButton::clicked, this, &AssistedConversion::validateSelection);
}

void AssistedConversion::validateSelection()
{
  emit valuesChosen(this->mScannedDirName, this->getChosenValuesFromInterface());
  this->close();
}

void AssistedConversion::modifyComboBoxLockState(int aIndex)
{
  // Catch the sender of the event
  auto lEventSource{qobject_cast<QComboBox*>(sender())};

  // Fetch the grid layout
  auto lDataContainer{this->findChild<QGridLayout*>("data_container")};
  auto lLinesToTreat{lDataContainer->rowCount()};
  QComboBox* lComboBox{nullptr};
  auto lComboBoxIndex{0};

  // For each row (skip the row 0 because it is a "header")
  for (int i = 1; i < lLinesToTreat; i++)
  {
    // Get the combobox
    lComboBox = qobject_cast<QComboBox*>(lDataContainer->itemAtPosition(i, 2)->widget());

    if (lComboBox == lEventSource)
    {
      lComboBoxIndex = i - 1;
      break;
    }
  }

  // Keep the old index and store the new selected index
  auto lOldIndex{this->mBoxSelectedIndexes.at(lComboBoxIndex)};
  this->mBoxSelectedIndexes.at(lComboBoxIndex) = aIndex;

  // For each row (skip the row 0 because it is a "header")
  for (int i = 1; i < lLinesToTreat; i++)
  {
    // Get the combobox
    lComboBox = qobject_cast<QComboBox*>(lDataContainer->itemAtPosition(i, 2)->widget());

    if (lComboBox == lEventSource)
    {
      continue;
    }

    QListView* view = qobject_cast<QListView*>(lComboBox->view());

    // Disable the new selected option
    if (aIndex != 0)
    {
      view->setRowHidden(aIndex, true);
    }

    // Enable the old option
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(lComboBox->model());
    QStandardItem* item = model->item(lOldIndex);
    view->setRowHidden(lOldIndex, false);
    item->setFlags(item->flags() | Qt::ItemIsEnabled);
  }
}
