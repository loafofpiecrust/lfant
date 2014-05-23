#!/bin/bash

if [[ "$#" != "1" ]]; then
	echo "Incorrect number of arguments."
	echo "syntax: ./setup.sh lfant_dir"
	exit
fi

lfantDir=$1
currDir=$(pwd)

echo "$lfantDir" > $currDir/bin/x86/linux/lfantdir.txt

ln -s $lfantDir/bin/x86/linux $currDir/bin/x86/linux/lib
ln -s $lfantDir/bin/x64/linux $currDir/bin/x64/linux/lib