# private dbx command file

# set library path
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../basar/library/lib/debug:${DEVLIB_PATH}/marathon/lib/log4cplus/1.0.4_2/lib/debug:${DEVLIB_PATH}/marathon/lib/boost/1.41.0_2/lib:${DEVLIB_PATH}/marathon/lib/tcaccess/8.0.6.3/lib;

# load executable
debug testdbsql

# set leak checking on
check -leaks

# set mode verbose
dbxenv rtc_biu_at_exit verbose
