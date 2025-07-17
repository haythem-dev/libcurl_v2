#!/bin/sh

# copy project files to export directory (tar.gz)
# -----------------------------------------------

# exit immediately if a command exits with non-zero exit status
set -e

PROJECT=narcotics

SAVEDIR=`pwd`
BASEDIR=`CDPATH=. cd $SAVEDIR && pwd`
SRCDIR=$BASEDIR
DESTDIR=$BASEDIR/../../export/$PROJECT

echo "-----------------------------------------------------"
echo " copy $PROJECT files to export directory" 
echo "      from $SRCDIR"
echo "      to   $DESTDIR"
echo " compress export files to $DESTDIR/$PROJECT.tar.gz"
echo "-----------------------------------------------------"

rm -rf $DESTDIR
mkdir -p $DESTDIR/bin
mkdir -p $DESTDIR/vars
mkdir -p $DESTDIR/proc
mkdir -p $DESTDIR/ini

echo "copy files ..."
cp -r $SRCDIR/narctransfer/release/narctransfer.bin $DESTDIR/bin
cp -r $SRCDIR/narctransfer/proc/*.vars 				$DESTDIR/vars
cp -r $SRCDIR/narctransfer/proc/*.sh 				$DESTDIR/proc
cp -r $SRCDIR/narctransfer/proc/*.option 			$DESTDIR/proc
cp -r $SRCDIR/narctransfer/*.cfg 					$DESTDIR/ini

cp -r $SRCDIR/transfer2bfarm/release/transfer2bfarm.bin 	$DESTDIR/bin
cp -r $SRCDIR/transfer2bfarm/ini/*.cfg 				 		$DESTDIR/ini
cp -r $SRCDIR/transfer2bfarm/proc/*.sh 				 		$DESTDIR/proc
cp -r $SRCDIR/transfer2bfarm/proc/*.option 			 		$DESTDIR/proc

cp -r $SRCDIR/narcoticsalerting/release/narcoticsalerting.bin 	$DESTDIR/bin
cp -r $SRCDIR/narcoticsalerting/ini/*.cfg 					$DESTDIR/ini
cp -r $SRCDIR/narcoticsalerting/ini/*.ini 					$DESTDIR/ini
cp -r $SRCDIR/narcoticsalerting/proc/*.sh 					$DESTDIR/proc
cp -r $SRCDIR/narcoticsalerting/proc/*.option 				$DESTDIR/proc

echo "compress files ..."
cd $DESTDIR
tar --remove-files -czf ./$PROJECT.tar.gz ./bin ./vars ./proc ./ini
rm -rf ./bin
rm -rf ./vars
rm -rf ./proc
rm -rf ./ini

cp $SRCDIR/build.xml $DESTDIR
cp $SRCDIR/ivy.xml   $DESTDIR

cd $SAVEDIR
