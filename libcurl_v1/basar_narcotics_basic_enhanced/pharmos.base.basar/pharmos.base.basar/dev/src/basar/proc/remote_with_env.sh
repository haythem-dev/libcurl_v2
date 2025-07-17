#!/usr/bin/ksh

. /software/etc/profile_prz

$DEVLIB_PATH/marathon/bin/ut_scripts/remote_return_42.sh

return $?