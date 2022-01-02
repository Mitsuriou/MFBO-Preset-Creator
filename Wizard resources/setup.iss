[Setup]  
#define AppVersion GetVersionNumbersString(".\client release\MFBOPresetCreator.exe")
#define AppName "MFBO Preset Creator"

AppId={{F0572980-4893-4828-B9EC-B9AD99DBCE35}
AppName=MFBO Preset Creator
AppVersion={#AppVersion}
AppPublisher=Mitsuriou
AppPublisherURL=https://github.com/Mitsuriou/MFBO-Preset-Creator
AppSupportURL=https://github.com/Mitsuriou/MFBO-Preset-Creator
AppUpdatesURL=https://github.com/Mitsuriou/MFBO-Preset-Creator
DefaultDirName={commonpf64}\{#AppName}
DisableProgramGroupPage=yes
PrivilegesRequiredOverridesAllowed=dialog
OutputDir=wizard
OutputBaseFilename=mfbopc-install-wizard
SetupIconFile=.\icon.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern
UninstallDisplayIcon={app}\MFBOPresetCreator.exe
UninstallDisplayName=MFBO Preset Creator (v.{#AppVersion})
ChangesAssociations = yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[CustomMessages]
english.DeleteAppDataDir=Do you want to remove configuration files too?%nClicking "YES" will delete the whole "MFBOPresetCreator" folder located in "%1".%nClicking "NO" will let the files untouched.
french.DeleteAppDataDir=Voulez-vous supprimer les fichiers de configuration également ?%nCliquer sur "OUI" supprimera complètement le dossier "MFBOPresetCreator" situé dans "%1".%nCliquer sur "NON" laissera les fichiers tels quels.

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: ".\client release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\filters.json"; DestDir: "{localappdata}\MFBOPresetCreator"; Flags: ignoreversion uninsneveruninstall onlyifdoesntexist
Source: ".\assets\*"; DestDir: "{localappdata}\MFBOPresetCreator\assets"; Flags: ignoreversion recursesubdirs createallsubdirs uninsneveruninstall onlyifdoesntexist
Source: ".\fonts\Roboto-Regular.ttf"; DestDir: "{commonfonts}"; FontInstall: "Roboto"; Flags: onlyifdoesntexist uninsneveruninstall

[Icons]
Name: "{autoprograms}\{#AppName}"; Filename: "{app}\MFBOPresetCreator.exe"
Name: "{autodesktop}\{#AppName} (v.{#AppVersion})"; Filename: "{app}\MFBOPresetCreator.exe"; Tasks: desktopicon

[InstallDelete]
Type: filesandordirs; Name: "{localappdata}\MFBOPresetCreator\assets\skeletons\Female - XPMSSE (v4.72)"
Type: filesandordirs; Name: "{localappdata}\MFBOPresetCreator\assets\skeletons\Female beast - XPMSSE (v4.72)"
Type: filesandordirs; Name: "{localappdata}\MFBOPresetCreator\assets\skeletons\Female - XPMSSE (v4.80)"
Type: filesandordirs; Name: "{localappdata}\MFBOPresetCreator\assets\skeletons\Female beast - XPMSSE (v4.80)"
Type: filesandordirs; Name: "{localappdata}\MFBOPresetCreator\assets\skeletons\Female - Vera's Female Skeleton (v1.52)"
Type: filesandordirs; Name: "{localappdata}\MFBOPresetCreator\assets\skeletons\Female beast - Vera's Female Skeleton (v1.52)"

[Run]
Filename: "{app}\MFBOPresetCreator.exe"; Description: "{cm:LaunchProgram,{#AppName}}"; Flags: nowait postinstall skipifsilent

[Registry]
Root: HKCR; Subkey: ".pcp"; ValueData: "{#AppName}"; Flags: uninsdeletevalue; ValueType: string; ValueName: ""
Root: HKCR; Subkey: "{#AppName}"; ValueData: "Program {#AppName}";  Flags: uninsdeletekey; ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#AppName}\DefaultIcon"; ValueData: "{app}\MFBOPresetCreator.exe,0"; Flags: uninsdeletevalue; ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#AppName}\shell\open\command";  ValueData: """{app}\MFBOPresetCreator.exe"" ""%1"""; Flags: uninsdeletevalue; ValueType: string;  ValueName: ""

[Code]
var Concat: String;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usPostUninstall then
  begin
    Concat := '{cm:DeleteAppDataDir,' + ExpandConstant('{localappdata}') + '}'
    if MsgBox(ExpandConstant(Concat), mbConfirmation, MB_YESNO or MB_DEFBUTTON2) = IDYES then
    begin
        DelTree(ExpandConstant('{localappdata}\MFBOPresetCreator'), True, True, True);
    end;
  end;
end;
