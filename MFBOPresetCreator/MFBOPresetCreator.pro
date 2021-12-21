QT += core gui network widgets xml

TEMPLATE = app

CONFIG += c++17

HEADERS += ./resource.h \
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
    ./BCGroupWidget.h \
    TargetMeshesPicker.h

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
    ./BCGroupWidget.cpp \
    TargetMeshesPicker.cpp

FORMS += ./MFBOPresetCreator.ui

win32:RC_FILE = MFBOPresetCreator.rc

RESOURCES += MFBOPresetCreator.qrc

TRANSLATIONS += ./mfbopc_en.ts \
    ./mfbopc_fr.ts \
    ./mfbopc_zh_TW.ts
