#include "BCDragWidget.h"
#include <QDrag>
#include <QGridLayout>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>

BCDragWidget::BCDragWidget(QWidget* aParent, const QString& aButtonText)
  : QWidget(aParent)
{
  ui.setupUi(this);

  // Create the layout
  auto lMainLayout{new QGridLayout(this)};
  this->setLayout(lMainLayout);

  // TODO: Add the content
  // TOREMOVE: debug label below:
  QLabel* lDebug = new QLabel(aButtonText, this);
  lDebug->setObjectName("data");
  lMainLayout->addWidget(lDebug, 0, 0);
}

void BCDragWidget::mousePressEvent(QMouseEvent* aEvent)
{
  if (aEvent->button() == Qt::LeftButton)
  {
    this->mDragStartPosition = aEvent->pos();
  }
}

void BCDragWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
  if (!(aEvent->buttons() & Qt::LeftButton))
  {
    return;
  }

  if ((aEvent->pos() - this->mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
  {
    return;
  }

  QMimeData* lMimeData = new QMimeData;
  auto lWidget{this->findChild<QLabel*>("data")};
  lMimeData->setData("text/plain", lWidget->text().toLocal8Bit());

  QDrag* lDragObject{new QDrag(this)};
  lDragObject->setMimeData(lMimeData);

  Qt::DropAction dropAction = lDragObject->exec(Qt::CopyAction | Qt::MoveAction);
}

void BCDragWidget::dragEnterEvent(QDragEnterEvent* aEvent)
{
  aEvent->acceptProposedAction();
}

void BCDragWidget::dropEvent(QDropEvent* aEvent)
{
  if (aEvent->source() == this && aEvent->possibleActions() & Qt::MoveAction)
  {
    return;
  }

  if (aEvent->proposedAction() == Qt::MoveAction)
  {
    aEvent->acceptProposedAction();
  }
  else if (aEvent->proposedAction() == Qt::CopyAction)
  {
    aEvent->acceptProposedAction();
  }
  else
  {
    return;
  }
}
