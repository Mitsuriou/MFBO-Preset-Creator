#pragma once

#include "Enum.h"
#include "Struct.h"
#include <QWidget>

class BCGroupWidget final : public QWidget
{
  Q_OBJECT

public:
  explicit BCGroupWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aSectionTitle, const QString& aSectionIconName, const BCGroupWidgetCallContext& aCallContext);
  void setData(const Struct::BatchConversionPresetData& aData);
  void removeData();
  void duplicateData();

signals:
  void removePressed(const QString& aOriginFolder, const QString& aResourcePath);
  void duplicatePressed(const QString& aOriginFolder, const QString& aResourcePath);
  void dropEventTriggered(const QString& aOriginFolder, const QString& aResourcePath, const bool isCheckBoxChecked);
  void checkBoxStateChangedTriggered(const bool aIsActive);

private:
  BCGroupWidgetCallContext mCallContext{BCGroupWidgetCallContext::UNDEFINED};

  void dropEventTrigerredReceiver(const QString& aOldOriginFolder, const QString& aOldResourcePath, const QString& aNewOriginFolder, const QString& aNewResourcePath, const bool isCheckBoxChecked);
  void checkBoxStateChangedReceiver(const bool aIsActive);

  explicit BCGroupWidget(const BCGroupWidget&) = delete;
  BCGroupWidget& operator=(const BCGroupWidget&) = delete;
};
