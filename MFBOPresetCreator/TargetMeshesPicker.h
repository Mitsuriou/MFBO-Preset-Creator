#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>
#include <QListWidget>

class TargetMeshesPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit TargetMeshesPicker(QWidget* aParent,
                              const Struct::Settings& aSettings,
                              const BodyNameVersion& aPreSelectedBody,
                              const FeetNameVersion& aPreSelectedFeet);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  const BodyNameVersion mOriginalBody;
  const FeetNameVersion mOriginalFeet;

  // Body related list widgets
  int mCurrentBodyBase;
  int mCurrentBodyVersionNumber;
  int mCurrentBodyVersionName;
  // Feet related list widgets
  int mCurrentFeetBase;
  int mCurrentFeetVersionNumber;
  int mCurrentFeetVersionName;

  // Widgets
  QListWidget* mListBodyName{nullptr};
  QListWidget* mListBodyVersion{nullptr};
  QListWidget* mListBodyVariantName{nullptr};
  QListWidget* mListFeetName{nullptr};
  QListWidget* mListFeetVersion{nullptr};
  QListWidget* mListFeetVariantName{nullptr};

  // Smarter lists behavior
  bool mIsWindowInitialized{false};
  QString mLastSelectedFeetName;
  QString mLastSelectedFeetVariant;

  // Common functions
  void setWindowProperties();
  void initializeGUI();

  BodyVariant getChosenBodyVariant() const;
  BodyNameVersion getChosenBodyName() const;
  FeetNameVersion getChosenFeetName() const;

  // GUI widgets events
  void validateAndClose();
  // Body GUI widgets events
  void bodyNameIndexChanged(const int aNewIndex);
  void bodyVersionIndexChanged(const int aNewIndex);
  void bodyVariantIndexChanged(const int aNewIndex);
  // Feet GUI widgets events
  void feetNameIndexChanged(const int aNewIndex);
  void feetVersionIndexChanged(const int aNewIndex);
  void feetVariantIndexChanged(const int aNewIndex);

  explicit TargetMeshesPicker(const TargetMeshesPicker&) = delete;
  TargetMeshesPicker& operator=(const TargetMeshesPicker&) = delete;

signals:
  void valuesChosen(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);
};
