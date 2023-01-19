#include "SliderSetsDBDAO.h"
#include "Utils.h"
#include <QDir>
#include <QFile>

namespace SliderSetsDBDAO
{
  constexpr QChar DB_SEPARATOR = ';';

  std::map<int, Struct::DatabaseSliderSet> loadDatabase()
  {
    QFile lDatabaseFile(Utils::GetDatabaseFilePath());
    if (!lDatabaseFile.exists())
    {
      if (!QDir().mkpath(Utils::GetDatabaseFilePath()))
      {
        // TODO: error message
        return {};
      }
    }

    std::map<int, Struct::DatabaseSliderSet> lDatabase;

    if (lDatabaseFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream in(&lDatabaseFile);
      while (!in.atEnd())
      {
        // Push the entry
        lDatabase.insert(SliderSetsDBDAO::parseDatabaseLine(in.readLine()));
      }
    }
    lDatabaseFile.close();

    // Purge the database
    const auto lDatabaseBeforePurge{lDatabase};
    SliderSetsDBDAO::detectRemovedFiles(lDatabase);

    // Save the database if needed
    if (lDatabaseBeforePurge != lDatabase)
      SliderSetsDBDAO::saveDatabase(lDatabase);

    return lDatabase;
  }

  void saveDatabase(const std::map<int, Struct::DatabaseSliderSet>& aDatabase)
  {
    const auto lAbsFilePath{Utils::GetDatabaseFilePath()};

    QFile lOSPFile(lAbsFilePath);
    if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      QTextStream lTextStream(&lOSPFile);
      lTextStream << SliderSetsDBDAO::databaseToString(aDatabase);
      lTextStream.flush();

      lOSPFile.close();
    }
    else
    {
      // TODO: error message
    }
  }

  void removeFromDatabase(std::map<int, Struct::DatabaseSliderSet>& aDatabase, const int aIndex)
  {
    const auto lPosition{aDatabase.find(aIndex)};

    if (lPosition != aDatabase.end())
      lPosition->second.setActive(false);
  }

  void detectRemovedFiles(std::map<int, Struct::DatabaseSliderSet>& aDatabase)
  {
    const auto lBasePath{Utils::GetSliderSetsFolderPath()};

    for (const auto& lEntry : aDatabase)
    {
      // If the file does not exist anymore, mark it as removed
      if (!QFile(lBasePath + QString::number(lEntry.first) + QStringLiteral(".osp")).exists())
      {
        SliderSetsDBDAO::removeFromDatabase(aDatabase, lEntry.first);
      }
    }
  }

  QString databaseToString(const std::map<int, Struct::DatabaseSliderSet>& aDatabase)
  {
    QString lString;

    for (const auto& lEntry : aDatabase)
      lString.append(SliderSetsDBDAO::stringifyDatabaseEntry(lEntry));

    return lString;
  }

  std::pair<int, Struct::DatabaseSliderSet> parseDatabaseLine(const QString& aLine)
  {
    const auto& lParts{aLine.split(DB_SEPARATOR)};

    return std::make_pair(lParts[0].toInt(),
                          Struct::DatabaseSliderSet(lParts[1].toInt() == 1,
                                                    QByteArray::fromBase64(lParts[2].toUtf8()),
                                                    static_cast<MeshPartType>(lParts[3].toInt())));
  }

  QString stringifyDatabaseEntry(const std::pair<int, Struct::DatabaseSliderSet>& lEntry)
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
};
