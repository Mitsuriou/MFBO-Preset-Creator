#pragma once

#include <QGroupBox>

class GroupBox final : public QGroupBox
{
  Q_OBJECT

public:
  explicit GroupBox(QWidget* aParent,
                    const QString& aTitle,
                    const QString& aIconName,
                    const QString& aIconFolder,
                    const int aFontSize,
                    const QString& aObjectName = QString(),
                    const bool aIsDisabled = false);

  void setExpandState(const bool aMustExpand);

private:
  int mFontSize;
  int mExpandedHeight{-1};
};
