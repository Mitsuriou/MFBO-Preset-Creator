#pragma once
#include "Enum.h"

class LangManager final
{
public:
  static ApplicationLanguage GetSystemLanguage();

private:
  explicit LangManager() {}
  ~LangManager() = delete;
};
