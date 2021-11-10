#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>

class TargetMeshesPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit TargetMeshesPicker(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;

  void setWindowProperties();
  void initializeGUI();

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);

  explicit TargetMeshesPicker(const TargetMeshesPicker&) = delete;
  TargetMeshesPicker& operator=(const TargetMeshesPicker&) = delete;

signals:
  void valuesChosen(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);
};
