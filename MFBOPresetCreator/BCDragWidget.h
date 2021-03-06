#pragma once

#include <QWidget>

class BCDragWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDragWidget(QWidget* aParent, const QString& aOriginFolder, const QString& aPath);

protected:
  void mousePressEvent(QMouseEvent* aEvent) override;
  void mouseMoveEvent(QMouseEvent* aEvent) override;

private:
  QPoint mDragStartPosition;
};
