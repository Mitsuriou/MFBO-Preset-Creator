#include "LineEdit.h"
#include <QEvent>

LineEdit::LineEdit(const QString& aText, QWidget* aParent)
  : QLineEdit(aText, aParent)
{}

LineEdit::LineEdit(QWidget* aParent)
  : LineEdit("", aParent)
{}

bool LineEdit::event(QEvent* aEvent)
{
  // Fix placeholder colour set incorrectly (QTBUG-95297)
  if (aEvent->type() == QEvent::PaletteChange)
  {
    auto lCurrentPalette{palette()};
    for (auto lRole : {QPalette::Active, QPalette::Disabled, QPalette::Inactive})
    {
      if (lCurrentPalette.color(lRole, QPalette::ColorRole::PlaceholderText).alpha() == 255)
      {
        auto lColor{lCurrentPalette.color(lRole, QPalette::ColorRole::Text)};
        lColor.setAlpha(128);
        lCurrentPalette.setColor(lRole, QPalette::ColorRole::PlaceholderText, lColor);
        setPalette(lCurrentPalette);
      }
    }
  }

  return QLineEdit::event(aEvent);
}
