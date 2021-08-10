#include "BCDropWidget.h"
#include "BCDragWidget.h"
#include "Utils.h"
#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeData>
#include <QMouseEvent>

BCDropWidget::BCDropWidget(QWidget* aParent, const BCGroupWidgetCallContext& aCallContext)
  : QWidget(aParent)
  , mCallContext(aCallContext)
{
  // Accept the drops since this is the purprose of this widget
  this->setAcceptDrops(true);

  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setMargin(0);
  lMainLayout->setSpacing(10);
  lMainLayout->setColumnStretch(0, 0);
  lMainLayout->setColumnStretch(1, 1);
  this->setLayout(lMainLayout);

  // Path
  auto lPathLabel{new QLabel(this)};
  lPathLabel->setObjectName("path_label");
  lMainLayout->addWidget(lPathLabel, 0, 0);

  auto lPathContent{new QLabel(this)};
  lPathContent->setObjectName("path_content");
  lMainLayout->addWidget(lPathContent, 0, 1);

  // Origin folder
  auto lOriginLabel{new QLabel(this)};
  lOriginLabel->setObjectName("origin_label");
  lMainLayout->addWidget(lOriginLabel, 1, 0);

  auto lOriginContent{new QLabel(this)};
  lOriginContent->setObjectName("origin_content");
  lMainLayout->addWidget(lOriginContent, 1, 1);

  // TODO: Add a checkbox for the alternative models

  this->resetData();
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

void BCDropWidget::setData(const QString& aOriginFolder, const QString& aRessourcePath, const bool aUseAlternativeModel)
{
  this->mOriginFolder = aOriginFolder;
  this->mRessourcePath = aRessourcePath;
  this->tweakWidgetsVisibility(aRessourcePath.length() == 0, aOriginFolder, aRessourcePath);
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
  auto lPathLabel{this->findChild<QLabel*>(QString("path_label"))};
  auto lPathContent{this->findChild<QLabel*>(QString("path_content"))};
  auto lOriginLabel{this->findChild<QLabel*>(QString("origin_label"))};
  auto lOriginContent{this->findChild<QLabel*>(QString("origin_content"))};

  if (aShouldViewDropZoneOnly)
  {
    lPathLabel->setText(tr("Drop some data here..."));
    lPathContent->setText("");
    lOriginLabel->setText("");
    lOriginContent->setText("");
  }
  else
  {
    lPathLabel->setText(tr("Mesh path:"));
    lPathContent->setText(aNewRessourceText);
    lOriginLabel->setText(tr("Origin mod:"));
    lOriginContent->setText(aNewOriginText);
  }
}
