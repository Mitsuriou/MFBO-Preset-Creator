#pragma once

#include <QWidget>

class BCDragWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDragWidget(QWidget* aParent, const QString& aButtonText);

protected:
  void mousePressEvent(QMouseEvent* aEvent) override;
  void mouseMoveEvent(QMouseEvent* aEvent) override;

private:
  QPoint mDragStartPosition;
};
