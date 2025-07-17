#!/bin/sh

####################################################################
#
# copy ssuk runtime files to tar file for rolling out to live system
#
####################################################################

#set -x

# ------------------------------------------------------
### variables

. ../data/version.inc

SOURCE=$( cd $(dirname $0)/.. && pwd)
DEST=../tmp/marathon/$SSUKVERSION

# ------------------------------------------------------
### copy/link files
echo "create directories in $DEST"

mkdir -p $DEST/sharedlib/debug
mkdir -p $DEST/sharedlib/release


# basar 
for ADD in "sharedlib/debug" "sharedlib/release"
do
	cp -h $SOURCE/$ADD/* $DEST/$ADD/
done
chmod -R ug=rwx,o=rx "$DEST/sharedlib"

# ------------------------------------------------------
### create archive
echo "create archive ../${SSUKVERSION}.tar.gz ..."

cd  ..
rm ${SSUKVERSION}.tar 2>/dev/null
tar -C ./tmp -czf ${SSUKVERSION}.tar.gz .
rm -rf ./tmp


echo "runtime files archived"
