#include "LangManager.h"
#include "Utils.h"
#include <QLocale>

ApplicationLanguage LangManager::getSystemLanguage()
{
  return Utils::getStructLanguageFromName(QLocale::languageToString(QLocale::system().language()));
}