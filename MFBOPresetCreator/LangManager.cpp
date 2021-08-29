#include "LangManager.h"
#include "Utils.h"
#include <QLocale>

ApplicationLanguage LangManager::GetSystemLanguage()
{
  return Utils::GetStructLanguageFromName(QLocale::languageToString(QLocale::system().language()));
}