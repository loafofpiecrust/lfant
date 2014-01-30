
:help
echo syntax. ./new-project.bat dir name title
exit

:start
if "%1" == "--help" goto help
if not "%~3" goto help

set projDir=%1
set projName=%2
set projTitle=%3
set currDir="%~dp0"

rd /s /q %projDir%\%projName%
mkdir "%projDir%"
copy "%currDir%\project-template" "%projDir%\%projName%"

set projDir="%projDir%\%projName%"

move "%projDir%\src\game" ""
move "%projDir%\src\%projName%\Game.h" "%projDir%\src\%projName%\%projTitle%.h"
move "%projDir%\src\%projName%\Game.cpp" "%projDir%\src\%projName%\%projTitle%.cpp"

sed -i "s*projectName*%projName%*g" "%projDir%\CMakeLists.txt"
sed -i "s*projectTitle*%projTitle%*g" "%projDir%\CMakeLists.txt"

sed -i "s*projectTitle*%projTitle%*g" "%projDir%\src\%projName%\%projTitle%.h"
sed -i "s*projectTitle*%projTitle%*g" "%projDir%\src\%projName%\%projTitle%.cpp"
sed -i "s*projectName*%projName%*g" "%projDir%\src\%projName%\%projTitle%.h"
sed -i "s*projectName*%projName%*g" "%projDir%\src\%projName%\%projTitle%.cpp"
sed -i "s*projectName*%projName%*g" "%projDir%\src\launcher\main.cpp"

sed -i "s*projectName*%projName%*g" "%projDir%\android\AndroidManifest.xml"
sed -i "s*projectTitle*%projTitle%*g" "%projDir%\android\AndroidManifest.xml"