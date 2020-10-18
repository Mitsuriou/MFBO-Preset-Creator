#pragma once

#include <QtTest/QtTest>

class UtilsTest : public QObject
{
  Q_OBJECT

private slots:
  void toUpper();
  void cleanupTestCase();
};

void UtilsTest::toUpper()
{
  QString str = "Hello";
  QCOMPARE(str.toUpper(), QString("HELLO"));
}

void UtilsTest::cleanupTestCase()
{
  system("Pause. All tests have been executed.");
}

QTEST_MAIN(UtilsTest)
#include "UtilsTest.moc"
