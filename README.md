# Mitsuriou's Follower Bodies Overhaul: Preset Creator
MFBOPC is a GUI application that should able anyone to change a body mesh of a follower or an NPC to a CBBE 3BBB 3BA body mesh.
In addition, the user is able to generate files that the BodySlide application is able to read, to custom the follower's or NPC's body shape.

MFBOPC has been designed to work with **Skyrim: Special Edition only**. The application might be able to work for other games with future updated...

At this time, the application is only capable of changing the body, hands and feet meshes and cannot convert any cloth, outfit or armor: it is a difficult and very time consuming task even for a Human being, I'm not sure of being able to make the application that powerful.

# How to install and run MFBO: PC
### Download
Choose the **latest release** on https://github.com/Mitsuriou/MFBO-Preset-Creator/releases and download either the **.7z** file or the **.zip** file that is named **MFBO.Preset.Creator.[*version number*].[*7z|zip*]**. They are the same, but some people do not know how to open .7z files, so I try to always provide both alternatives.

### Install
The application is currently a simple executable file and not any installer is provided to the user. So, when the download is finished, unzip in the folder of your choice the content of the archive.

### Launch
To launch the application, simply click on the **MFBOPresetCreator.exe** file (the one with a black and white icon).

# Roadmap (not ordered by priority)
### GUI modifications
- [X] (1.7.4+) Separate the main GUI into tabs for the different body types
- [ ] Create the tabs for all the most popular body types
- [ ] Add compatibility with CBBE 3BBB SMP
- [X] (1.9.0) Allow the user to type different paths for the body, hands and feet
- [X] (1.7.3 & 1.7.4) Add a check system to know is the *"meshes/"* path is well filled and seems to be valid

### New tools
- [ ] Automatic conversion (choose a folder to automatically try to find where are located the meshes for the NPC that needs to be converted)

### Other cool features and improvements
- [ ] Better software linking with Mod Organizer 2
- [X] (1.7.4) Add an option to disable the automatic opening of the generated folder, after a generation
- [X] (1.8.2) Be able to import a custom *female_skeleton.nif* file
- [X] (1.8.4) Be able to add BodySlide presets filters (currently, there are only "CBBE" and "MFBO")
- [X] (1.7.4) Be able to set a default export path that would be chosen automatically when the application launches

### Developer side
- [ ] Create test classes
- [X] (1.7.6) Splashscreen when the application is being launched
- [X] (1.8.0) In-app update checker
- [ ] In-app automatic updater
- [ ] Executable to install the app on C:/ProgramFiles (InnoSetup?)
- [X] (1.7.4) Change the source code auto-formatter for more compatibility with Visual Studio 2019
