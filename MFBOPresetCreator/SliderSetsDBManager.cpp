#include "SliderSetsDBManager.h"
#include "ComponentFactory.h"
#include "SliderSetsImporter.h"
#include "Utils.h"
#include <QApplication>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QProgressDialog>
#include <QSplitter>

constexpr int XML_INDENT = 4;
constexpr QChar DB_SEPARATOR = ';';

SliderSetsDBManager::SliderSetsDBManager(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, tr("Slider Sets Database Manager"), "database", aSettings, aLastPaths, 800)
{
  // Load the database before doing anything else
  this->loadDatabase();

  // Build the window's interface
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.sliderSetsDBManagerDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void SliderSetsDBManager::closeEvent(QCloseEvent*)
{
  this->saveDatabase();
}

void SliderSetsDBManager::initializeGUI()
{
  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this->getCentralWidget(), true, false)};

  // Splitter
  const auto lSplitter{new QSplitter(Qt::Horizontal, this)};
  lSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lMainLayout->addWidget(lSplitter, 0, 0);

  // Left side
  const auto lLeftWrapper{new QWidget(this)};
  const auto lLeftWrapperLayout{new QGridLayout(this)};
  lLeftWrapper->setLayout(lLeftWrapperLayout);

  const auto lLeftLayout{ComponentFactory::CreateScrollAreaComponentLayout(lLeftWrapper, *lLeftWrapperLayout, 0, 0)};
  lSplitter->addWidget(lLeftWrapper);

  auto lButton{ComponentFactory::CreateButton(this, "DEBUG - IMPORT SLIDER SETS", "", "", "")};
  lLeftLayout->addWidget(lButton);

  // Right side
  const auto lFileViewer{new QPlainTextEdit(this)};
  lFileViewer->setReadOnly(true);
  lFileViewer->setLineWrapMode(QPlainTextEdit::NoWrap);
  lFileViewer->setWordWrapMode(QTextOption::NoWrap);
  lFileViewer->setPlainText("test");
  lSplitter->addWidget(lFileViewer);

  QObject::connect(lButton, &QPushButton::clicked, this, &SliderSetsDBManager::openSliderSetsImporter);
}

void SliderSetsDBManager::loadDatabase()
{
  QFile lDatabaseFile(Utils::GetDatabaseFilePath());
  if (!lDatabaseFile.exists())
  {
    if (!QDir().mkpath(Utils::GetDatabaseFilePath()))
    {
      // TODO: error message
      return;
    }
  }

  if (lDatabaseFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream in(&lDatabaseFile);
    while (!in.atEnd())
    {
      // This should be the only place the mInitialDatabase is used as lhs
      this->mInitialDatabase.insert(this->parseDatabaseLine(in.readLine()));
    }
  }
  lDatabaseFile.close();

  // Copy the initial database to the database that is going to be used for this session
  this->mRunningDatabase = this->mInitialDatabase;

  // Purge the database
  this->detectRemovedFiles();

  // Save the new database state
  this->saveDatabase();
}

void SliderSetsDBManager::saveDatabase()
{
  if (this->mInitialDatabase != this->mRunningDatabase)
  {
    const auto lAbsFilePath{Utils::GetDatabaseFilePath()};

    QFile lOSPFile(lAbsFilePath);
    if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      QTextStream lTextStream(&lOSPFile);
      lTextStream << this->databaseToString();
      lTextStream.flush();

      lOSPFile.close();

      this->mInitialDatabase = this->mRunningDatabase;
    }
    else
    {
      // TODO: error message
    }
  }
}

void SliderSetsDBManager::detectRemovedFiles()
{
  const auto lBasePath{Utils::GetSliderSetsFolderPath()};

  QString lString;
  for (const auto& lEntry : this->mRunningDatabase)
  {
    // If the file does not exist anymore, mark it as removed
    if (!QFile(lBasePath + QString::number(lEntry.first) + ".osp").exists())
    {
      this->removeFromDatabase(lEntry.first);
    }
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
  auto lProgressBar{new QProgressBar(this)};
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
      if (lNextNodeToParse.tagName().compare("SliderSet", Qt::CaseSensitivity::CaseInsensitive) == 0)
      {
        const auto lSliderSetName{lNextNodeToParse.attribute("name", "")};

        // Check if a SliderSet chosen by the user has the same name than the current one
        for (const auto& lSliderSet : lEntry.second)
        {
          if (lSliderSetName == lSliderSet->getOriginalSliderSetName())
          {
            // Update the name with the new one given by the user
            lNextNodeToParse.setAttribute("name", lSliderSet->getNewSliderSetName());

            this->saveSliderSetToDatabase(lNextNodeToParse,
                                          Struct::DatabaseSliderSet(true,
                                                                    lSliderSet->getNewSliderSetName(),
                                                                    lSliderSet->getMeshType()));

            break;
          }

          lProgressDialog.setValue(++lTreatedFiles);
          QCoreApplication::processEvents();
        }
      }

      lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
    }
  }
}

bool SliderSetsDBManager::saveSliderSetToDatabase(const QDomElement& aSliderSetNode, const Struct::DatabaseSliderSet& aSliderSetData)
{
  QString lXMLContent;
  QTextStream lStream(&lXMLContent, QIODevice::WriteOnly);
  aSliderSetNode.save(lStream, XML_INDENT);

  auto lSliderSetIndex{this->nextAvailableDatabaseIndex()};

  // Create the XML file on disk
  const auto lAbsFilePath{Utils::GetSliderSetsFolderPath()
                            .append(QString::number(lSliderSetIndex))
                            .append(".osp")};

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

void SliderSetsDBManager::addDatabaseLine(const int& aIndex, const Struct::DatabaseSliderSet& aSliderSetData)
{
  auto lPosition{this->mRunningDatabase.find(aIndex)};

  // If the index is not already in the database, add it
  if (lPosition == this->mRunningDatabase.end())
  {
    this->mRunningDatabase.insert({aIndex, aSliderSetData});
  }

  this->saveDatabase();
}

void SliderSetsDBManager::updateSliderSetName(const int aIndex, const QString& aNewName)
{
  const auto lPosition{this->mRunningDatabase.find(aIndex)};
  if (lPosition != this->mRunningDatabase.end())
  {
    lPosition->second.setSliderSetName(aNewName);
  }
}

void SliderSetsDBManager::updateSliderSetMeshType(const int aIndex, const MeshPartType aMeshType)
{
  const auto lPosition{this->mRunningDatabase.find(aIndex)};
  if (lPosition != this->mRunningDatabase.end())
  {
    lPosition->second.setMeshType(aMeshType);
  }
}

void SliderSetsDBManager::removeFromDatabase(const int& aIndex)
{
  const auto lPosition{this->mRunningDatabase.find(aIndex)};
  if (lPosition != this->mRunningDatabase.end())
  {
    lPosition->second.setActive(false);
  }
}

int SliderSetsDBManager::nextAvailableDatabaseIndex()
{
  if (this->mRunningDatabase.begin() == this->mRunningDatabase.end())
  {
    return 1;
  }

  return std::max_element(this->mRunningDatabase.begin(),
                          this->mRunningDatabase.end(),
                          [](const auto& lhs, const auto& rhs) {
                            return lhs.first < rhs.first;
                          })
           ->first
         + 1;
}

QString SliderSetsDBManager::databaseToString()
{
  QString lString;
  for (const auto& lEntry : this->mRunningDatabase)
  {
    lString.append(this->stringifyDatabaseEntry(lEntry));
  }

  return lString;
}

std::pair<int, Struct::DatabaseSliderSet> SliderSetsDBManager::parseDatabaseLine(const QString& aLine)
{
  const auto& lParts{aLine.split(DB_SEPARATOR)};
  return {lParts[0].toInt(),
          Struct::DatabaseSliderSet(lParts[1].toInt() == 1,
                                    QByteArray::fromBase64(lParts[2].toUtf8()),
                                    static_cast<MeshPartType>(lParts[3].toInt()))};
}

QString SliderSetsDBManager::stringifyDatabaseEntry(const std::pair<int, Struct::DatabaseSliderSet>& lEntry)
{
  return QString::number(lEntry.first)
    .append(DB_SEPARATOR)
    .append(QString::number(lEntry.second.isActive()))
    .append(DB_SEPARATOR)
    .append(lEntry.second.getSliderSetName().toUtf8().toBase64())
    .append(DB_SEPARATOR)
    .append(QString::number(static_cast<int>(lEntry.second.getMeshType())))
    .append("\n");
}
