#pragma once

#include <QLineEdit>

class LineEdit : public QLineEdit
{
  Q_OBJECT

public:
  explicit LineEdit(const QString& aText, QWidget* aParent = nullptr);
  explicit LineEdit(QWidget* aParent = nullptr);

protected:
  bool event(QEvent* aEvent) override;
};
