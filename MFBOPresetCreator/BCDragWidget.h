#pragma once

#include "Struct.h"
#include <QWidget>

class BCDragWidget final : public QWidget
{
  Q_OBJECT

public:
  explicit BCDragWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aOriginFolder, const QString& aRessourcePath);

protected:
  void mousePressEvent(QMouseEvent* aEvent) override;
  void mouseReleaseEvent(QMouseEvent* aEvent) override;
  void mouseMoveEvent(QMouseEvent* aEvent) override;

private:
  QPoint mDragStartPosition;
  QString mOriginFolder;
  QString mRessourcePath;

  explicit BCDragWidget(const BCDragWidget&) = delete;
  BCDragWidget& operator=(const BCDragWidget&) = delete;
};
