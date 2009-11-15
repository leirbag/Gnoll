#!/bin/bash

# First we make sure we got the revision
if [ $# -lt 3 ]
then
	echo "You need at least 4 parameters: oldversion newversion inputfile"
	exit -1
fi

OLD_VERSION=$1
NEW_VERSION=$2
INPUT_FILE=$3
OUTPUT_FILE="$3.tmp"

COMMAND1="head -n 1 $INPUT_FILE | sed -e \"s/${OLD_VERSION}/${NEW_VERSION}/\" > $OUTPUT_FILE"
COMMAND2="tail -n +2 $INPUT_FILE >> $OUTPUT_FILE" 

echo "$COMMAND1"
eval $COMMAND1
echo "$COMMAND2"
eval $COMMAND2

mv $OUTPUT_FILE $INPUT_FILE
