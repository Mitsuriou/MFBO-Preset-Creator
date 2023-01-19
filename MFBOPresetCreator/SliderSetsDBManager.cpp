#include "SliderSetsDBManager.h"
#include "ComponentFactory.h"
#include "PlainTextEdit.h"
#include "SliderSetsDBEntry.h"
#include "SliderSetsImporter.h"
#include "Utils.h"
#include <QApplication>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QProgressBar>
#include <QProgressDialog>
#include <QSplitter>

constexpr int XML_INDENT = 4;

SliderSetsDBManager::SliderSetsDBManager(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, tr("Slider Sets Database Manager"), "database", aSettings, aLastPaths, 1200, 700)
{
  // Build the window's interface
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.sliderSetsDBManagerDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void SliderSetsDBManager::closeEvent(QCloseEvent*)
{
  // TODO: check to remove this call
  SliderSetsDBDAO::saveDatabase(this->mDatabase);
}

void SliderSetsDBManager::initializeGUI()
{
  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this->getCentralWidget(), true, false)};

  // Splitter
  const auto lSplitter{new QSplitter(Qt::Horizontal, this)};
  lSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lMainLayout->addWidget(lSplitter, 0, 0);

  //-----------//
  // Left side //
  //-----------//

  // Wrapper
  const auto lLeftWrapper{new QWidget(this)};
  const auto lLeftWrapperLayout{new QGridLayout(this)};
  lLeftWrapperLayout->setSpacing(10);
  lLeftWrapperLayout->setContentsMargins(0, 0, 10, 0);
  lLeftWrapper->setLayout(lLeftWrapperLayout);

  // Scroll area
  ComponentFactory::CreateScrollAreaComponentLayout(lLeftWrapper, *lLeftWrapperLayout, 0, 0);

  // "Add new slider sets" button
  const auto lButton{ComponentFactory::CreateButton(this, "Import new slider sets", "", "plus", this->getThemedResourcePath())};
  lLeftWrapperLayout->addWidget(lButton);

  //------------//
  // Right side //
  //------------//

  // Wrapper
  const auto lRightWrapper{new QWidget(this)};
  const auto lRightWrapperLayout{new QVBoxLayout(this)};
  lRightWrapperLayout->setSpacing(10);
  lRightWrapperLayout->setContentsMargins(10, 0, 0, 0);
  lRightWrapper->setLayout(lRightWrapperLayout);

  // Plain text viewer
  const auto lFileViewer{new PlainTextEdit(this)};
  lFileViewer->setObjectName(QStringLiteral("file_content_viewer"));
  lFileViewer->setReadOnly(true);
  lFileViewer->setLineWrapMode(QPlainTextEdit::NoWrap);
  lFileViewer->setWordWrapMode(QTextOption::NoWrap);
  lRightWrapperLayout->addWidget(lFileViewer);

  // Displayed file's path
  const auto lDisplayedFilePath{new QLabel(this)};
  lDisplayedFilePath->setObjectName(QStringLiteral("file_path"));
  lRightWrapperLayout->addWidget(lDisplayedFilePath);

  //
  lSplitter->addWidget(lLeftWrapper);
  lSplitter->addWidget(lRightWrapper);

  //
  QObject::connect(lButton, &QPushButton::clicked, this, &SliderSetsDBManager::openSliderSetsImporter);

  // Post-bind initialization functions
  this->refreshList();
  this->clearPreviewContent();
}

void SliderSetsDBManager::refreshList()
{
  const auto lDataContainer{this->findChild<QGridLayout*>("data_container")};

  auto lRow{0};

  for (const auto& lEntry : this->mDatabase)
  {
    if (!lEntry.second.isActive())
      continue;

    const auto lEntryWidget{new SliderSetsDBEntry(this,
                                                  this->settings().display.applicationTheme,
                                                  this->settings().display.font.pointSize,
                                                  lEntry.first,
                                                  lEntry.second)};

    QObject::connect(lEntryWidget, &SliderSetsDBEntry::nameUpdated, this, &SliderSetsDBManager::updateSliderSetName);
    QObject::connect(lEntryWidget, &SliderSetsDBEntry::meshTypeUpdated, this, &SliderSetsDBManager::updateSliderSetMeshType);
    QObject::connect(lEntryWidget, &SliderSetsDBEntry::deleteButtonClicked, this, &SliderSetsDBManager::removeFromDatabase);
    QObject::connect(lEntryWidget, &SliderSetsDBEntry::viewContentButtonClicked, this, &SliderSetsDBManager::displaySliderSetContent);

    lDataContainer->addWidget(lEntryWidget, lRow++, 0);
  }
}

void SliderSetsDBManager::openSliderSetsImporter()
{
  const auto lSliderSetsImporter{new SliderSetsImporter(this, this->settings(), this->lastPaths())};
  QObject::connect(lSliderSetsImporter, &SliderSetsImporter::valuesChosen, this, &SliderSetsDBManager::importNewSliderSets);
}

void SliderSetsDBManager::importNewSliderSets(const std::vector<Struct::SliderSetResult>& aChosenSliderSets)
{
  std::map<QString, std::vector<const Struct::SliderSetResult*>> lPaths;
  auto lEntriesCount{0};

  // Iterate over the results
  for (const auto& lChosenSliderSet : aChosenSliderSets)
  {
    // Filter the paths to know which file to open and parse
    lPaths[lChosenSliderSet.getFilePath()].push_back(&lChosenSliderSet);
    lEntriesCount++;
  }

  // Progress bar
  const auto lProgressBar{new QProgressBar(this)};
  lProgressBar->setFormat("%v / %m");
  lProgressBar->setRange(0, lEntriesCount);
  lProgressBar->setValue(0);
  lProgressBar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Parsing XML files. Please wait..."), tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressBar);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  QCoreApplication::processEvents();

  auto lTreatedFiles{0};

  // Start the scan
  for (const auto& lEntry : lPaths)
  {
    QFile lReadFile(lEntry.first);
    QDomDocument lOSPDocument;
    if (!lReadFile.open(QFile::ReadOnly | QFile::Text))
    {
      // Display a message in case the file cannot be found
      Utils::DisplayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(lEntry.first));

      lTreatedFiles += static_cast<int>(lEntry.second.size()); // Skip all the entries coming from this file
      lProgressDialog.setValue(lTreatedFiles);
      QCoreApplication::processEvents();
      continue;
    }

    lOSPDocument.setContent(&lReadFile);
    lReadFile.close();

    const auto lRoot{lOSPDocument.documentElement()};
    auto lNextNodeToParse{lRoot.firstChild().toElement()};

    while (!lNextNodeToParse.isNull())
    {
      if (lNextNodeToParse.tagName().compare(QStringLiteral("SliderSet"), Qt::CaseSensitivity::CaseInsensitive) == 0)
      {
        const auto lSliderSetName{lNextNodeToParse.attribute("name", "")};

        // Check if a SliderSet chosen by the user has the same name than the current one
        for (const auto& lSliderSet : lEntry.second)
        {
          if (lSliderSetName == lSliderSet->getOriginalSliderSetName())
          {
            this->saveSliderSetToDatabase(lNextNodeToParse,
                                          Struct::DatabaseSliderSet(true, lSliderSet->getNewSliderSetName(), lSliderSet->getMeshType()));
            break;
          }

          lProgressDialog.setValue(++lTreatedFiles);
          QCoreApplication::processEvents();
        }
      }

      lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
    }
  }

  // Refresh the displayed slider sets
  this->refreshList();
}

bool SliderSetsDBManager::saveSliderSetToDatabase(QDomElement& aSliderSetNode, const Struct::DatabaseSliderSet& aSliderSetData, const int aIndex)
{
  // Update the name with the new one given by the user
  aSliderSetNode.setAttribute("name", aSliderSetData.getSliderSetName());

  QString lXMLContent;
  QTextStream lStream(&lXMLContent, QIODevice::WriteOnly);
  aSliderSetNode.save(lStream, XML_INDENT);

  const auto lSliderSetIndex{aIndex == -1 ? this->nextAvailableDatabaseIndex() : aIndex};

  // Create the XML file on disk
  const auto lAbsFilePath{Utils::GetSliderSetsFolderPath().append(QString::number(lSliderSetIndex)).append(QStringLiteral(".osp"))};

  QFile lOSPFile(lAbsFilePath);
  if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    QTextStream lTextStream(&lOSPFile);
    lTextStream << lXMLContent;
    lTextStream.flush();

    lOSPFile.close();

    this->addDatabaseLine(lSliderSetIndex, aSliderSetData);

    return true;
  }

  Utils::DisplayWarningMessage(tr("Error while trying to create the SliderSet file \"%1\". Aborting process.").arg(lAbsFilePath));
  return false;
}

void SliderSetsDBManager::addDatabaseLine(const int aIndex, const Struct::DatabaseSliderSet& aSliderSetData)
{
  auto lPosition{this->mDatabase.find(aIndex)};

  // If the index is not already in the database, add it
  if (lPosition == this->mDatabase.end())
  {
    this->mDatabase.insert(std::make_pair(aIndex, aSliderSetData));
  }

  SliderSetsDBDAO::saveDatabase(this->mDatabase);
}

void SliderSetsDBManager::updateSliderSetName(const int aIndex, const QString& aNewSliderSetName)
{
  const auto lPosition{this->mDatabase.find(aIndex)};

  if (lPosition != this->mDatabase.end())
  {
    // Update the OSP file's content
    QFile lReadFile(Utils::GetSliderSetsFolderPath().append(QString::number(aIndex)).append(QStringLiteral(".osp")));
    if (lReadFile.open(QFile::ReadOnly | QFile::Text))
    {
      QDomDocument lOSPDocument;
      lOSPDocument.setContent(&lReadFile);

      // Update the database entry before saving it
      lPosition->second.setSliderSetName(aNewSliderSetName);

      auto lNode{lOSPDocument.firstChildElement()};
      this->saveSliderSetToDatabase(lNode, Struct::DatabaseSliderSet(true, aNewSliderSetName, lPosition->second.getMeshType()), aIndex);

      // Refresh the file's content viewer if it is displaying the current preset already
      if (this->mCurrentPreviewIndex == aIndex)
        this->displaySliderSetContent(aIndex);
    }

    lReadFile.close();
  }
}

void SliderSetsDBManager::updateSliderSetMeshType(const int aIndex, const MeshPartType aMeshType)
{
  const auto lPosition{this->mDatabase.find(aIndex)};

  if (lPosition != this->mDatabase.end())
    lPosition->second.setMeshType(aMeshType);
}

void SliderSetsDBManager::removeFromDatabase(const int aIndex)
{
  SliderSetsDBDAO::removeFromDatabase(this->mDatabase, aIndex);

  // Clear the file's content viewer if it was displaying the preset that has just been removed
  if (this->mCurrentPreviewIndex == aIndex)
    this->clearPreviewContent();
}

void SliderSetsDBManager::displaySliderSetContent(const int aIndex)
{
  auto lFilePath{Utils::GetSliderSetsFolderPath().append(QString::number(aIndex)).append(QStringLiteral(".osp"))};
  Utils::CleanPathString(lFilePath);

  QFile lReadFile(lFilePath);

  if (lReadFile.open(QFile::ReadOnly | QFile::Text))
  {
    const auto lFileViewer{this->findChild<PlainTextEdit*>("file_content_viewer")};
    lFileViewer->setPlainText(lReadFile.readAll());

    const auto lDisplayedFilePath{this->findChild<QLabel*>("file_path")};
    lDisplayedFilePath->setText(tr("Current file:") + " " + lFilePath);

    this->mCurrentPreviewIndex = aIndex;
  }
  else
  {
    this->clearPreviewContent();
  }

  lReadFile.close();
}

void SliderSetsDBManager::clearPreviewContent()
{
  const auto lFileViewer{this->findChild<PlainTextEdit*>("file_content_viewer")};
  lFileViewer->clear();

  const auto lDisplayedFilePath{this->findChild<QLabel*>("file_path")};
  lDisplayedFilePath->setText(tr("Waiting for a file to be displayed..."));

  this->mCurrentPreviewIndex = -1;
}

int SliderSetsDBManager::nextAvailableDatabaseIndex() const
{
  if (this->mDatabase.cbegin() == this->mDatabase.cend())
    return 1;

  return std::max_element(this->mDatabase.cbegin(),
                          this->mDatabase.cend(),
                          [](const auto& lhs, const auto& rhs) {
                            return lhs.first < rhs.first;
                          })
           ->first
         + 1;
}
