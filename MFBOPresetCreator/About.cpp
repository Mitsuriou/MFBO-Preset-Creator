#include "About.h"
#include "Enum.h"
#include "Utils.h"

About::About(QWidget* aParent, const Struct::Settings& aSettings)
  : QDialog(aParent)
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
  this->setWindowTitle(tr("About"));
  this->setWindowIcon(QIcon(QPixmap(":/black/info-circle")));

  // Hacky window's background color change for Windows Vista theme
  if (this->mSettings.appTheme == GUITheme::WINDOWS_VISTA)
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
  if (this->mSettings.appTheme == GUITheme::MITSURIOU_BLACK_THEME)
  {
    lLinksColorOverride = " style='color: #3991ff;'";
  }
  else if (this->mSettings.appTheme == GUITheme::MITSURIOU_DARK_THEME
           || this->mSettings.appTheme == GUITheme::MITSURIOU_LIGHT_THEME)
  {
    lLinksColorOverride = " style='color: #e95985;'";
  }

  // Build the description
  auto lDescription{
    QString(
      "<h1 style='text-align: center; vertical-align: top; padding: 0px; margin: 0px;'><img src=':/application/icon' alt='~MFBOPC icon~' height='48' width='48'> About this application</h1><br />"
      "<p style='padding: 0px; margin: 0px;'>"
      "Mitsuriou's Follower Bodies Overhaul Preset Creator (MFBOPC) is an application "
      "created by Dylan Jacquemin (also known under the nickname <a%1 href='https://www.nexusmods.com/users/37026145'>Mitsuriou</a>).<br />"
      "This application has been developed to be provided for free to any user that wants to use the application.<br />"
      "The totality of the source code is available on <a%1 href='https://github.com/Mitsuriou/MFBO-Preset-Creator'>GitHub.com</a>.<br /><br />"
      "Acknowledgements:"
      "<ul style='margin: 0;'><li>Traditional Chinese (Taiwan) translation by the great <a%1 href='https://www.nexusmods.com/skyrimspecialedition/users/84981953'>Sephirothryan</a>.</li>"
      "</ul><br />"
      "Ressources used to make this application:"
      "<ul style='margin: 0;'><li><a%1 href='https://www.qt.io/download-open-source'>Qt</a> (free version) is used for the Graphical User Interface (GUI).</li>"
      "<li>Some icons were taken from <a%1 href='https://fonts.google.com/icons'>Google Fonts</a> website.</li>"
      "<li>Some icons were taken from <a%1 href='https://materialdesignicons.com'>MaterialDesignIcons</a> website.</li>"
      "<li>Some GUI themes were taken from <a%1 href='https://github.com/6788-00'>6788-00's GitHub repository</a>.</li>"
      "<li>Some GUI themes were taken from <a%1 href='https://github.com/Alexhuszagh/BreezeStyleSheets'>Alexhuszagh/BreezeStyleSheets GitHub page</a>.</li>"
      "</ul><br />"
      "Ressources bundled in this application:<ul style='margin: 0;'>"
      "<li>The major parts of the BodySlide (OSP and XML) files that are generated with MFBOPC were taken from:<ul style='margin: 0;'>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/198'>Caliente's Beautiful Bodies Enhancer -CBBE-</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/30174'>CBBE 3BBB</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/29023'>CBBE SMP (3BBB)</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/31126'>BHUNP (UUNP Next Generation) SSE</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/51173'>Mimir Ebonic Body (Custom CBBE sliders and CBBE preset) V.2</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/30072'>MSF - More Sliders for Feet CBBE - BHUNP (High Heel)</a> mod on NexusMod and modified by Dylan Jacquemin.</li>"
      "</ul></li><li>The \"skeleton_female.nif\" and \"skeletonbeast_female.nif\" files have been taken from<br />"
      "the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/1988'>XP32 Maximum Skeleton Special Extended - XPMSSE</a> "
      "mod on NexusMod. The files have not been modified.</li>"
      "</ul></p>")
      .arg(lLinksColorOverride)};

  auto lTextContainer{new QLabel(this)};
  lTextContainer->setTextFormat(Qt::RichText);
  lTextContainer->setTextInteractionFlags(Qt::TextBrowserInteraction);
  lTextContainer->setOpenExternalLinks(true);
  lTextContainer->setText(lDescription);
  lTextContainer->adjustSize();
  lTextContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lMainVertical->addWidget(lTextContainer);
}
