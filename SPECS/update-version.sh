#!/bin/bash

# First we make sure we got the revision
if [ $# -lt 4 ]
then
	echo "You need at least 4 parameters: oldversion newversion inputfile outputfile"
	exit -1
fi

OLD_VERSION=$1
NEW_VERSION=$2
INPUT_FILE=$3
OUTPUT_FILE=$4

COMMAND="sed -e \"s/Version: ${OLD_VERSION}/Version: ${NEW_VERSION}/\" $INPUT_FILE  > $OUTPUT_FILE"

echo "$COMMAND"
eval $COMMAND
