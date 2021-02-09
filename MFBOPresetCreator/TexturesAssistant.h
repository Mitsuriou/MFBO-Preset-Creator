#pragma once

#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"

class TexturesAssistant : public QDialog
{
  Q_OBJECT

public:
  TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  bool mHasUserDoneSomething;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface();
};
