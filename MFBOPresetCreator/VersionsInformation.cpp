#include "VersionsInformation.h"

QString VersionsInformation::getLatestStableVersionNumber() const
{
  return this->stableVersionsList.at(0).first;
}

int VersionsInformation::sizeStableVersionsList() const
{
  return static_cast<int>(this->stableVersionsList.size());
}

QString VersionsInformation::getStableReleaseNotes(const QString& aVersion) const
{
  for (const auto& lPair : this->stableVersionsList)
  {
    if (lPair.first == aVersion)
    {
      return lPair.second;
    }
  }

  return QString();
}

QString VersionsInformation::getLatestStableReleaseNotes() const
{
  return this->stableVersionsList.begin()->second;
}

QString VersionsInformation::getLatestBetaVersionNumber() const
{
  return this->betaVersionsList.at(0).first;
}

int VersionsInformation::sizeBetaVersionsList() const
{
  return static_cast<int>(this->betaVersionsList.size());
}

QString VersionsInformation::getBetaReleaseNotes(const QString& aVersion) const
{
  for (const auto& lPair : this->betaVersionsList)
  {
    if (lPair.first == aVersion)
    {
      return lPair.second;
    }
  }

  return QString();
}

QString VersionsInformation::getLatestBetaReleaseNotes() const
{
  return this->betaVersionsList.begin()->second;
}

void VersionsInformation::appendStableVersion(const QString& aVersion, const QString& aReleaseNotes)
{
  this->stableVersionsList.push_back(std::make_pair(aVersion, aReleaseNotes));
}

void VersionsInformation::appendBetaVersion(const QString& aVersion, const QString& aReleaseNotes)
{
  this->betaVersionsList.push_back(std::make_pair(aVersion, aReleaseNotes));
}

bool VersionsInformation::isRunningBetaVersion(const QString& aVersion) const
{
  for (const auto& lPair : this->betaVersionsList)
  {
    if (lPair.first == aVersion)
    {
      return true;
    }
  }

  return false;
}

bool VersionsInformation::stableVersionsListContains(const QString& aVersion) const
{
  for (const auto& lPair : this->stableVersionsList)
  {
    if (lPair.first == aVersion)
    {
      return true;
    }
  }

  return false;
}
