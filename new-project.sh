#!/bin/bash

if [[ "$1" == "--help" ]]; then
	echo "syntax: ./new-project.sh dir name title"
	exit
fi

if [[ "$#" != "3" ]]; then
	echo "Incorrect number of arguments."
	echo "syntax: ./new-project.sh dir name title"
	exit
fi

projDir=$1
projName=$2
projTitle=$3
currDir=$(pwd)

# Copy over the template
rm -rf "$projDir/$projName"
mkdir -p "$projDir"
cp -r "$currDir/project-template" "$projDir/$projName"

projDir=$projDir/$projName

mv "$projDir/src/game" "$projDir/src/$projName"
mv "$projDir/src/$projName/Game.h" "$projDir/src/$projName/$projTitle.h"
mv "$projDir/src/$projName/Game.cpp" "$projDir/src/$projName/$projTitle.cpp"

sed -i "s*projectName*$projName*g" "$projDir/CMakeLists.txt"
sed -i "s*projectTitle*$projTitle*g" "$projDir/CMakeLists.txt"

sed -i "s*projectTitle*$projTitle*g" "$projDir/src/$projName/$projTitle.h"
sed -i "s*projectTitle*$projTitle*g" "$projDir/src/$projName/$projTitle.cpp"
sed -i "s*projectName*$projName*g" "$projDir/src/$projName/$projTitle.h"
sed -i "s*projectName*$projName*g" "$projDir/src/$projName/$projTitle.cpp"
sed -i "s*projectName*$projName*g" "$projDir/src/launcher/main.cpp"
sed -i "s*projectTitle*$projTitle*g" "$projDir/src/launcher/main.cpp"

sed -i "s*projectName*$projName*g" "$projDir/android/AndroidManifest.xml"
sed -i "s*projectTitle*$projTitle*g" "$projDir/android/AndroidManifest.xml"

cp -r "$currDir/assets" "$projDir"

echo "$currDir" > $projDir/bin/x86/linux/lfantdir.txt

ln -s $currDir/bin/x86/linux $projDir/bin/x86/linux/lib
ln -s $currDir/bin/x64/linux $projDir/bin/x64/linux/lib
ln -s $currDir/cmake $projDir/cmake