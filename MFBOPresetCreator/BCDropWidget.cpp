#include "BCDropWidget.h"
#include "BCDragWidget.h"
#include "Utils.h"
#include <QMimeData>
#include <QMouseEvent>

BCDropWidget::BCDropWidget(QWidget* aParent)
  : QWidget(aParent)
{
  this->setAcceptDrops(true);

  auto lMainLayout{new QVBoxLayout(this)};
  this->setLayout(lMainLayout);

  auto lDebug{new QLabel("Drop something here...", this)};
  lDebug->setObjectName("data");
  lMainLayout->addWidget(lDebug);
}

QString BCDropWidget::getRessourcePath()
{
  return this->mPath;
}

void BCDropWidget::resetData()
{
  this->setAcceptDrops(true);
  this->mHasReceivedData = false;
  this->mPath.clear();

  auto lDebug{this->findChild<QLabel*>(QString("data"))};
  lDebug->setText("Drop something here...");
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
    // Block the next drops
    this->setAcceptDrops(false);

    // Save the new path
    this->mPath = aEvent->mimeData()->text();

    // Update the displayed information
    auto lDebug{this->findChild<QLabel*>(QString("data"))};
    lDebug->setText("[DEBUG] Dropped data: " + this->mPath);
    aEvent->acceptProposedAction();
    this->mHasReceivedData = true;

    // Upper treatment
    emit dropEventTriggered(this->mPath);
  }
}
