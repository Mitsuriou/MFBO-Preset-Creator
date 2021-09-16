#pragma once

#include "Enum.h"
#include "Struct.h"
#include <QWidget>

class BCGroupWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCGroupWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aSectionTitle, const QString& aSectionIconName, const BCGroupWidgetCallContext& aCallContext);
  void setData(const Struct::BatchConversionPresetData& aData);
  void removeData();

signals:
  void removePressed(const QString& aOriginFolder, const QString& aRessourcePath);
  void dropEventTriggered(const QString& aOriginFolder, const QString& aRessourcePath, const bool isCheckBoxChecked);
  void checkBoxStateChangedTriggered(const bool aIsActive);

private:
  BCGroupWidgetCallContext mCallContext{BCGroupWidgetCallContext::UNDEFINED};

  void dropEventTrigerredReceiver(const QString& aOldOriginFolder, const QString& aOldRessourcePath, const QString& aNewOriginFolder, const QString& aNewRessourcePath, const bool isCheckBoxChecked);
  void checkBoxStateChangedReceiver(const bool aIsActive);

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);
};
