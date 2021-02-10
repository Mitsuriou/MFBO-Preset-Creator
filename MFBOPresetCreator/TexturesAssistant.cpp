#include "TexturesAssistant.h"

TexturesAssistant::TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mHasUserDoneSomething(false)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void TexturesAssistant::closeEvent(QCloseEvent* aEvent)
{
  // Catch the sender of the event
  auto lEventSource{qobject_cast<QPushButton*>(sender())};
  auto lValidationBtn{this->findChild<QPushButton*>("validate_selection")};

  if ((lEventSource && lValidationBtn) || !this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  QMessageBox lBox(QMessageBox::Icon::Question, tr("Closing"), tr("Do you want to close the window?"), QMessageBox::StandardButton::NoButton, this);
  lBox.setIconPixmap(QPixmap(QString(":/%1/help-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

  auto lCloseBtn{lBox.addButton(tr("Close the window"), QMessageBox::ButtonRole::YesRole)};
  lCloseBtn->setCursor(Qt::PointingHandCursor);
  lCloseBtn->setStyleSheet(QString("color: %1;").arg(this->mSettings.dangerColor));

  auto lStayBtn{lBox.addButton(tr("Go back to the textures assistant window"), QMessageBox::ButtonRole::NoRole)};
  lStayBtn->setCursor(Qt::PointingHandCursor);
  lStayBtn->setStyleSheet(QString("color: %1;").arg(this->mSettings.successColor));

  lBox.setDefaultButton(lStayBtn);
  lBox.exec();

  if (lBox.clickedButton() != lCloseBtn)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
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
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Textures Assistant"));
  this->setWindowIcon(QIcon(QPixmap(":/black/textures")));
}

void TexturesAssistant::initializeGUI()
{
  // Main window layout
  auto lMainGrid{new QGridLayout(this)};
  lMainGrid->setAlignment(Qt::AlignTop);
  this->setLayout(lMainGrid);

  this->setupInterface();
}

void TexturesAssistant::setupInterface()
{
  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // First line
  auto lInputPathLabel{new QLabel(tr("Input path:"), this)};
  lMainLayout->addWidget(lInputPathLabel, 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  lMainLayout->addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lInputPathChooser->setCursor(Qt::PointingHandCursor);
  lInputPathChooser->setIcon(QIcon(QPixmap(QString(":/%1/folder").arg(lIconFolder))));
  lInputPathChooser->setAutoDefault(false);
  lInputPathChooser->setDefault(false);
  lMainLayout->addWidget(lInputPathChooser, 0, 2);

  // Launch search button
  auto lLaunchSearchButton{new QPushButton(tr("Launch the scan of the mod"), this)};
  lLaunchSearchButton->setCursor(Qt::PointingHandCursor);
  lLaunchSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/search").arg(lIconFolder))));
  lLaunchSearchButton->setObjectName("launch_search_button");
  lLaunchSearchButton->setAutoDefault(false);
  lLaunchSearchButton->setDefault(false);
  lLaunchSearchButton->setDisabled(true);
  lMainLayout->addWidget(lLaunchSearchButton, 1, 0, 1, 3, Qt::AlignTop);

  // Hint zone
  this->displayHintZone();

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &TexturesAssistant::chooseInputDirectory);
  this->connect(lLaunchSearchButton, &QPushButton::clicked, this, &TexturesAssistant::launchSearchProcess);
}

void TexturesAssistant::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setMinimumHeight(300);
  lHintZone->setObjectName("hint_zone");
  lHintZone->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lHintZone, 2, 0, 1, 3);
}

void TexturesAssistant::deleteAlreadyExistingWindowBottom() const
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

std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> TexturesAssistant::scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const
{
  // The map is storing <path+fileName, <path, fileName>>
  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> lScannedValues;

  auto lRelativeDirPath{QString()};
  auto lFileName{QString()};
  auto lKey{std::string()};
  auto lFirstValue{QString()};

  auto lRootDir{aRootDir + QString("/textures")};

  QDirIterator it(lRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Get the current directory
    lRelativeDirPath = it.fileInfo().absolutePath().remove(aRootDir + "/", Qt::CaseInsensitive);

    lFileName = it.fileInfo().fileName();

    // Clean the file name from any artifact
    lFileName.remove("_0.nif", Qt::CaseInsensitive);
    lFileName.remove("_1.nif", Qt::CaseInsensitive);
    lFileName.remove(".nif", Qt::CaseInsensitive);

    // Construct the key of the map
    lKey = lRelativeDirPath.toStdString() + "/textures/" + lFileName.toStdString();

    // Construct the value of the map
    lFirstValue = lRelativeDirPath;
    lKey.append("/textures");

    auto lValue{std::make_pair(lFirstValue, lFileName)};

    // Insert the key-value into the map
    lScannedValues.insert(std::make_pair(lKey, lValue));
  }

  return lScannedValues;
}

void TexturesAssistant::createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int& aRowIndex)
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
  this->connect(lChoiceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TexturesAssistant::modifyComboBoxLockState);
}

std::vector<Struct::AssistedConversionResult> TexturesAssistant::getChosenValuesFromInterface() const
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

void TexturesAssistant::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>("launch_search_button")};
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::getPathFromKey(this->mLastPaths, "assistedConversionInput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::updatePathAtKey(this->mLastPaths, "assistedConversionInput", lPath);

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
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  auto lInputPath{this->findChild<QLineEdit*>("input_path_directory")->text()};

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area chooser
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};

  auto lScrollArea{new QScrollArea(this)};
  lScrollArea->setObjectName("scrollable_zone");
  lScrollArea->setWidgetResizable(true);

  auto lMainWidget{new QFrame(this)};

  auto lDataContainer{new QGridLayout(lMainWidget)};
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
  auto lFoundNifFiles{this->scanForFilesByExtension(lInputPath, "*.dds")};
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
  this->connect(lValidateSelection, &QPushButton::clicked, this, &TexturesAssistant::validateSelection);
}

void TexturesAssistant::validateSelection()
{
  emit valuesChosen(this->mScannedDirName, this->getChosenValuesFromInterface());
  this->close();
}

void TexturesAssistant::modifyComboBoxLockState(int aIndex)
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

  // Check if an option can be set before actually setting it.
  // This will prevent the mouse scrolling bug that permits the user
  // to select a hidden option from the combobox.
  if (aIndex != 0)
  {
    auto lStep{1};
    if (aIndex < this->mBoxSelectedIndexes.at(lComboBoxIndex))
    {
      // If the user took an option that is placed first
      lStep = -1;
    }

    auto lSourceView{qobject_cast<QListView*>(lEventSource->view())};

    auto lTestIndex{aIndex};
    while (lSourceView->isRowHidden(lTestIndex) && lTestIndex <= lEventSource->count())
    {
      lTestIndex += lStep;
    }

    this->disconnect(lEventSource, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TexturesAssistant::modifyComboBoxLockState);

    if (lTestIndex == lEventSource->count())
    {
      lEventSource->setCurrentIndex(this->mBoxSelectedIndexes.at(lComboBoxIndex));
      this->connect(lEventSource, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TexturesAssistant::modifyComboBoxLockState);
      return;
    }

    lEventSource->setCurrentIndex(lTestIndex);
    aIndex = lTestIndex;
    this->connect(lEventSource, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TexturesAssistant::modifyComboBoxLockState);
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

    auto lView{qobject_cast<QListView*>(lComboBox->view())};

    // Disable the new selected option
    if (aIndex != 0)
    {
      lView->setRowHidden(aIndex, true);
    }

    // Enable the old option
    auto lModel{qobject_cast<QStandardItemModel*>(lComboBox->model())};
    auto lItem{lModel->item(lOldIndex)};
    lView->setRowHidden(lOldIndex, false);
    lItem->setFlags(lItem->flags() | Qt::ItemIsEnabled);
  }
}
