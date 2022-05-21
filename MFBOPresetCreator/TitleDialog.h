#pragma once
#include "Struct.h"
#include <QDialog>
#include <QHBoxLayout>

class TitleDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TitleDialog(QWidget* aParent,
                       const QString& aTitle,
                       const QString& aIconName,
                       const Struct::Settings& aSettings,
                       std::map<QString, QString>* aLastPaths,
                       const int aMinimumWidth = -1,
                       const int aMinimumHeight = -1);

  const Struct::Settings& settings();
  std::map<QString, QString>* lastPaths();

protected:
  virtual void closeEvent(QCloseEvent* aEvent) override = 0;
  void reject() override;

  // Modify title behaviour
  void overrideContentTitle(const QString& aTitle);
  void hideTitle();
  void showTitle();

  // Get some GUI elements
  QWidget* getCentralWidget();
  QLayout* getCentralLayout();
  QHBoxLayout* getButtonsLayout();

  // Utils functions
  const QString& getThemedResourcePath() const;

private:
  // Common attributes
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths{nullptr};

  const QString mApplicationIconResourceFolder;

  void setWindowProperties(const QString& aTitle,
                           const QString& aIconName,
                           const int aMinimumWidth,
                           const int aMinimumHeight);
  void constructGUI(const QString& aTitle);
};
