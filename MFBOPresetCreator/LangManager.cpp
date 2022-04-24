#include "LangManager.h"
#include "Utils.h"
#include <QLocale>

namespace LangManager
{
  ApplicationLanguage GetSystemLanguage()
  {
    return Utils::GetStructLanguageFromName(QLocale::languageToString(QLocale::system().language()));
  }
}