#pragma once
#include <QWidget>

class ThemeCard final : public QWidget
{
  Q_OBJECT

public:
  explicit ThemeCard(QWidget* aParent, const QString& aThemeName, const int aCardIndex);

signals:
  void askThemeChange(const QString& aThemeName, const int aCardIndex);

protected:
  void resizeEvent(QResizeEvent* aEvent) override;

private:
  QString mThemeName;
  QPixmap mPreviewImage;
};
