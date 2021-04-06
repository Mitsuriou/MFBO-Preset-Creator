Unicode True

; Constants
!define name "MFBO Preset Creator"
!define version "2.9.5.0"

; Base setup
Name "MFBO Preset Creator"
InstallDir "$PROGRAMFILES64\MFBO Preset Creator"
RequestExecutionLevel admin
OutFile "wizard\mfbopc-install-wizard.exe"
Icon "icon.ico"
SetCompressor /SOLID lzma
UninstallIcon "icon.ico"

# start default section
Section
    ; PROGRAM directory
    SetOutPath $INSTDIR
    SetOverwrite on
    File /r "client release\*"

    ; ASSETS directory
    ; Copy only if the directory does not already exist
    SetOutPath "$LOCALAPPDATA\MFBOPresetCreator\assets"
    SetOverwrite off
    File /r "assets\*"

    ; FILTERS.JSON file
    ; Copy only if the file does not already exist
    SetOutPath "$LOCALAPPDATA\MFBOPresetCreator"
    SetOverwrite off
    File "filters.json"

    ; Desktop shortcut
    CreateShortcut "$DESKTOP\${Name} (v.${Version}).lnk" "$INSTDIR\MFBOPresetCreator.exe"

    ; Start menu launch shortcut
    CreateShortcut "$SMPROGRAMS\${Name}.lnk" "$INSTDIR\MFBOPresetCreator.exe"

    ; Start menu uninstall shortcut
    CreateShortcut "$SMPROGRAMS\Uninstall ${Name}.lnk" "$INSTDIR\uninstall.exe"

    # create the uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

# uninstaller section start
Section "uninstall"

    # first, delete the uninstaller
    Delete "$INSTDIR\uninstaller.exe"

    # remove the link from the desktop
    Delete "$DESKTOP\${Name} (v.${Version}).lnk"

    ; remove the link from the start menu
    Delete "$SMPROGRAMS\${Name}.lnk"

    ; remove the link from the start menu
    Delete "$SMPROGRAMS\Uninstall ${Name}.lnk"

    ; remove the main program directory
    RMDir /r $INSTDIR
# uninstaller section end
SectionEnd


; TODO:
; Automatic detection of the .exe file version
; Fancier installer and uninstaller GUI
; Translations
; Custom install location
; Choose the files and directories to uninstall