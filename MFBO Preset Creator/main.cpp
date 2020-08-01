#include "MFBOPresetCreator.h"
#include "stdafx.h"
#include "Utils.h"

#include <QtWidgets/QApplication>
//#include <QFile>

int main(int argc, char* argv[])
{
  QApplication lMainApplication(argc, argv);
  lMainApplication.setApplicationDisplayName("v." + Utils::getProgramVersion());
  lMainApplication.setApplicationVersion(Utils::getProgramVersion());

  // Change the theme of the application
  //auto lStylesheet = new QFile(":/software/light_style");
  //if (lStylesheet->open(QIODevice::ReadOnly))
  //{
  //  lMainApplication.setStyleSheet(lStylesheet->readAll());
  //  lStylesheet->close();
  //}
  //delete lStylesheet;
  //lStylesheet = nullptr;

  MFBOPresetCreator w;
  w.show();
  return lMainApplication.exec();
}
