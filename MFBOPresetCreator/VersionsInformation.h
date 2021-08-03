#pragma once
#include <QString>
#include <QStringList>
#include <vector>

struct VersionsInformation
{
public:
  // Stable versions list
  QString getLatestStableVersionNumber() const;
  const int sizeStableVersionsList() const;

  // Latest stable release notes
  QString getStableReleaseNotes(const QString& aVersion) const;
  QString getLatestStableReleaseNotes() const;

  // BETA versions list
  QString getLatestBetaVersionNumber() const;
  const int sizeBetaVersionsList() const;

  // Latest BETA release notes
  QString getBetaReleaseNotes(const QString& aVersion) const;
  QString getLatestBetaReleaseNotes() const;

  // Populate the maps
  void appendStableVersion(const QString& aVersion, const QString& aReleaseNotes);
  void appendBetaVersion(const QString& aVersion, const QString& aReleaseNotes);

  // Is running BETA version
  bool isRunningBetaVersion(const QString& aVersion) const;
  bool stableVersionsListContains(const QString& aVersion) const;

private:
  std::vector<std::pair<QString, QString>> stableVersionsList; // std::vector<std::pair<version number, release notes>>
  std::vector<std::pair<QString, QString>> betaVersionsList;   // std::vector<std::pair<version number, release notes>>
};
