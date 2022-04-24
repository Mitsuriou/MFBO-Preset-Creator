#include "TitleDialog.h"
#include <QCloseEvent>

TitleDialog::TitleDialog(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent)
  , mSettings{aSettings}
  , mLastPaths{aLastPaths}
{
}

const Struct::Settings& TitleDialog::settings()
{
  return this->mSettings;
}

std::map<QString, QString>* TitleDialog::lastPaths()
{
  return this->mLastPaths;
}

void TitleDialog::reject()
{
  this->close();
}
