#pragma once

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QtTest/QtTest>

#include "./MFBOPresetCreator/DataLists.h"

class UtilsTest : public QObject
{
  Q_OBJECT

private slots:
  void randomTest();
  void cleanupTestCase();
};

void UtilsTest::randomTest()
{
  QStringList lExpectedList;
  lExpectedList.append(QString("1.40"));
  lExpectedList.append(QString("1.50"));
  lExpectedList.append(QString("1.51 - 1.52"));

  auto lObtainedList = DataLists::getCBBE3BBBVersions();

  QCOMPARE(lExpectedList, lObtainedList);
}

void UtilsTest::cleanupTestCase()
{
  qDebug() << "All tests have been executed.";
  system("pause");
}

QTEST_MAIN(UtilsTest)
#include "UtilsTest.moc"
