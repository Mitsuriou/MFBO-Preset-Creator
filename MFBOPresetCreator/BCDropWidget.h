#pragma once

#include <QWidget>

class BCDropWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDropWidget(QWidget* aParent);

protected:
  void dragEnterEvent(QDragEnterEvent* aEvent) override;
  void dragMoveEvent(QDragMoveEvent* aEvent) override;
  void dropEvent(QDropEvent* aEvent) override;
};
