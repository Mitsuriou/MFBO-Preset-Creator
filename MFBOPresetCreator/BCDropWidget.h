#pragma once

#include "Enum.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QWidget>

class BCDropWidget final : public QWidget
{
  Q_OBJECT

public:
  explicit BCDropWidget(QWidget* aParent, const BCGroupWidgetCallContext& aCallContext);
  QString getOriginFolder() const;
  QString getResourcePath() const;
  void resetData();
  void setData(const QString& aOriginFolder, const QString& aResourcePath, const bool aUseAlternativeModel = false);
  void simulateDropEvent(const QString& aOriginFolder, const QString& aResourcePath);

signals:
  void dropEventTriggered(const QString& aOldOriginFolder, const QString& aOldResourcePath, const QString& aNewOriginFolder, const QString& aNewResourcePath, const bool isCheckBoxChecked = false);
  void checkBoxStateChangedTriggered(const bool aIsActive);

protected:
  void dragEnterEvent(QDragEnterEvent* aEvent) override;
  void dragMoveEvent(QDragMoveEvent* aEvent) override;
  void dropEvent(QDropEvent* aEvent) override;

private:
  BCGroupWidgetCallContext mCallContext{BCGroupWidgetCallContext::UNDEFINED};
  QString mOriginFolder;
  QString mResourcePath;

  // Event handlers
  void checkBoxStateChanged(int aNewState);

  // GUI
  void tweakWidgetsVisibility(const bool aShouldViewDropZoneOnly, const QString& aNewOriginText = QString(), const QString& aNewResourceText = QString(), const bool aUseAlternativeModel = false);
  QCheckBox* createCheckBox(const QString& lText, QGridLayout& aLayout);
};
