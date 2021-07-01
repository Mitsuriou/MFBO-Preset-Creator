#pragma once

#include "ui_BCDragWidget.h"
#include <QWidget>

class BCDragWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDragWidget(QWidget* aParent, const QString& aButtonText);

protected:
  void mousePressEvent(QMouseEvent* aEvent) override;
  void mouseMoveEvent(QMouseEvent* aEvent) override;

  void dragEnterEvent(QDragEnterEvent* aEvent) override;
  void dropEvent(QDropEvent* aEvent) override;

private:
  Ui::BCDragWidget ui;
  QPoint mDragStartPosition;
};
