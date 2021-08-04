#pragma once

#include "Struct.h"
#include <QWidget>

class BCDragWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDragWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aOriginFolder, const QString& aPath);

protected:
  void mousePressEvent(QMouseEvent* aEvent) override;
  void mouseMoveEvent(QMouseEvent* aEvent) override;

private:
  QPoint mDragStartPosition;
};
