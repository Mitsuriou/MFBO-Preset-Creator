#pragma once
#include "TitleDialog.h"
#include <QGridLayout>
#include <QListWidget>

class TargetMeshesPicker final : public TitleDialog
{
  Q_OBJECT

  // DEBUG
public:
  void debugtest();
  // END DEBUG

public:
  explicit TargetMeshesPicker(QWidget* aParent,
                              const Struct::Settings& aSettings,
                              std::map<QString, QString>* aLastPaths,
                              const BodyNameVersion& aPreSelectedBody,
                              const FeetNameVersion& aPreSelectedFeet);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
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

signals:
  void valuesChosen(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);
};
