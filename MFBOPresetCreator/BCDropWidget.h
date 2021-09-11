#pragma once

#include "Enum.h"
#include <QWidget>

class BCDropWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDropWidget(QWidget* aParent, const BCGroupWidgetCallContext& aCallContext);
  QString getOriginFolder() const;
  QString getRessourcePath() const;
  void resetData();
  void setData(const QString& aOriginFolder, const QString& aRessourcePath, const bool aUseAlternativeModel = false);

signals:
  void dropEventTriggered(const QString& aOldOriginFolder, const QString& aOldRessourcePath, const QString& aNewOriginFolder, const QString& aNewRessourcePath);

protected:
  void dragEnterEvent(QDragEnterEvent* aEvent) override;
  void dragMoveEvent(QDragMoveEvent* aEvent) override;
  void dropEvent(QDropEvent* aEvent) override;

private:
  BCGroupWidgetCallContext mCallContext{BCGroupWidgetCallContext::UNDEFINED};
  QString mOriginFolder;
  QString mRessourcePath;

  void tweakWidgetsVisibility(const bool aShouldViewDropZoneOnly, const QString& aNewOriginText = QString(""), const QString& aNewRessourceText = QString(""));
};
