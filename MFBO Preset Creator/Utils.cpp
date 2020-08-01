#include "Utils.h"

void Utils::cleanPathString(QString& aPath)
{
  aPath.replace("\\", "/");
}

QString Utils::getProgramVersion()
{
  return QString("1.2.0.0-pre1");
}
