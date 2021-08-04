#pragma once

#include <QWidget>

class BCDropWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDropWidget(QWidget* aParent);
  QString getRessourcePath();
  void resetData();

signals:
  void dropEventTriggered(const QString& aPathToRemove);

protected:
  void dragEnterEvent(QDragEnterEvent* aEvent) override;
  void dragMoveEvent(QDragMoveEvent* aEvent) override;
  void dropEvent(QDropEvent* aEvent) override;

private:
  bool mHasReceivedData{false};
  QString mPath;
};
