#include "SliderSetsImporter.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "SSSPSelectionBlock.h"
#include "Utils.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDirIterator>
#include <QFileDialog>
#include <QProgressBar>
#include <QProgressDialog>
#include <QScrollArea>
#include <QStyledItemDelegate>

SliderSetsImporter::SliderSetsImporter(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, tr("Slider Sets Importer"), "publish", aSettings, aLastPaths, 700)
{
  // Build the window's interface
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.sliderSetsImporterDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void SliderSetsImporter::closeEvent(QCloseEvent* aEvent)
{
  // Catch the sender of the event
  const auto lEventSource{qobject_cast<QPushButton*>(this->sender())};
  const auto lValidationBtn{this->findChild<QPushButton*>(QStringLiteral("validate_selection"))};

  if ((lEventSource == lValidationBtn) || !this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    this->getThemedResourcePath(),
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the slider sets importer window"),
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

void SliderSetsImporter::initializeGUI()
{
  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setRowStretch(2, 1); // Make the hint zone as high as possible
  lMainLayout->setAlignment(Qt::AlignTop);
  this->getCentralWidget()->setLayout(lMainLayout);

  // Input path label
  lMainLayout->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input path value
  auto lInputPathLineEdit{new LineEdit(this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QStringLiteral("input_path_directory"));
  lInputPathLineEdit->setDisabled(true);
  lMainLayout->addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::CreateButton(this,
                                                        tr("Choose a directory..."),
                                                        "",
                                                        "folder",
                                                        this->getThemedResourcePath(),
                                                        "",
                                                        false,
                                                        true)};
  lMainLayout->addWidget(lInputPathChooser, 0, 2);

  // Launch search button
  auto lLaunchSearchButton{ComponentFactory::CreateButton(this,
                                                          tr("Launch the scan of the mod"),
                                                          "",
                                                          "search",
                                                          this->getThemedResourcePath(),
                                                          "launch_search_button",
                                                          true,
                                                          true)};
  lMainLayout->addWidget(lLaunchSearchButton, 1, 0, 1, 3);

  // Hint zone
  this->displayHintZone();

  // Event binding
  QObject::connect(lInputPathChooser, &QPushButton::clicked, this, &SliderSetsImporter::chooseInputDirectory);
  QObject::connect(lLaunchSearchButton, &QPushButton::clicked, this, &SliderSetsImporter::launchSearchProcess);
}

void SliderSetsImporter::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->getCentralLayout())};
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setMinimumHeight(300);
  lHintZone->setObjectName(QStringLiteral("hint_zone"));
  lHintZone->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lHintZone, 2, 0, 1, 3);
}

void SliderSetsImporter::deleteAlreadyExistingWindowBottom() const
{
  auto lHintZone{this->findChild<QLabel*>(QStringLiteral("hint_zone"))};
  if (lHintZone)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lOldValidationButton{this->findChild<QPushButton*>(QStringLiteral("validate_selection"))};
  if (lOldValidationButton)
  {
    delete lOldValidationButton;
    lOldValidationButton = nullptr;
  }

  auto lOldScrollArea{this->findChild<QScrollArea*>(QStringLiteral("scrollable_zone"))};
  if (lOldScrollArea)
  {
    delete lOldScrollArea;
    lOldScrollArea = nullptr;
  }
}

void SliderSetsImporter::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>(QStringLiteral("launch_search_button"))};
  auto lLineEdit{this->findChild<QLineEdit*>(QStringLiteral("input_path_directory"))};

  // Open a directory chooser dialog
  const auto lContextPath{Utils::GetPathFromKey(this->lastPaths(),
                                                QStringLiteral("sliderSetsImporterInput"),
                                                lLineEdit->text(),
                                                this->settings().general.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->lastPaths(), "sliderSetsImporterInput", lPath);

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.isEmpty()};
  lLineEdit->setDisabled(lMustDisableButton);
  lLaunchSearchButton->setDisabled(lMustDisableButton);

  this->displayHintZone();
}

bool SliderSetsImporter::isFileNameRecognized(const QString& aFileName)
{
  return (aFileName.compare(QStringLiteral("skeleton_female"), Qt::CaseSensitivity::CaseInsensitive) == 0
          || aFileName.compare(QStringLiteral("skeletonbeast_female"), Qt::CaseSensitivity::CaseInsensitive) == 0
          || aFileName.compare(QStringLiteral("femalehands"), Qt::CaseSensitivity::CaseInsensitive) == 0
          || aFileName.compare(QStringLiteral("femalefeet"), Qt::CaseSensitivity::CaseInsensitive) == 0
          || aFileName.compare(QStringLiteral("femalebody"), Qt::CaseSensitivity::CaseInsensitive) == 0);
}

void SliderSetsImporter::launchSearchProcess()
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QStringLiteral("launch_search_button"))};
  lLaunchSearchButton->setDisabled(true);

  if (this->mHasUserDoneSomething)
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Relaunch the scan"),
                                      tr("You will lose all the unsaved data. Do you still want to relaunch the scan?"),
                                      this->getThemedResourcePath(),
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

void SliderSetsImporter::launchSearch()
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QStringLiteral("launch_search_button"))};
  const auto lInputPath{this->findChild<QLineEdit*>(QStringLiteral("input_path_directory"))->text()};

  // The root directory should at least contain an ESP, ESL or ESM file to be scanned.
  if (Utils::GetNumberFilesByExtensions(lInputPath, QStringList({QStringLiteral("*.esl"), QStringLiteral("*.esm"), QStringLiteral("*.esp")})) == 0)
  {
    // If it does not contain one of the wanted files, ask the user to start or cancel the scan
    if (Utils::DisplayQuestionMessage(this,
                                      tr("No mod archive file has been found"),
                                      tr("No ESL, ESM or ESP files were found in the scanned directory. Do you still want to continue the scan?"),
                                      this->getThemedResourcePath(),
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
    Utils::DisplayErrorMessage(tr("No \"CalienteTools\" directory was found in the scanned directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // Error: there is no BodySlide folder
  lCheckPath.append(QDir::separator()).append("BodySlide");
  if (!QDir(lCheckPath).exists())
  {
    Utils::DisplayErrorMessage(tr("No \"BodySlide\" directory was found in the CalienteTools directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // Error: there is no SliderSets folder
  lCheckPath.append(QDir::separator()).append("SliderSets");
  if (!QDir(lCheckPath).exists())
  {
    Utils::DisplayErrorMessage(tr("No \"SliderSets\" directory was found in the BodySlide directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // The root directory should at least contain an OSP file to be scanned.
  if (Utils::GetNumberFilesByExtensions(lCheckPath, QStringList({QStringLiteral("*.osp")})) == 0)
  {
    Utils::DisplayErrorMessage(tr("No OSP file were found in the \"SliderSets\" directory."));
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  // Fetch all the "*.nif" files
  Utils::CleanPathString(lCheckPath);
  this->displayObtainedData(this->scanForOspFilesData(lCheckPath));
}

std::multimap<QString, std::vector<Struct::SliderSet>> SliderSetsImporter::scanForOspFilesData(const QString& aRootDir) const
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

  QDirIterator it(aRootDir, QStringList(QStringLiteral("*.osp")), QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return std::multimap<QString, std::vector<Struct::SliderSet>>();
    }

    it.next();

    lScannedValues.insert(std::make_pair(it.filePath(), Utils::ReadOSPFileInformation(it.filePath(), true)));
  }

  return lScannedValues;
}

void SliderSetsImporter::displayObtainedData(const std::multimap<QString, std::vector<Struct::SliderSet>>& aFoundOspFiles)
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QStringLiteral("launch_search_button"))};
  lLaunchSearchButton->setDisabled(false);

  // No file found
  if (aFoundOspFiles.empty())
  {
    this->displayHintZone();
    auto lHintZone{this->findChild<QLabel*>(QStringLiteral("hint_zone"))};
    if (lHintZone)
    {
      lHintZone->setText(tr("No OSP file were found in the \"SliderSets\" directory."));
    }

    this->mHasUserDoneSomething = false;
    return;
  }

  this->mHasUserDoneSomething = true;

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area chooser
  auto lMainLayout{qobject_cast<QGridLayout*>(this->getCentralLayout())};
  auto lDataContainer{ComponentFactory::CreateScrollAreaComponentLayout(this, *lMainLayout, 2, 0, 1, 3)};

  auto lNextRow{0};

  for (const auto& lOspFile : aFoundOspFiles)
  {
    for (const auto& lSliderSet : lOspFile.second)
    {
      lDataContainer->addWidget(new SSSPSelectionBlock(this,
                                                       this->settings().display.applicationTheme,
                                                       this->settings().display.font.pointSize,
                                                       lOspFile.first,
                                                       lSliderSet),
                                lNextRow++,
                                0);
    }
  }

  // Create the validation button
  auto lValidateSelection{ComponentFactory::CreateButton(this,
                                                         tr("Start importing the chosen slider sets(s) and close this window"),
                                                         "",
                                                         "playlist-check",
                                                         this->getThemedResourcePath(),
                                                         "validate_selection")};
  lMainLayout->addWidget(lValidateSelection, 3, 0, 1, 3);

  QObject::connect(lValidateSelection, &QPushButton::clicked, this, &SliderSetsImporter::validateSelection);
}

std::vector<Struct::SliderSetResult> SliderSetsImporter::getChosenValuesFromInterface() const
{
  // Fetch the grid layout
  auto lDataContainer{this->findChild<QGridLayout*>(QStringLiteral("data_container"))};

  // Iterate in the layout
  auto lLinesToTreat{lDataContainer->rowCount()};

  if (lLinesToTreat < 1)
  {
    return std::vector<Struct::SliderSetResult>();
  }

  std::vector<Struct::SliderSetResult> lResults;
  SSSPSelectionBlock* lSSPSBlock{nullptr};

  // For each row (skip the row 0 because it is a "header")
  for (int i = 0; i < lLinesToTreat; i++)
  {
    lSSPSBlock = qobject_cast<SSSPSelectionBlock*>(lDataContainer->itemAtPosition(i, 0)->widget());

    if (!lSSPSBlock->isCheckedForImport() || lSSPSBlock->getCurrentlySetMeshPartType() == MeshPartType::UNKNOWN)
    {
      continue;
    }

    // Save the gotten values
    lResults.push_back(lSSPSBlock->getData());
  }

  return lResults;
}

void SliderSetsImporter::validateSelection()
{
  const auto lValues{this->getChosenValuesFromInterface()};

  if (lValues.empty())
  {
    // If not any file as been tagged, ask the user to continue or reselect values
    if (Utils::DisplayQuestionMessage(this,
                                      tr("No entry selected"),
                                      tr("You did not select any entry. Do you still want to validate this selection as is?"),
                                      this->getThemedResourcePath(),
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

  emit valuesChosen(lValues);
  this->close();
}
