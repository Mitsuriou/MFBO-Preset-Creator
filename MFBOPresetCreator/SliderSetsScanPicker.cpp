#include "SliderSetsScanPicker.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "FileIDPicker.h"
#include "SSSPSelectionBlock.h"
#include "Utils.h"
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QDomDocument>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QListView>
#include <QNetworkReply>
#include <QProgressBar>
#include <QProgressDialog>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

SliderSetsScanPicker::SliderSetsScanPicker(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, aSettings, aLastPaths)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.sliderSetsScanPickerDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void SliderSetsScanPicker::closeEvent(QCloseEvent* aEvent)
{
  // Catch the sender of the event
  auto lEventSource{qobject_cast<QPushButton*>(this->sender())};
  auto lValidationBtn{this->findChild<QPushButton*>(QString("validate_selection"))};

  if ((lEventSource == lValidationBtn) || !this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->settings().display.applicationTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the assisted conversion tool window"),
                                    "",
                                    this->settings().display.dangerColor,
                                    this->settings().display.successColor,
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

void SliderSetsScanPicker::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Assisted Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/pencil")));
}

void SliderSetsScanPicker::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->settings().display.applicationTheme)};

  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setRowStretch(2, 1); // Make the hint zone as high as possible
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // Input path label
  lMainLayout->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input path value
  auto lInputPathLineEdit{new QLineEdit(this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QString("input_path_directory"));
  lInputPathLineEdit->setDisabled(true);
  lMainLayout->addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "", false, true)};
  lMainLayout->addWidget(lInputPathChooser, 0, 2);

  // Launch search button
  auto lLaunchSearchButton{ComponentFactory::CreateButton(this, tr("Launch the scan of the mod"), "", "search", lIconFolder, "launch_search_button", true, true)};
  lMainLayout->addWidget(lLaunchSearchButton, 1, 0, 1, 3);

  // Hint zone
  this->displayHintZone();

  // Event binding
  QObject::connect(lInputPathChooser, &QPushButton::clicked, this, &SliderSetsScanPicker::chooseInputDirectory);
  QObject::connect(lLaunchSearchButton, &QPushButton::clicked, this, &SliderSetsScanPicker::launchSearchProcess);
}

void SliderSetsScanPicker::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setMinimumHeight(300);
  lHintZone->setObjectName(QString("hint_zone"));
  lHintZone->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lHintZone, 2, 0, 1, 3);
}

void SliderSetsScanPicker::deleteAlreadyExistingWindowBottom() const
{
  auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
  if (lHintZone)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lOldValidationButton{this->findChild<QPushButton*>(QString("validate_selection"))};
  if (lOldValidationButton)
  {
    delete lOldValidationButton;
    lOldValidationButton = nullptr;
  }

  auto lOldScrollArea{this->findChild<QScrollArea*>(QString("scrollable_zone"))};
  if (lOldScrollArea)
  {
    delete lOldScrollArea;
    lOldScrollArea = nullptr;
  }
}

void SliderSetsScanPicker::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::GetPathFromKey(this->lastPaths(), "sliderSetsScanPickerInput", lLineEdit->text(), this->settings().general.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->lastPaths(), "sliderSetsScanPickerInput", lPath);

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.compare("", Qt::CaseInsensitive) == 0};
  lLineEdit->setDisabled(lMustDisableButton);
  lLaunchSearchButton->setDisabled(lMustDisableButton);

  this->displayHintZone();
}

bool SliderSetsScanPicker::isFileNameRecognized(const QString& aFileName)
{
  return (aFileName.compare("skeleton_female", Qt::CaseInsensitive) == 0
          || aFileName.compare("skeletonbeast_female", Qt::CaseInsensitive) == 0
          || aFileName.compare("femalehands", Qt::CaseInsensitive) == 0
          || aFileName.compare("femalefeet", Qt::CaseInsensitive) == 0
          || aFileName.compare("femalebody", Qt::CaseInsensitive) == 0);
}

void SliderSetsScanPicker::launchSearchProcess()
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  lLaunchSearchButton->setDisabled(true);

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->settings().display.applicationTheme)};

  if (this->mHasUserDoneSomething)
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Relaunch the scan"),
                                      tr("You will lose all the unsaved data. Do you still want to relaunch the scan?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Relaunch the scan"),
                                      tr("Cancel the relaunch"),
                                      "",
                                      this->settings().display.dangerColor,
                                      this->settings().display.successColor,
                                      "",
                                      false)
        != ButtonClicked::YES)
    {
      lLaunchSearchButton->setDisabled(false);
      return;
    }
  }

  this->launchSearch();
}

void SliderSetsScanPicker::launchSearch()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->settings().display.applicationTheme)};

  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  const auto lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

  // The root directory should at least contain an ESP, ESL or ESM file to be scanned.
  if (Utils::GetNumberFilesByExtensions(lInputPath, QStringList({"*.esl", "*.esm", "*.esp"})) == 0)
  {
    // If it does not contain one of the wanted files, ask the user to start or cancel the scan
    if (Utils::DisplayQuestionMessage(this,
                                      tr("No mod archive file has been found"),
                                      tr("No ESL, ESM or ESP files were found in the scanned directory. Do you still want to continue the scan?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the scan"),
                                      tr("Cancel the scan"),
                                      "",
                                      this->settings().display.warningColor,
                                      this->settings().display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      lLaunchSearchButton->setDisabled(false);
      return;
    }
  }

  // Error: there is no CalienteTools folder
  auto lCheckPath{QString("%1%2%3").arg(lInputPath, QDir::separator(), "CalienteTools")};
  if (!QDir(lCheckPath).exists())
  {
    Utils::DisplayErrorMessage(tr("No CalienteTools directory was found in the scanned directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // Error: there is no BodySlide folder
  lCheckPath.append(QDir::separator()).append("BodySlide");
  if (!QDir(lCheckPath).exists())
  {
    Utils::DisplayErrorMessage(tr("No BodySlide directory was found in the CalienteTools directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // Error: there is no SliderSets folder
  lCheckPath.append(QDir::separator()).append("SliderSets");
  if (!QDir(lCheckPath).exists())
  {
    Utils::DisplayErrorMessage(tr("No SliderSets directory was found in the BodySlide directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // The root directory should at least contain an OSP file to be scanned.
  if (Utils::GetNumberFilesByExtensions(lCheckPath, QStringList({"*.osp"})) == 0)
  {
    Utils::DisplayErrorMessage(tr("No OSP file were found in the SliderSets directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // Fetch all the "*.nif" files
  Utils::CleanPathString(lCheckPath);
  this->displayObtainedData(this->scanForOspFiles(lCheckPath));
}

std::multimap<QString, std::vector<Struct::SliderSet>> SliderSetsScanPicker::scanForOspFiles(const QString& aRootDir) const
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

  QCoreApplication::processEvents();

  std::multimap<QString, std::vector<Struct::SliderSet>> lScannedValues;

  QDirIterator it(aRootDir, QStringList() << "*.osp", QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return std::multimap<QString, std::vector<Struct::SliderSet>>();
    }

    it.next();

    lScannedValues.insert({it.filePath(), Utils::ReadOSPFileInformation(it.filePath(), true)});
  }

  return lScannedValues;
}

void SliderSetsScanPicker::displayObtainedData(const std::multimap<QString, std::vector<Struct::SliderSet>>& aFoundOspFiles)
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  lLaunchSearchButton->setDisabled(false);

  // No file found
  if (aFoundOspFiles.empty())
  {
    this->displayHintZone();
    auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
    if (lHintZone)
    {
      lHintZone->setText(tr("No NIF file was found in the scanned directory."));
    }

    this->mHasUserDoneSomething = false;
    return;
  }

  this->mHasUserDoneSomething = true;

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area chooser
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};
  auto lDataContainer{ComponentFactory::CreateScrollAreaComponentLayout(this, *lMainLayout, 2, 0, 1, 3)};

  auto lNextRow{0};

  for (const auto& lOspFile : aFoundOspFiles)
  {
    for (const auto& lSliderSet : lOspFile.second)
    {
      lDataContainer->addWidget(new SSSPSelectionBlock(this, lSliderSet), lNextRow++, 0);
    }
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->settings().display.applicationTheme)};

  // Create the validation button
  auto lValidateSelection{ComponentFactory::CreateButton(this, tr("Import the slider sets(s) and close this window"), "", "playlist-check", lIconFolder, "validate_selection")};
  lMainLayout->addWidget(lValidateSelection, 3, 0, 1, 3);

  QObject::connect(lValidateSelection, &QPushButton::clicked, this, &SliderSetsScanPicker::validateSelection);
}

std::vector<Struct::AssistedConversionResult> SliderSetsScanPicker::getChosenValuesFromInterface() const
{
  // Fetch the grid layout
  auto lDataContainer{this->findChild<QGridLayout*>(QString("data_container"))};

  // Iterate in the layout
  auto lLinesToTreat{lDataContainer->rowCount()};

  if (lLinesToTreat <= 1)
  {
    return std::vector<Struct::AssistedConversionResult>();
  }

  std::vector<Struct::AssistedConversionResult> lResults;
  QComboBox* lComboBox{nullptr};
  QString lFilePath;
  QString lFileName;

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
    if (lFilePath.startsWith("<span"))
    {
      // Parse the inner text of the XML span tag
      QDomDocument lDoc;
      lDoc.setContent(lFilePath);
      const auto lElementToParse{lDoc.documentElement()};

      lFilePath = lElementToParse.text();
    }

    // Second column is the file name
    lFileName = qobject_cast<QLabel*>(lDataContainer->itemAtPosition(i, 1)->widget())->text();
    if (lFileName.startsWith("<span"))
    {
      // Parse the inner text of the XML span tag
      QDomDocument lDoc;
      lDoc.setContent(lFileName);
      const auto lElementToParse{lDoc.documentElement()};

      lFileName = lElementToParse.text();
    }

    // Save the gotten values
    lResults.push_back(Struct::AssistedConversionResult(lFilePath, lFileName, lComboBox->currentIndex()));
  }

  return lResults;
}

void SliderSetsScanPicker::validateSelection()
{
  auto lValues{this->getChosenValuesFromInterface()};

  if (lValues.empty())
  {
    // If not any file as been tagged, ask the user to continue or reselect values
    if (Utils::DisplayQuestionMessage(this,
                                      tr("No entry selected"),
                                      tr("You did not select any entry. Do you still want to validate this selection as is?"),
                                      Utils::GetIconResourceFolder(this->settings().display.applicationTheme),
                                      "help-circle",
                                      tr("Validate as is"),
                                      tr("Cancel, I wanted to select values"),
                                      "",
                                      this->settings().display.dangerColor,
                                      this->settings().display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  // emit valuesChosen(); // TODO
  this->close();
}
