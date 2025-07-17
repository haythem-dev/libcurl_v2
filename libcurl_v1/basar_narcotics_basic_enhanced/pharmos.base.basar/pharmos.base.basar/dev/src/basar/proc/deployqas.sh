#!/bin/sh

####################################################################
#
# copy ssuk runtime files to tar file for rolling out to qas system
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

### create qas basar.config.xml from test basar.config.xml
sed s/zdev21_tcp/zqas21_tcp/ < ../data/basar.config.xml >$DEST/sharedlib/debug/basar.config.xml
cp $DEST/sharedlib/debug/basar.config.xml $DEST/sharedlib/release/

# basar 
for ADD in "sharedlib/debug" "sharedlib/release"
do
	cp -h $SOURCE/$ADD/* $DEST/$ADD/
done
chmod -R ug=rwx,o=rx "$DEST/sharedlib"

# ------------------------------------------------------
### create archive
echo "create archive ../${SSUKVERSION}_qas.tar.gz ..."

cd  ..
rm ${SSUKVERSION}_qas.tar 2>/dev/null
tar -C ./tmp -czf ${SSUKVERSION}_qas.tar.gz .
rm -rf ./tmp



echo "runtime files archived"
