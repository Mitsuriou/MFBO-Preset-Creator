#pragma once
#include <QWidget>

class ThemeCard : public QWidget
{
  Q_OBJECT

public:
  explicit ThemeCard(const QString& aThemeName, QWidget* aParent);

signals:
  void loadQSSTheme(QString aThemeName);

protected:
  void resizeEvent(QResizeEvent* aEvent) override;

private:
  QString mThemeName;
  QPixmap mPreviewImage;
};
