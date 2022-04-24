#pragma once
#include "Struct.h"
#include <QDialog>

class TitleDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TitleDialog(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

  const Struct::Settings& settings();
  std::map<QString, QString>* lastPaths();

protected:
  virtual void closeEvent(QCloseEvent* aEvent) override = 0;
  void reject() override;

private:
  // Common attributes
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths{nullptr};
};
