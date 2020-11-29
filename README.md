# Mirrors
[![CodeFactor](https://www.codefactor.io/repository/github/mitsuriou/mfbo-preset-creator/badge)](https://www.codefactor.io/repository/github/mitsuriou/mfbo-preset-creator)

The source code is available on:
- [Github](https://github.com/Mitsuriou/MFBO-Preset-Creator)
- [GitLab](https://gitlab.com/Mitsuriou/MFBO-Preset-Creator)

# Mitsuriou's Follower Bodies Overhaul: Preset Creator
MFBOPC is a relatively simple GUI application that should allow anyone to change a body mesh of a follower or an NPC to another body mesh (currently supporting CBBE 3BBB 3BA, CBBE 3BBB SMP and BHUNP SSE).
In addition, the user will generate files that the BodySlide application is able to read, to easily custom the follower's or NPC's body shape.

MFBOPC has been designed to work with **Skyrim: Special Edition only**. The application might be able to work for other games with future updates...

At this time, the application is only capable of changing the body, hands and feet meshes and cannot convert any cloth, outfit or armor: it is a difficult and very time consuming task even for a Human being, I'm not sure of being able to make the application that is that powerful.

# How to install and run MFBOPC
### Download
Find the **latest release** on [Github](https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/latest) and download either the **.7z** file or the **.zip** file that is named **MFBO.Preset.Creator.[*7z|zip*]**. They provide the same exact application, but some people do not know how to open **.7z** files, so I try to always provide both alternatives.

### Installation
The application is currently a simple executable file and not any installer is provided to the user. So, when the download is finished, unzip in the directory of your choice the content of the archive.
Note: you cannot execute the app from the archive itself, it really needs to be extracted on your computer.

### Launch
To launch the application, simply click on the **MFBOPresetCreator.exe** file (the one with a black and white icon).

# Development roadmap (not ordered by priority and not exhaustive)
### Ressources
- [ ] User guides and video tutorials

### Graphical User Interface (GUI) improvements
- [X] (1.7.3 & 1.7.4) Add a check system to know is the *"meshes/"* path is well filled and seems to be valid
- [X] (1.9.0) Allow the user to type different paths for the body, hands and feet
- [X] (2.0.0) Add compatibility with CBBE 3BBB SMP
- [X] (2.0.0) Add compatibility for all the most popular body types

### New tools
- [X] (1.9.0) Automatic conversion (choose a directory to automatically try to find where are located the meshes for the NPC that needs to be converted)

### Other cool features and improvements
- [X] (1.7.4) Add an option to disable the automatic opening of the generated folder, after a generation
- [X] (1.7.4) Be able to set a default export path that would be chosen automatically when the application launches
- [X] (1.8.2) Be able to import a custom *female_skeleton.nif* file
- [X] (1.8.4) Be able to add BodySlide presets filters (currently, there are only "CBBE" and "MFBO")
- [X] (1.9.1) Be able to generate more then one preset in the output directory.
- [ ] Batch conversion (multiple conversions at once)
- [ ] Better software linking with Mod Organizer 2

### Developer side
- [X] (1.7.4) Change the source code auto-formatter for more compatibility with Visual Studio 2019
- [X] (1.7.6) Splashscreen when the application is being launched
- [X] (1.8.0) In-app update checker
- [ ] In-app automatic updater
- [ ] Executable to install the app on C:/ProgramFiles (InnoSetup?)
- [ ] Create test classes
