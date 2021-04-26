#pragma once

#include "Enum.h"

class LangManager final
{
public:
  static ApplicationLanguage getSystemLanguage();

private:
  ~LangManager() = delete;
  explicit LangManager(){};
};
