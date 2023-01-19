#include "PlainTextEdit.h"
#include <QEvent>

PlainTextEdit::PlainTextEdit(const QString& aText, QWidget* aParent)
  : QPlainTextEdit(aText, aParent)
{
}

PlainTextEdit::PlainTextEdit(QWidget* aParent)
  : PlainTextEdit("", aParent)
{
}

bool PlainTextEdit::event(QEvent* aEvent)
{
  // Fix placeholder colour set incorrectly (QTBUG-95297)
  if (aEvent->type() == QEvent::PaletteChange)
  {
    auto lCurrentPalette{palette()};
    for (auto lRole : {QPalette::Active, QPalette::Disabled, QPalette::Inactive})
    {
      if (lCurrentPalette.color(lRole, QPalette::ColorRole::PlaceholderText).alpha() == 255)
      {
        QColor lColor{lCurrentPalette.color(lRole, QPalette::ColorRole::Text)};
        lColor.setAlpha(128);
        lCurrentPalette.setColor(lRole, QPalette::ColorRole::PlaceholderText, lColor);
        setPalette(lCurrentPalette);
      }
    }
  }

  return QPlainTextEdit::event(aEvent);
}
