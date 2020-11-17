#pragma once

#include "DataLists.h"
#include "Settings.h"
#include "Utils.h"
#include "stdafx.h"

class Tab : public QWidget
{
  Q_OBJECT

public:
  void updateSettings(Struct::Settings aSettings);
  virtual void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList) = 0;

protected:
  explicit Tab(QWidget* aParent, const Struct::Settings& aSettings);
  Struct::Settings mSettings;

  QVBoxLayout* getMainLayout();

private:
  QVBoxLayout* mMainVertical;

private slots:
  void mouseCursorPressed();
  void mouseCursorReleased();
};
