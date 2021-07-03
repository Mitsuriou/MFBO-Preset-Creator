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
  Utils::printMessageStdOut("ENTER");
  if (static_cast<BCDragWidget*>(aEvent->source()))
  {
    Utils::printMessageStdOut("ENTER ACCEPTED");
    aEvent->acceptProposedAction();
  }
}

void BCDropWidget::dragMoveEvent(QDragMoveEvent* aEvent)
{
  Utils::printMessageStdOut("MOVE");
  if (static_cast<BCDragWidget*>(aEvent->source()))
  {
    Utils::printMessageStdOut("MOVE ACCEPTED");
    aEvent->acceptProposedAction();
  }
}

void BCDropWidget::dropEvent(QDropEvent* aEvent)
{
  if (aEvent->source() == this && aEvent->possibleActions() & Qt::MoveAction)
  {
    return;
  }

  if (aEvent->proposedAction() == Qt::MoveAction)
  {
    auto lDebug{this->findChild<QLabel*>("data")};
    if (lDebug)
    {
      lDebug->setText("Dropped data:" + aEvent->mimeData()->text());
      aEvent->acceptProposedAction();
    }
    return;
  }
  else
  {
    return;
  }
}
