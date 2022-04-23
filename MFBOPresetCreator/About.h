#pragma once
#include "Struct.h"
#include <QDialog>

class About final : public QDialog
{
  Q_OBJECT

public:
  explicit About(QWidget* aParent, const Struct::Settings& aSettings);

private:
  const Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();

  // Prevent object copy
  About(const About&) = delete;
  About& operator=(const About&) = delete;
};
