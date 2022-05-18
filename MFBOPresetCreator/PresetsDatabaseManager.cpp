#include "PresetsDatabaseManager.h"
#include "ComponentFactory.h"
#include "SliderSetsImporter.h"
#include "Utils.h"
#include <QApplication>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QProgressBar>
#include <QProgressDialog>

constexpr int XML_INDENT = 4;

PresetsDatabaseManager::PresetsDatabaseManager(QWidget* aParent,
                                               const Struct::Settings& aSettings,
                                               std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, aSettings, aLastPaths)
{
  this->loadDatabase();

  const auto lMainLayout = new QVBoxLayout(this);
  this->setLayout(lMainLayout);

  const auto lButton{ComponentFactory::CreateButton(this, "DEBUG - IMPORT PRESETS", "", "", "")};
  lMainLayout->addWidget(lButton);

  QObject::connect(lButton, &QPushButton::clicked, this, &PresetsDatabaseManager::openSliderSetsImporter);

  this->show();
}

void PresetsDatabaseManager::closeEvent(QCloseEvent*)
{
  this->saveDatabase();
}

void PresetsDatabaseManager::loadDatabase()
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
      const auto lData{this->parseDatabaseLine(in.readLine())};

      // This should be the only place the mInitialDatabase is used as lhs
      this->mInitialDatabase[lData.first] = lData.second;
    }
  }
  lDatabaseFile.close();

  // Copy the initial database to the database that is going to be used for this session
  this->mRunningDatabase = this->mInitialDatabase;

  // Purge the database
  this->detectRemovedFiles();
}

void PresetsDatabaseManager::saveDatabase()
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
    }
    else
    {
      // TODO: error message
    }
  }
}

void PresetsDatabaseManager::detectRemovedFiles()
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

void PresetsDatabaseManager::openSliderSetsImporter()
{
  const auto lSliderSetsImporter{new SliderSetsImporter(this, this->settings(), this->lastPaths())};
  QObject::connect(lSliderSetsImporter, &SliderSetsImporter::valuesChosen, this, &PresetsDatabaseManager::importNewSliderSets);
}

void PresetsDatabaseManager::importNewSliderSets(const std::vector<Struct::SliderSetResult>& aChosenPresets)
{
  std::map<QString, std::vector<const Struct::SliderSetResult*>> lPaths;
  auto lEntriesCount{0};

  // Iterate over the results
  for (const auto& lChosenPreset : aChosenPresets)
  {
    // Filter the paths to know which file to open and parse
    lPaths[lChosenPreset.getFilePath()].push_back(&lChosenPreset);
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

        // Check if a preset chosen by the user has the same name than the current one
        for (const auto& lSliderSet : lEntry.second)
        {
          if (lSliderSetName == lSliderSet->getOriginalSliderSetName())
          {
            // Update the name with the new one given by the user
            lNextNodeToParse.setAttribute("name", lSliderSet->getNewSliderSetName());

            this->saveSliderSetToDatabase(lNextNodeToParse);

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

bool PresetsDatabaseManager::saveSliderSetToDatabase(const QDomElement& aSliderSetNode)
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

    this->addDatabaseLine(lSliderSetIndex);

    return true;
  }

  Utils::DisplayWarningMessage(tr("Error while trying to create the SliderSet file \"%1\". Aborting process.").arg(lAbsFilePath));
  return false;
}

void PresetsDatabaseManager::addDatabaseLine(const int& aIndex)
{
  auto lPosition{this->mRunningDatabase.find(aIndex)};

  // If the index is not already in the database, add it
  if (lPosition == this->mRunningDatabase.end())
  {
    this->mRunningDatabase[aIndex] = true;
  }
}

void PresetsDatabaseManager::updateDatabaseLine(const int aIndex, const bool aValue)
{
  auto lPosition{this->mRunningDatabase.find(aIndex)};

  // If the index is already in the database, update it
  if (lPosition != this->mRunningDatabase.end())
  {
    lPosition->second = aValue;
  }
}

void PresetsDatabaseManager::removeFromDatabase(const int& aIndex)
{
  const auto lPosition{this->mRunningDatabase.find(aIndex)};
  if (lPosition != this->mRunningDatabase.end())
  {
    lPosition->second = false;
  }
}

int PresetsDatabaseManager::nextAvailableDatabaseIndex()
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

QString PresetsDatabaseManager::databaseToString()
{
  QString lString;
  for (const auto& lEntry : this->mRunningDatabase)
  {
    lString.append(this->stringifyDatabaseEntry(lEntry));
  }

  return lString;
}

std::pair<int, bool> PresetsDatabaseManager::parseDatabaseLine(const QString& aLine)
{
  const auto& lParts{aLine.split(',')};
  return std::pair<int, bool>(lParts[0].toInt(), lParts[1].toInt() == 1);
}

QString PresetsDatabaseManager::stringifyDatabaseEntry(const std::pair<int, bool>& lEntry)
{
  return QString::number(lEntry.first)
    .append(",")
    .append(QString::number(lEntry.second))
    .append("\n");
}
