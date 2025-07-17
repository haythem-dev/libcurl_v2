# private dbx command file

# set library path
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../../library/lib/debug:../../../../log4cplus/lib/debug

# load executable
debug testdbaspect

# set leak checking on
check -leaks

# set mode verbose
dbxenv rtc_biu_at_exit verbose
