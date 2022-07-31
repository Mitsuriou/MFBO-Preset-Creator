#pragma once

#include <QPlainTextEdit>

class PlainTextEdit : public QPlainTextEdit
{
  Q_OBJECT

public:
  explicit PlainTextEdit(const QString& aText, QWidget* aParent = nullptr);
  explicit PlainTextEdit(QWidget* aParent = nullptr);

protected:
  bool event(QEvent* aEvent) override;
};
