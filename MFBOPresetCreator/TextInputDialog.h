#include "Enum.h"
#include <QDialog>

class TextInputDialog final : public QDialog
{
  Q_OBJECT

public:
  explicit TextInputDialog(const QString& aTitle, const QString& aLabel, const GUITheme& aAppTheme, QWidget* aParent);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  const GUITheme mAppTheme;

  void setWindowProperties(const QString& aTitle);
  void initializeGUI(const QString& aLabel);

  void updateAddButtonStatus(const QString& aText);

signals:
  void getTextValue(const QString& aText);
};
