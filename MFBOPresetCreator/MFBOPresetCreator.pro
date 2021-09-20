TEMPLATE = app
TARGET = MFBOPresetCreator
DESTDIR = ../x64/Debug
CONFIG += debug
DEFINES += $(Qt_DEFINES_)
LIBS += -L"."
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += .
TRANSLATIONS += mfbopc_en.ts \
    mfbopc_fr.ts \
    mfbopc_zh_TW.ts
win32:RC_FILE = MFBOPresetCreator.rc
QT += core gui network widgets xml
win32:QT += winextras
HEADERS += ./resource.h \
    ./stdafx.h \
    ./WelcomeScreen.h \
    ./TexturesAssistant.h \
    ./TextInputDialog.h \
    ./Update.h \
    ./PresetCreator.h \
    ./AssistedConversion.h \
    ./About.h \
    ./BatchConversion.h \
    ./BatchConversionPicker.h \
    ./BodySlideFiltersEditor.h \
    ./ReleaseNotesViewer.h \
    ./Settings.h \
    ./RetargetingTool.h \
    ./MFBOPresetCreator.h \
    ./VersionsInformation.h \
    ./Enum.h \
    ./LangManager.h \
    ./SliderFileBuilder.h \
    ./Struct.h \
    ./DataLists.h \
    ./ComponentFactory.h \
    ./Utils.h \
    ./BCDragWidget.h \
    ./BCDropWidget.h \
    ./BCGroupWidget.h
SOURCES += ./main.cpp \
    ./About.cpp \
    ./AssistedConversion.cpp \
    ./BatchConversion.cpp \
    ./BatchConversionPicker.cpp \
    ./BodySlideFiltersEditor.cpp \
    ./ReleaseNotesViewer.cpp \
    ./Settings.cpp \
    ./RetargetingTool.cpp \
    ./PresetCreator.cpp \
    ./TextInputDialog.cpp \
    ./TexturesAssistant.cpp \
    ./Update.cpp \
    ./WelcomeScreen.cpp \
    ./MFBOPresetCreator.cpp \
    ./ComponentFactory.cpp \
    ./DataLists.cpp \
    ./LangManager.cpp \
    ./SliderFileBuilder.cpp \
    ./Utils.cpp \
    ./VersionsInformation.cpp \
    ./BCDragWidget.cpp \
    ./BCDropWidget.cpp \
    ./BCGroupWidget.cpp
FORMS += ./MFBOPresetCreator.ui
TRANSLATIONS += ./mfbopc_en.ts \
    ./mfbopc_fr.ts \
    ./mfbopc_zh_TW.ts
RESOURCES += MFBOPresetCreator.qrc
