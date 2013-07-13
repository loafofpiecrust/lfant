#!/bin/bash

if [[ "$1" == "--help" ]]; then
	echo "syntax: ./new-project.sh dir name title"
	exit
fi

if [[ "$#" != "3" ]]; then
	echo "Incorrect number of arguments."
	exit
fi

projDir=$1
projName=$2
projTitle=$3
currDir=$(pwd)

# Copy over the template
rm -rf "$projDir/$projName"
mkdir -p "$projDir"
cp -r "./project-template" "$projDir/$projName"

projDir=$1/$2

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

ln -s $currDir $projDir/bin32/linux