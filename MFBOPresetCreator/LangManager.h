#pragma once
#include "Enum.h"

class LangManager final
{
public:
  static ApplicationLanguage GetSystemLanguage();

private:
  ~LangManager() = delete;
  explicit LangManager(){};
};
