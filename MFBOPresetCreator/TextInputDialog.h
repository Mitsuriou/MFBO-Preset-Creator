#include "Enum.h"
#include <QDialog>

class TextInputDialog final : public QDialog
{
  Q_OBJECT

public:
  explicit TextInputDialog(const QString& aTitle, const QString& aLabel, const GUITheme& aAppTheme, QWidget* aParent);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

#pragma region PRIVATE_SLOTS
  void updateAddButtonStatus(const QString& aText);
#pragma endregion PRIVATE_SLOTS

private:
  GUITheme mAppTheme;

  void setWindowProperties(const QString& aTitle);
  void initializeGUI(const QString& aLabel);

signals:
  void getTextValue(const QString&);
};
