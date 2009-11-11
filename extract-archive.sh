#!/bin/sh
git archive --format=tar --prefix=gnoll-0.1.7/ HEAD | gzip >gnoll-0.1.7.tar.gz
