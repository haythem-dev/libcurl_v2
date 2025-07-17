#!/usr/bin/ksh


echo "starting task in background..."

echo "pwd is ${PWD}"

. /software/etc/profile_prz

cd ${DEVLIB_PATH}/marathon/bin/ut_scripts

if [[ -f test.txt ]]; then
    rm test.txt
fi

echo "pwd is now ${PWD}"

nohup remote_test.sh > test.txt  2>&1 &

exit $?