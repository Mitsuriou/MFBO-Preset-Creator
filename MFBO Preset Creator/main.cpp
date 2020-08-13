﻿#include "MFBOPresetCreator.h"
#include "Utils.h"

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
  // Create the main GUI
  QApplication lMainApplication(argc, argv);
  lMainApplication.setApplicationDisplayName("MFBOPC v." + Utils::getProgramVersion());
  lMainApplication.setApplicationVersion(Utils::getProgramVersion());
  lMainApplication.setWindowIcon(QIcon(":/software/icon"));

  // Launch the application
  MFBOPresetCreator lMainWindow;
  lMainWindow.show();
  return lMainApplication.exec();
}
