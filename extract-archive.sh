#!/bin/bash

# First we make sure we got the revision
if [ $# -lt 1 ]
then
	echo "You need to pass the revision you wish to extract at least"
	exit -1
fi


NAME=gnoll
VERSION=$1
FILENAME=VERSION

if [ -n $2 ]
then
	FILENAME=$2
fi

COMMAND="git archive --format=tar --prefix=${NAME}-${FILENAME}/ ${VERSION} | gzip > ${NAME}-${FILENAME}.tar.gz"


echo "$COMMAND"

eval $COMMAND
