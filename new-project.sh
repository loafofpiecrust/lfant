#!/bin/bash

if [[ "$#" != "3" ]]; then
	echo "Incorrect number of arguments."
	exit
fi

projDir=$1
projName=$2
projTitle=$3

# Copy over the template
mkdir -p "$projDir"
rm -rf "$projDir/$projName"
cp -r "./project-template" "$projDir/$projName"

projDir=$1/$2

mv "$projDir/src/game" "$projDir/src/$projName"
mv "$projDir/src/$projName/Game.hpp" "$projDir/src/$projName/$projTitle.hpp"
mv "$projDir/src/$projName/Game.cpp" "$projDir/src/$projName/$projTitle.cpp"
mv "$projDir/src/$projName/game.cbp" "$projDir/src/$projName/$projName.cbp"

sed -i "s*projectDir*$projDir*g" "$projDir/src/codews.workspace"
sed -i "s*projectName*$projName*g" "$projDir/src/codews.workspace"
sed -i "s*projectTitle*$projTitle*g" "$projDir/src/codews.workspace"

sed -i "s*projectDir*$projDir*g" "$projDir/src/$projName/$projName.cbp"
sed -i "s*projectName*$projName*g" "$projDir/src/$projName/$projName.cbp"

sed -i "s*projectTitle*$projTitle*g" "$projDir/src/$projName/$projTitle.hpp"
sed -i "s*projectTitle*$projTitle*g" "$projDir/src/$projName/$projTitle.cpp"
