#include "About.h"

About::About(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void About::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("About"));
  this->setWindowIcon(QIcon(QPixmap(":/black/info-circle")));

  // Hacky window's background color change for Windows Vista theme
  if (this->mSettings.appTheme == GUITheme::WindowsVista)
  {
    this->setStyleSheet("background-color: white;");
  }
}

void About::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QVBoxLayout(this)};
  lMainVertical->setAlignment(Qt::AlignCenter);
  lMainVertical->setContentsMargins(20, 20, 20, 20);

  // Hacky links' colors override for some themes
  auto lLinksColorOverride{QString()};
  if (this->mSettings.appTheme == GUITheme::MitsuriouLightTheme || this->mSettings.appTheme == GUITheme::MitsuriouDarkTheme)
  {
    lLinksColorOverride = " style='color: #e95985;'";
  }

  // Build the description
  auto lDescription(
    QStringLiteral(
      "<h1 style='text-align: center; vertical-align: top; padding: 0px; margin: 0px;'><img src=':/application/icon' alt='~MFBOPC icon~' height='48' width='48'> About this application</h1><br />"
      "<p style='padding: 0px; margin: 0px;'>"
      "Mitsuriou's Follower Bodies Overhaul Preset Creator (MFBOPC) is an application "
      "created by Dylan Jacquemin (also known under the nickname <a%1 href='https://www.nexusmods.com/users/37026145'>Mitsuriou</a>).<br />"
      "This application has been developed to be provided for free to any user that wants to use the application.<br />"
      "The totality of the source code is available on <a%1 href='https://github.com/Mitsuriou/MFBO-Preset-Creator'>GitHub.com</a>.<br /><br />"
      "Ressources used to make this application:"
      "<ul style='margin: 0;'><li><a%1 href='https://www.qt.io/download-open-source'>Qt</a> (free version) is used for the Graphical User Interface (GUI).</li>"
      "<li>Some icons were taken from Google's <a%1 href='https://material.io/resources/icons'>Material Design</a> website.</li>"
      "<li>Some icons were taken from <a%1 href='https://materialdesignicons.com'>MaterialDesignIcons</a> website.</li>"
      "<li>Some GUI themes were taken from <a%1 href='https://github.com/6788-00'>6788-00's GitHub repository</a>.</li>"
      "<li>Some GUI themes were taken from <a%1 href='https://github.com/Alexhuszagh/BreezeStyleSheets'>Alexhuszagh/BreezeStyleSheets GitHub page</a>.</li>"
      "<li>Some GUI themes were taken from <a%1 href='https://github.com/QuasarApp/QStyleSheet'>QuasarApp/QStyleSheet GitHub page</a>.</li>"
      "</ul><br />"
      "Ressources bundled in this application:"
      "<ul style='margin: 0;'><li>Some BodySlide (OSP and XML) files that are generated with MFBOPC were taken from the "
      "<a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/30174'>CBBE 3BBB</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>Some BodySlide (OSP and XML) files that are generated with MFBOPC were taken from the "
      "<a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/29023'>CBBE SMP (3BBB)</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>Some BodySlide (OSP and XML) files that are generated with MFBOPC were taken from the "
      "<a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/31126'>BHUNP (UUNP Next Generation) SSE</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>The \"female_skeleton.nif\" file has been taken from the "
      "<a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/1988'>XP32 Maximum Skeleton Special Extended - XPMSSE</a> "
      "mod on NexusMod. The file has not been modified.</li>"
      "</ul></p>")
      .arg(lLinksColorOverride));

  auto lTextContainer{new QLabel(this)};
  lTextContainer->setTextFormat(Qt::RichText);
  lTextContainer->setTextInteractionFlags(Qt::TextBrowserInteraction);
  lTextContainer->setOpenExternalLinks(true);
  lTextContainer->setText(lDescription);
  lTextContainer->adjustSize();
  lTextContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lMainVertical->addWidget(lTextContainer);
}
