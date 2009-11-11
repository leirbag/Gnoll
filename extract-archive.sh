#!/bin/bash

# First we make sure we got the revision
if [ $# -lt 1 ]
then
	echo "You need to pass the revision you wish to extract"
	exit -1
fi


VERSION=$1
NAME=gnoll

COMMAND="git archive --format=tar --prefix=${NAME}-${VERSION}/ ${VERSION} | gzip > ${NAME}-${VERSION}.tar.gz"


echo "$COMMAND"

eval $COMMAND
