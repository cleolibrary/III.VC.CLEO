@echo off

SET zip="C:\Program Files\7-Zip\7z.exe"

echo Preparing GTA III CLEO
FOR /F "USEBACKQ" %%F IN (`powershell -NoLogo -NoProfile -Command ^(Get-Item "bin\III.CLEO.asi"^).VersionInfo.FileVersion`) DO (SET fileVersion=%%F)
echo Detected version: %fileVersion%
SET outputFile=".\CLEO.III_v%fileVersion%.zip"
if exist %outputFile% del %outputFile% /q

%zip% a -tzip %outputFile% ".\Changelog.md" -bb2 | findstr "+" 
%zip% rn %outputFile% "Changelog.md" "CLEO_Readme\Changelog.txt" -bso0

%zip% a -tzip %outputFile% ".\Readme_III.md" -bb2 | findstr "+" 
%zip% rn %outputFile% "Readme_III.md" "CLEO_Readme\Readme.txt" -bso0

%zip% a -tzip %outputFile% ".\bin\*" -bb2 -xr!*.tmp -xr!*.pdb -xr!*.ilk -xr!*.ipdb -xr!*.iobj -xr!*.db -xr!*.exp -xr!*.lib -xr!*VC.*.* | findstr "+"

echo.
echo Preparing GTA VC CLEO
FOR /F "USEBACKQ" %%F IN (`powershell -NoLogo -NoProfile -Command ^(Get-Item "bin\VC.CLEO.asi"^).VersionInfo.FileVersion`) DO (SET fileVersion=%%F)
echo Detected version: %fileVersion%
SET outputFile=".\CLEO.VC_v%fileVersion%.zip"
if exist %outputFile% del %outputFile% /q

%zip% a -tzip %outputFile% ".\Changelog.md" -bb2 | findstr "+" 
%zip% rn %outputFile% "Changelog.md" "CLEO_Readme\Changelog.txt" -bso0

%zip% a -tzip %outputFile% ".\Readme_VC.md" -bb2 | findstr "+" 
%zip% rn %outputFile% "Readme_VC.md" "CLEO_Readme\Readme.txt" -bso0

%zip% a -tzip %outputFile% ".\bin\*" -bb2 -xr!*.tmp -xr!*.pdb -xr!*.ilk -xr!*.ipdb -xr!*.iobj -xr!*.db -xr!*.exp -xr!*.lib -xr!*III.*.* | findstr "+"

pause
