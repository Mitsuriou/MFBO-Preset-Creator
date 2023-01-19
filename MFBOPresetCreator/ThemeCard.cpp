#include "ThemeCard.h"
#include "ComponentFactory.h"
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

ThemeCard::ThemeCard(QWidget* aParent, const QString& aThemeName, const int aCardIndex)
  : QWidget(aParent)
  , mThemeName(aThemeName)
  , mPreviewImage(QPixmap(QString(":/qss-previews/%2").arg(aThemeName)))
{
  auto lWrapper{new QVBoxLayout(this)};
  this->setLayout(lWrapper);

  // Preview image
  const auto lImageLabel{new QLabel(this)};
  lImageLabel->setObjectName(QStringLiteral("image_label"));
  lWrapper->addWidget(lImageLabel);

  // Load radio button
  const auto lRadioButton{new QRadioButton(aThemeName, this)};
  lRadioButton->setCursor(Qt::CursorShape::PointingHandCursor);
  lRadioButton->setObjectName(QStringLiteral("selector_%1").arg(QString::number(aCardIndex)));
  lWrapper->addWidget(lRadioButton);

  // Event binding
  QObject::connect(lRadioButton, &QRadioButton::clicked, this, [=]() {
    // If the new state is "unchecked", skip the event
    if (!lRadioButton->isChecked())
    {
      lRadioButton->setChecked(true);
      return;
    }

    emit askThemeChange(this->mThemeName, aCardIndex);
  });

  // Simulate a resize event
  this->resizeEvent(nullptr);
}

void ThemeCard::resizeEvent(QResizeEvent*)
{
  auto lImageLabel{this->findChild<QLabel*>("image_label")};

  const auto lOriginalWidth{this->mPreviewImage.width()};
  const auto lOriginalHeight{this->mPreviewImage.height()};

  lImageLabel->setMinimumSize(1, lImageLabel->width() / (lOriginalWidth * lOriginalHeight));

  lImageLabel->setPixmap(this->mPreviewImage.scaledToWidth(lImageLabel->width(),
                                                           Qt::TransformationMode::SmoothTransformation));
}
