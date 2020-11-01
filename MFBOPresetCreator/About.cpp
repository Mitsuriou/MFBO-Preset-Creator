#include "About.h"

About::About(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->show();
}

void About::setWindowProperties()
{
  this->setModal(true);
  this->setMaximumWidth(500);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("About"));

  // Hacky color change for Windows Vista theme
  if (this->mSettings.appTheme == GUITheme::WindowsVista)
  {
    QPalette lPalette;
    lPalette.setColor(QPalette::Window, Qt::white);
    this->setPalette(lPalette);
  }
}

void About::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QVBoxLayout(this)};
  lMainVertical->setAlignment(Qt::AlignCenter);

  // Build the description
  auto lDescription(
    QStringLiteral(
      "<h1 style=\"text-align: center; padding: 0px; margin: 0px;\">About this software</h1><br />"
      "<p style=\"padding: 0px; margin: 0px; margin-left: 25px; margin-right: 25px;\">"
      "Mitsuriou's Follower Bodies Overhaul Preset Creator (MFBOPC) is a software "
      "created by Dylan Jacquemin (also known under the nickname <a href=\"https://www.nexusmods.com/users/37026145\">Mitsuriou</a>).<br />"
      "This software has been developed to be provided for free to any user that wants to use the software.<br />"
      "The totality of the source code is available on "
      "<a href=\"https://github.com/Mitsuriou/MFBO-Preset-Creator\">GitHub.com</a>."
      "<br /><br />"
      "Ressources used to make this software:<br />"
      "&bull; <a href='https://www.qt.io'>Qt</a> (free version) is used for the Graphical User Iterface (GUI).<br />"
      "&bull; <a href='http://buildnumber.sourceforge.net/'>BuildNumber</a> is used for version number auto-incrementat.<br />"
      "&bull; All the icons were taken from <a href=\"https://materialdesignicons.com\">MaterialDesignIcons.com</a>.<br />"
      "&bull; Some GUI themes were taken from <a href=\"https://github.com/6788-00\">6788-00's GitHub repository</a>.<br />"
      "&bull; Some GUI themes were taken from <a href=\"https://github.com/Alexhuszagh/BreezeStyleSheets\">Alexhuszagh/BreezeStyleSheets GitHub page</a>.<br />"
      "&bull; Some GUI themes were taken from <a href=\"https://github.com/QuasarApp/QStyleSheet\">QuasarApp/QStyleSheet GitHub page</a>.<br />"
      "<br />"
      "Ressources bundled in this software:<br />"
      "&bull; The BodySlide (OSP and XML) files that are generated with MFBOPC were taken from the "
      "<a href=\"https://www.nexusmods.com/skyrimspecialedition/mods/30174\">CBBE 3BBB</a> mod on NexusMod "
      "and modified by Dylan Jacquemin.<br />"
      "&bull; The \"female_skeleton.nif\" file has been taken from the "
      "<a href=\"https://www.nexusmods.com/skyrimspecialedition/mods/1988\">XP32 Maximum Skeleton Special Extended - XPMSSE</a> "
      "mod on NexusMod. The file has not been modified."
      "</p>"));

  auto lTextContainer{new QLabel(this)};
  lTextContainer->setWordWrap(true);
  lTextContainer->setTextFormat(Qt::RichText);
  lTextContainer->setTextInteractionFlags(Qt::TextBrowserInteraction);
  lTextContainer->setOpenExternalLinks(true);
  lTextContainer->setText(lDescription);
  lTextContainer->adjustSize();
  lMainVertical->addWidget(lTextContainer);
}
