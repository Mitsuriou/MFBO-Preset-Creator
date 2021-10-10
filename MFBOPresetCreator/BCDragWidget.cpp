#include "BCDragWidget.h"
#include "Utils.h"
#include <QApplication>
#include <QDrag>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonDocument >
#include <QJsonObject>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QVBoxLayout>

BCDragWidget::BCDragWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aOriginFolder, const QString& aRessourcePath)
  : QWidget(aParent)
  , mOriginFolder(aOriginFolder)
  , mRessourcePath(aRessourcePath)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(aSettings.display.applicationTheme)};

  // Main layout
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setMargin(0);
  this->setLayout(lMainLayout);

  // Tweak the group box text and icon
  auto lGroupBoxTitle{tr("Unknown mesh type")};
  auto lGroupBoxIcon{QString("mesh")};

  auto lRessourceType{Utils::GetMeshTypeFromFileName(aRessourcePath)};
  switch (lRessourceType)
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
  }

  auto lSection{new QGroupBox(lGroupBoxTitle.append("  "), this)};
  Utils::AddIconToGroupBox(lSection, lIconFolder, lGroupBoxIcon, aSettings.display.font.size);
  this->connect(lSection, &QGroupBox::toggled, this, &BCDragWidget::groupBoxChecked);
  Utils::SetGroupBoxState(lSection, false);

  auto lSectionLayout{new QVBoxLayout(this)};
  lSectionLayout->setSpacing(10);
  lSectionLayout->setContentsMargins(15, 20, 15, 15);
  lSection->setLayout(lSectionLayout);

  lMainLayout->addWidget(lSection);

  auto lPathLabel{new QLabel(this->mRessourcePath, this)};
  lSectionLayout->addWidget(lPathLabel);

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
  lDataObject["ressourcePath"] = this->mRessourcePath;

  QMimeData* lMimeData = new QMimeData;
  lMimeData->setData("application/json", QJsonDocument(lDataObject).toJson());

  QDrag* lDragObject{new QDrag(this)};
  lDragObject->setMimeData(lMimeData);
  lDragObject->exec(Qt::MoveAction);
}

void BCDragWidget::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::SetGroupBoxState(lGroupBox, !aIsChecked);
}
