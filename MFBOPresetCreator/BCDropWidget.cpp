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
    auto lDebug{this->findChild<QLabel*>(QString("data"))};
    if (lDebug)
    {
      lDebug->setText("Dropped data:" + aEvent->mimeData()->text());
      aEvent->acceptProposedAction();
    }
  }
}
