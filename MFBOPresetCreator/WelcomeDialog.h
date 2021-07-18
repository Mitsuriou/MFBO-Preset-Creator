#pragma once
#include "Struct.h"
#include <QDialog>

class WelcomeDialog final : public QDialog
{
  Q_OBJECT

public:
  explicit WelcomeDialog(QWidget* aParent, const Struct::Settings& aSettings);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();
};
