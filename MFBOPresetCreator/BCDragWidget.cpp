#include "BCDragWidget.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QApplication>
#include <QDrag>
#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QVBoxLayout>

BCDragWidget::BCDragWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aOriginFolder, const QString& aResourcePath)
  : QWidget(aParent)
  , mOriginFolder(aOriginFolder)
  , mResourcePath(aResourcePath)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(aSettings.display.applicationTheme)};

  // Main layout
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(lMainLayout);

  // Tweak the group box text and icon
  QString lGroupBoxTitle;
  QString lGroupBoxIcon;

  const auto lResourceType{Utils::GetMeshTypeFromFileName(aResourcePath)};
  switch (lResourceType)
  {
    case BCGroupWidgetCallContext::BODY:
      lGroupBoxTitle = tr("Body mesh");
      lGroupBoxIcon = "body";
      break;
    case BCGroupWidgetCallContext::FEET:
      lGroupBoxTitle = tr("Feet mesh");
      lGroupBoxIcon = "foot";
      break;
    case BCGroupWidgetCallContext::HANDS:
      lGroupBoxTitle = tr("Hands mesh");
      lGroupBoxIcon = "hand";
      break;
    case BCGroupWidgetCallContext::HEAD:
      lGroupBoxTitle = tr("Head mesh");
      lGroupBoxIcon = "woman-head";
      break;
    case BCGroupWidgetCallContext::EYES:
      lGroupBoxTitle = tr("Eyes mesh");
      lGroupBoxIcon = "eye";
      break;
    case BCGroupWidgetCallContext::SKELETON:
      lGroupBoxTitle = tr("Skeleton mesh");
      lGroupBoxIcon = "skeleton";
      break;
    case BCGroupWidgetCallContext::UNDEFINED:
      lGroupBoxTitle = tr("Unknown mesh type");
      lGroupBoxIcon = "mesh";
      break;
  }

  auto lGroupBox{ComponentFactory::CreateGroupBox(this, lGroupBoxTitle, lGroupBoxIcon, lIconFolder, aSettings.display.font.pointSize)};

  const auto lGroupBoxLayout{new QVBoxLayout(this)};
  lGroupBoxLayout->setSpacing(10);
  lGroupBoxLayout->setContentsMargins(15, 20, 15, 15);
  lGroupBox->setLayout(lGroupBoxLayout);

  lMainLayout->addWidget(lGroupBox);

  const auto lPathLabel{new QLabel(this->mResourcePath, this)};
  lGroupBoxLayout->addWidget(lPathLabel);

  // Change the cursor
  this->setCursor(Qt::OpenHandCursor);
}

void BCDragWidget::mousePressEvent(QMouseEvent* aEvent)
{
  if (aEvent->button() == Qt::LeftButton)
  {
    // Change the cursor
    this->setCursor(Qt::ClosedHandCursor);

    // Start the drag
    this->mDragStartPosition = aEvent->pos();
  }
}

void BCDragWidget::mouseReleaseEvent(QMouseEvent*)
{
  // Change the cursor
  this->setCursor(Qt::OpenHandCursor);
}

void BCDragWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
  // Check if the left mouse button is pressed
  if (!(aEvent->buttons() & Qt::LeftButton))
  {
    return;
  }

  // Check if the drag distance is long enough to be a drag
  if ((aEvent->pos() - this->mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
  {
    return;
  }

  // Accept the drag, set the mime data to be transfered
  QJsonObject lDataObject;
  lDataObject["originFolder"] = this->mOriginFolder;
  lDataObject["resourcesPath"] = this->mResourcePath;

  QMimeData* lMimeData{new QMimeData};
  lMimeData->setData("application/json", QJsonDocument(lDataObject).toJson());

  QDrag* lDragObject{new QDrag(this)};
  lDragObject->setMimeData(lMimeData);
  lDragObject->exec(Qt::MoveAction);
}
