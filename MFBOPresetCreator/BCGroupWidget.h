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

signals:
  void removePressed(const QString& aOriginFolder, const QString& aRessourcePath);
  void dropEventTriggered(const QString& aOriginFolder, const QString& aRessourcePath);

private:
  BCGroupWidgetCallContext mCallContext{BCGroupWidgetCallContext::UNDEFINED};

  void removeButtonClicked();
  void dropEventTrigerredReceiver(const QString& aOldOriginFolder, const QString& aOldRessourcePath, const QString& aNewOriginFolder, const QString& aNewRessourcePath);

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);
};
