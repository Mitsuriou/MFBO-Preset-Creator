#pragma once
#include "TitleDialog.h"

class About final : public TitleDialog
{
  Q_OBJECT

public:
  explicit About(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  void initializeGUI();
};
