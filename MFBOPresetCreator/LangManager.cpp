#include "LangManager.h"
#include "_Utils.h"
#include <QLocale>

namespace LangManager
{
  ApplicationLanguage GetSystemLanguage()
  {
    return Utils::GetStructLanguageFromName(QLocale::languageToString(QLocale::system().language()));
  }
}