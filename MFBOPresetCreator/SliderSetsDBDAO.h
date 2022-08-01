#pragma once
#include "Struct.h"

// Functions moved from SliderSetsDBManager class
namespace SliderSetsDBDAO
{
  // Database I/O streams
  std::map<int, Struct::DatabaseSliderSet> loadDatabase();
  void saveDatabase(const std::map<int, Struct::DatabaseSliderSet>& aDatabase);

  // Delete
  void removeFromDatabase(std::map<int, Struct::DatabaseSliderSet>& aDatabase, const int aIndex);

  // Utils functions
  void detectRemovedFiles(std::map<int, Struct::DatabaseSliderSet>& aDatabase);

  QString databaseToString(const std::map<int, Struct::DatabaseSliderSet>& aDatabase);
  std::pair<int, Struct::DatabaseSliderSet> parseDatabaseLine(const QString& aLine);
  QString stringifyDatabaseEntry(const std::pair<int, Struct::DatabaseSliderSet>& lEntry);
};
