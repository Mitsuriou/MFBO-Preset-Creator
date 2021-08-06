#pragma once

#include <QWidget>

class BCDropWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDropWidget(QWidget* aParent);
  QString getOriginFolder() const;
  QString getRessourcePath() const;
  void resetData();

signals:
  void dropEventTriggered(const QString& aOriginFolder, const QString& aRessourcePath);

protected:
  void dragEnterEvent(QDragEnterEvent* aEvent) override;
  void dragMoveEvent(QDragMoveEvent* aEvent) override;
  void dropEvent(QDropEvent* aEvent) override;

private:
  QString mOriginFolder;
  QString mRessourcePath;

  void tweakWidgetsVisibility(const bool aShouldViewDropZoneOnly, const QString& aNewOriginText = "", const QString& aNewRessourceText = "");
};
