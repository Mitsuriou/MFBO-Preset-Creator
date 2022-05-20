#include "About.h"
#include "Enum.h"
#include "Utils.h"
#include <QCloseEvent>

About::About(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, tr("About"), "info-circle", aSettings, aLastPaths)
{
  // Hacky window's background color change for Windows Vista theme
  if (this->settings().display.applicationTheme == GUITheme::DEFAULT_OS_THEME)
  {
    this->setStyleSheet("background-color: white;");
  }

  // Build the window's interface
  this->hideTitle();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void About::closeEvent(QCloseEvent* aEvent)
{
  aEvent->accept();
}

void About::initializeGUI()
{
  // Set a layout for this dialog box
  const auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setAlignment(Qt::AlignCenter);
  lMainLayout->setContentsMargins(10, 10, 10, 10);
  lMainLayout->setSpacing(10);
  this->getCentralWidget()->setLayout(lMainLayout);

  // Hacky links' colors override for some themes
  QString lLinksColorOverride;
  if (this->settings().display.applicationTheme == GUITheme::MITSURIOU_BLACK_THEME)
  {
    lLinksColorOverride = " style='color: #3991ff;'";
  }
  else if (this->settings().display.applicationTheme == GUITheme::MITSURIOU_DARK_THEME
           || this->settings().display.applicationTheme == GUITheme::MITSURIOU_LIGHT_THEME)
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
      "Resources used to make this application:"
      "<ul style='margin: 0;'><li><a%1 href='https://www.qt.io/download-open-source'>Qt</a> (free version) is used for the Graphical User Interface (GUI).</li>"
      "<li>Some icons were taken from <a%1 href='https://fonts.google.com/icons'>Google Fonts</a> website.</li>"
      "<li>Some icons were taken from <a%1 href='https://materialdesignicons.com'>MaterialDesignIcons</a> website.</li>"
      "<li>Some GUI themes were taken from <a%1 href='https://github.com/6788-00'>6788-00's GitHub repository</a>.</li>"
      "<li>Some GUI themes were taken from <a%1 href='https://github.com/Alexhuszagh/BreezeStyleSheets'>Alexhuszagh/BreezeStyleSheets GitHub page</a>.</li>"
      "</ul><br />"
      "Resources bundled in this application:"
      "<ul style='margin: 0;'>"
      "<li>The major parts of the BodySlide (OSP and XML) files that are generated with MFBOPC were taken from:"
      "<ul style='margin: 0;'>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/198'>Caliente's Beautiful Bodies Enhancer -CBBE-</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/30174'>CBBE 3BBB</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/29023'>CBBE SMP (3BBB)</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/31126'>BHUNP (UUNP Next Generation) SSE</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/51173'>Mimir Ebonic Body (Custom CBBE sliders and CBBE preset) V.2</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/62052'>COCO Body (3BBB) SE</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/59287'>BHUNP Lite - AKA ASDASDF Body (Also Myst's Settings)</a> mod and modified by Mitsuriou.</li>"
      "<li style='margin-top: 5px;'>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/30072'>MSF - More Sliders for Feet CBBE - BHUNP (High Heel)</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.sunkeumjeong.com/post/hg-feet-and-toes-bhunp-se'>HG Feet and Toes BHUNP SE</a> mod and modified by Mitsuriou.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/50679'>HGF Improvement Project</a> mod and modified by Mitsuriou.</li>"
      "</ul></li>"
      "<li style='margin-top: 5px;'>The \"skeleton_female.nif\" and \"skeletonbeast_female.nif\" files have been taken from:"
      "<ul style='margin: 0;'>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/1988'>XP32 Maximum Skeleton Special Extended - XPMSSE</a> mod. The files have not been modified.</li>"
      "<li>the <a%1 href='https://www.nexusmods.com/skyrimspecialedition/mods/59284'>Vera's Female Skeleton</a> mod. The files have not been modified.</li>"
      "</ul></li></p></ul>")
      .arg(lLinksColorOverride)};

  auto lTextContainer{new QLabel(this)};
  lTextContainer->setTextFormat(Qt::RichText);
  lTextContainer->setTextInteractionFlags(Qt::TextBrowserInteraction);
  lTextContainer->setOpenExternalLinks(true);
  lTextContainer->setText(lDescription);
  lTextContainer->adjustSize();
  lTextContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  qobject_cast<QVBoxLayout*>(this->getCentralLayout())->addWidget(lTextContainer);
}
