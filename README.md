# Mitsuriou's Follower Bodies Overhaul: Preset Creator
![Latest stable version](https://img.shields.io/github/v/release/Mitsuriou/MFBO-Preset-Creator?label=Latest%20stable%20version&logo=GitHub&logoColor=white&style=flat-square)
[![CodeFactor](https://img.shields.io/codefactor/grade/github/Mitsuriou/MFBO-Preset-Creator/main?label=Code%20quality(CodeFactor)&style=flat-square&logo=CodeFactor&logoColor=white)](https://www.codefactor.io/repository/github/mitsuriou/mfbo-preset-creator)
![All time downloads count](https://img.shields.io/github/downloads/Mitsuriou/MFBO-Preset-Creator/total?label=All%20time%20downloads%20count&logo=Github&logoColor=white&style=flat-square)
![Latest release downloads count](https://img.shields.io/github/downloads/Mitsuriou/MFBO-Preset-Creator/latest/total?label=Latest%20release%20downloads%20count&logo=Github&logoColor=white&style=flat-square)

MFBOPC is a relatively simple GUI application that should allow anyone to change a body mesh of a follower or an NPC to another body mesh (currently supporting CBBE 3BBB 3BA, CBBE 3BBB SMP and BHUNP SSE).
In addition, the user will generate files that the BodySlide application is able to read, to easily custom the follower's or NPC's body shape.

MFBOPC has been designed to work with **Skyrim: Special Edition only**. The application might be able to work for other games with future updates...

At this time, the application is only capable of changing the body, hands and feet meshes and cannot convert any cloth, outfit or armor: it is a difficult and very time consuming task even for a Human being, I'm not sure of being able to make the application that is that powerful.

# User guide and tutorials
You can find the "User guide and tutorials" [here](https://docs.google.com/document/d/1WpDKMk_WoPRrj0Lkst6TptUGEFAC2xYGd3HUBYxPQ-A/edit?usp=sharing). It is currently being written, but will be completed as things progress ;)

# Development roadmap (not ordered by priority and not exhaustive)
### Ressources
- [X] (2.5.1) User guide
- [ ] Tutorials

### Graphical User Interface (GUI) improvements
- [X] (1.7.3 & 1.7.4) Add a check system to know is the *"meshes/"* path is well filled and seems to be valid
- [X] (1.9.0) Allow the user to type different paths for the body, hands and feet
- [X] (2.0.0) Add compatibility with CBBE 3BBB SMP
- [X] (2.0.0+) Add compatibility for all the most popular body types

### New tools
- [X] (1.9.0) Automatic conversion (choose a directory to automatically try to find where are located the meshes for the NPC that needs to be converted)
- [X] (2.6.0) Tool to help users to swap converted follower's textures in a simpler way.

### Other cool features and improvements
- [X] (1.7.4) Add an option to disable the automatic opening of the generated folder, after a generation
- [X] (1.7.4) Be able to set a default export path that would be chosen automatically when the application launches
- [X] (1.8.2) Be able to import a custom *female_skeleton.nif* file
- [X] (1.8.4) Be able to add BodySlide presets filters (currently, there are only "CBBE" and "MFBO")
- [X] (1.9.1) Be able to generate more then one preset in the output directory.
- [ ] Batch conversion (multiple conversions at once)

### Developer side
- [X] (1.7.4) Change the source code auto-formatter for more compatibility with Visual Studio 2019
- [X] (1.7.6) Splashscreen when the application is being launched
- [X] (1.8.0) In-app update checker
- [X] (2.3.0) Executable installer
- [X] (2.5.0) In-app automatic updater
- [ ] Create test classes

# Mirrors
The source code is available on:
- [Github](https://github.com/Mitsuriou/MFBO-Preset-Creator)
- [GitLab](https://gitlab.com/Mitsuriou/MFBO-Preset-Creator)

You can find the Nexus Mods page [here](https://www.nexusmods.com/skyrimspecialedition/mods/44706).
