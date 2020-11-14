#include "AssistedConversion.h"

AssistedConversion::AssistedConversion(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void AssistedConversion::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Assisted Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/arrow_up")));
}
