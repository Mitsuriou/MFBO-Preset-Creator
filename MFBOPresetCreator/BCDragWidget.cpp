#include "BCDragWidget.h"
#include <QApplication>
#include <QDrag>
#include <QGridLayout>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>

BCDragWidget::BCDragWidget(QWidget* aParent, const QString& aButtonText)
  : QWidget(aParent)
{
  // Create the layout
  auto lMainLayout{new QGridLayout(this)};
  this->setLayout(lMainLayout);

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

  auto lWidget{this->findChild<QLabel*>("data")};

  QMimeData* lMimeData = new QMimeData;
  lMimeData->setText(lWidget->text());

  QDrag* lDragObject{new QDrag(this)};
  lDragObject->setMimeData(lMimeData);
  lDragObject->exec(Qt::MoveAction);
}
