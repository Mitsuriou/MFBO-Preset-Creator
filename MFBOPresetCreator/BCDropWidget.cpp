#include "BCDropWidget.h"
#include "BCDragWidget.h"
#include "Utils.h"
#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeData>
#include <QMouseEvent>

BCDropWidget::BCDropWidget(QWidget* aParent)
  : QWidget(aParent)
{
  this->setAcceptDrops(true); // Accept the drops since this is the purprose of this widget

  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setMargin(0);
  lMainLayout->setSpacing(10);
  lMainLayout->setColumnStretch(0, 0);
  lMainLayout->setColumnStretch(1, 1);
  this->setLayout(lMainLayout);

  // Drop indicator
  auto lDropIndicator{new QLabel("Drop some data here...", this)};
  lDropIndicator->setObjectName("drop_indicator");
  lMainLayout->addWidget(lDropIndicator, 0, 0, 1, 2);

  // Placeholder to avoid having a resizing widget
  auto lPlaceHolder{new QLabel(this)};
  lPlaceHolder->setObjectName("drop_indicator_place_holder");
  lMainLayout->addWidget(lPlaceHolder, 1, 0, 1, 2);

  // Path
  auto lPathLabel{new QLabel(tr("Mesh path:"))};
  lPathLabel->setObjectName("path_label");
  lPathLabel->hide();
  lMainLayout->addWidget(lPathLabel, 2, 0);

  auto lPathContent{new QLabel(this)};
  lPathContent->setObjectName("path_content");
  lPathContent->hide();
  lMainLayout->addWidget(lPathContent, 2, 1);

  // Origin folder
  auto lOriginLabel{new QLabel(tr("Origin mod:"))};
  lOriginLabel->setObjectName("origin_label");
  lOriginLabel->hide();
  lMainLayout->addWidget(lOriginLabel, 3, 0);

  auto lOriginContent{new QLabel(this)};
  lOriginContent->setObjectName("origin_content");
  lOriginContent->hide();
  lMainLayout->addWidget(lOriginContent, 3, 1);
}

QString BCDropWidget::getOriginFolder() const
{
  return this->mOriginFolder;
}

QString BCDropWidget::getRessourcePath() const
{
  return this->mRessourcePath;
}

void BCDropWidget::resetData()
{
  // Clear the two strings
  this->mOriginFolder.clear();
  this->mRessourcePath.clear();

  this->tweakWidgetsVisibility(true);
}

void BCDropWidget::dragEnterEvent(QDragEnterEvent* aEvent)
{
  // Only accept drag events from a BCDragWidget widget
  if (static_cast<BCDragWidget*>(aEvent->source()))
  {
    aEvent->acceptProposedAction();
  }
}

void BCDropWidget::dragMoveEvent(QDragMoveEvent* aEvent)
{
  // Only accept drag events from a BCDragWidget widget
  if (static_cast<BCDragWidget*>(aEvent->source()))
  {
    aEvent->acceptProposedAction();
  }
}

void BCDropWidget::dropEvent(QDropEvent* aEvent)
{
  if (aEvent->source() == this && aEvent->possibleActions() & Qt::MoveAction)
  {
    return;
  }

  // BCDropWidget is only compatibl with the Qt::MoveAction
  if (aEvent->proposedAction() == Qt::MoveAction)
  {
    // Accept the drop event
    aEvent->acceptProposedAction();

    // Parse the JSON string
    auto lDataObject{QJsonDocument::fromJson(aEvent->mimeData()->data("application/json")).object()};

    // Upper treatment (before updating the data)
    emit BCDropWidget::dropEventTriggered(this->mOriginFolder, this->mRessourcePath, lDataObject["originFolder"].toString(), lDataObject["ressourcePath"].toString());

    // Save the new data
    this->mOriginFolder = lDataObject["originFolder"].toString();
    this->mRessourcePath = lDataObject["ressourcePath"].toString();

    // Update the displayed information
    this->tweakWidgetsVisibility(false, this->mOriginFolder, this->mRessourcePath);
  }
}

void BCDropWidget::tweakWidgetsVisibility(const bool aShouldViewDropZoneOnly, const QString& aNewOriginText, const QString& aNewRessourceText)
{
  auto lDropIndicator{this->findChild<QLabel*>(QString("drop_indicator"))};
  lDropIndicator->setHidden(!aShouldViewDropZoneOnly);

  auto lPlaceHolder{this->findChild<QLabel*>(QString("drop_indicator_place_holder"))};
  lPlaceHolder->setHidden(!aShouldViewDropZoneOnly);

  auto lPathLabel{this->findChild<QLabel*>(QString("path_label"))};
  lPathLabel->setHidden(aShouldViewDropZoneOnly);

  auto lPathContent{this->findChild<QLabel*>(QString("path_content"))};
  lPathContent->setText(aNewRessourceText);
  lPathContent->setHidden(aShouldViewDropZoneOnly);

  auto lOriginLabel{this->findChild<QLabel*>(QString("origin_label"))};
  lOriginLabel->setHidden(aShouldViewDropZoneOnly);

  auto lOriginContent{this->findChild<QLabel*>(QString("origin_content"))};
  lOriginContent->setText(aNewOriginText);
  lOriginContent->setHidden(aShouldViewDropZoneOnly);
}
