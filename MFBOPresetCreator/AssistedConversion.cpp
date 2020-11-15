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
  this->setupInterface(*lMainGrid);
}

void AssistedConversion::setupInterface(QGridLayout& aLayout)
{
  // First line
  auto lInputPathLabel{new QLabel(tr("Input path:"), this)};
  aLayout.addWidget(lInputPathLabel, 0, 0);

  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  aLayout.addWidget(lInputPathLineEdit, 0, 1);

  auto lInputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lInputPathChooser->setCursor(Qt::PointingHandCursor);
  lInputPathChooser->setAutoDefault(false);
  lInputPathChooser->setDefault(false);
  aLayout.addWidget(lInputPathChooser, 0, 2);

  // Generate button
  auto lLaunchSearchButton{new QPushButton(tr("[WIP] Launch the files search"), this)};
  lLaunchSearchButton->setObjectName("launch_search_button");
  lLaunchSearchButton->setCursor(Qt::PointingHandCursor);
  lLaunchSearchButton->setAutoDefault(false);
  lLaunchSearchButton->setDefault(false);
  lLaunchSearchButton->setDisabled(true);
  aLayout.addWidget(lLaunchSearchButton, 1, 0, 1, 3, Qt::AlignBottom);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &AssistedConversion::chooseInputDirectory);
  this->connect(lLaunchSearchButton, &QPushButton::clicked, this, &AssistedConversion::launchSearchProcess);
}

std::vector<std::pair<QString, QString>> AssistedConversion::scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension)
{
  // vector<pair<file name, path>>
  std::vector<std::pair<QString, QString>> lScannedValues;

  auto lAbsFilePath{QString("")};
  auto lRelativeDirPath{QString("")};
  auto lFileName{QString("")};

  QDirIterator it(aRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Get the current directory
    lAbsFilePath = it.fileInfo().absolutePath();
    lRelativeDirPath = lAbsFilePath.remove(aRootDir, Qt::CaseInsensitive);

    lScannedValues.push_back(std::make_pair(it.fileInfo().fileName(), lRelativeDirPath));
  }

  return lScannedValues;
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
}

void AssistedConversion::launchSearchProcess()
{
  auto lInputPath{this->findChild<QLineEdit*>("input_path_directory")->text()};
  auto lFoundNifFiles{this->scanForFilesByExtension(lInputPath, "*.nif")};

  // Create the scroll area chooser
  auto lOldScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  if (lOldScrollArea)
  {
    delete lOldScrollArea;
    lOldScrollArea = nullptr;
  }

  auto lMainLayout{this->findChild<QGridLayout*>("main_layout")};

  auto lScrollArea{new QScrollArea(this)};
  lScrollArea->setObjectName("scrollable_zone");
  lScrollArea->setWidgetResizable(true);

  auto lMainWidget{new QFrame(this)};

  auto lVerticalContentContainer = new QGridLayout(lMainWidget);
  lVerticalContentContainer->setContentsMargins(0, 0, 0, 0);

  lScrollArea->setWidget(lMainWidget);
  lMainLayout->addWidget(lScrollArea, 2, 0, 1, 3);

  // Columns header
  lVerticalContentContainer->addWidget(new QLabel(tr("File path"), this), 0, 0, Qt::AlignLeft);
  lVerticalContentContainer->addWidget(new QLabel(tr("*.nif file name"), this), 0, 1, Qt::AlignLeft);
  lVerticalContentContainer->addWidget(new QLabel(tr("Action"), this), 0, 2, Qt::AlignRight);

  int lNextRow{1};
  for (const auto& lNifFile : lFoundNifFiles)
  {
    this->createSelectionBlock(*lVerticalContentContainer, lNifFile.first, lNifFile.second, lNextRow++);
  }

  // Create validate button
}

void AssistedConversion::createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int& aRowIndex)
{
  aLayout.addWidget(new QLabel(aPath + "/", this), aRowIndex, 0, Qt::AlignLeft);
  aLayout.addWidget(new QLabel(aFileName, this), aRowIndex, 1, Qt::AlignLeft);

  auto lChoiceCombo{new QComboBox(this)};
  lChoiceCombo->addItems(DataLists::getAssistedConversionActions());
  aLayout.addWidget(lChoiceCombo, aRowIndex, 2, Qt::AlignRight);
}
