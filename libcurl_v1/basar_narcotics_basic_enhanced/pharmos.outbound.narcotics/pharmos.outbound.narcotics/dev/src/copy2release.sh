#!/usr/bin/sh
RELEASE_DIR=$WSS/release

# bin dir
cp -f transfer2bfarm/release/transfer2bfarm.bin   $RELEASE_DIR/bin

# proc dir
cp -f transfer2bfarm/proc/transfer2bfarm.sh       $RELEASE_DIR/proc
cp -f transfer2bfarm/proc/transfer2bfarm.option   $RELEASE_DIR/proc

#ini dir
cp -f transfer2bfarm/ini/transfer2bfarm.cfg       $RELEASE_DIR/ini

#vars dir
cp -f narctransfer/proc/narcotics_global.vars     $RELEASE_DIR/vars
