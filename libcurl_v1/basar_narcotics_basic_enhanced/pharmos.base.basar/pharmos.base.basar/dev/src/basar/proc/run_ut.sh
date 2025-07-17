#!/bin/bash

basar_dir=$(pwd)
configuration=debug
interactive=1
onlytest=

for arg in "$@" ; do
	case "$arg" in
		debug|release)
			configuration=$arg
		;;
		appl|cmnutil|dbaspect|dbsql|login|property)
			onlytest=$arg
		;;
		*)
			echo "$0 debug|release" >&2
			echo "runs unit tests in configuration (default=$configuration)" >&2
			exit 1
		;;
	esac
done

if [ -f "$basar_dir/ivy.xml" ] ; then
	basar_dir=$basar_dir/basar
fi

if [ ! -f "$basar_dir/../ivy.xml" ] ; then
	echo "error: You have to run this program from dev/src/basar directory" >&2
	exit 1
fi

ext_dir=$basar_dir/../../ext

pushd $basar_dir >/dev/null || exit 1

# tcaccess has to be last in list, zlib the first
export LD_LIBRARY_PATH="$ext_dir/boost/lib:\
$ext_dir/zlib/lib/$configuration:\
$ext_dir/openssl/lib:\
$ext_dir/libssh2/lib/$configuration:\
$ext_dir/openldap/lib:\
$ext_dir/ghostscript/lib:\
$ext_dir/log4cplus/lib/$configuration:\
$ext_dir/qt5/lib:\
$basar_dir/library/lib/$configuration:\
$INFORMIXDIR/lib:\
$INFORMIXDIR/lib/esql:\
$WSS/lib/tcaccess/current/sharedlib/release\
"
unset LIBPATH

echo -e "\033[01;33mRunning Tests with $configuration configuration\033[00m" >&2
echo "" >&2

testsSuccessful=0
testsFailed=0

echo "    LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >&2
echo "">&2

for t in \
	appl \
	cmnutil \
	dbaspect \
	dbsql \
	login \
	property \
	; do

	if [ -n "$onlytest" ] && [ "$onlytest" != "$t" ] ; then
		continue
	fi

	echo -e "\033[01;33mRunning ut$t $*\033[00m">&2

	pushd test/unit/$t >/dev/null || exit 1

	cmd="$configuration/ut$t"

	if [ -n "$interactive" ] ; then
		echo "    $cmd $*" >&2
		echo "">&2
	fi

	$cmd $*
	if [ $? -eq 0 ] ; then
		echo -e "\033[01;32mTest successful\033[00m" >&2
		testsSuccessful=$(($testsSuccesful+1))
	else
		echo -e "\033[01;31mTest failed: Exitcode $?\033[00m" >&2
		testsFailed=$(($testsFailed+1))
		if [ -n "$interactive" ] ; then
			read
		fi
	fi

	popd >/dev/null 2>&1

	echo "">&2
done

echo "" >&2
echo "Finished running tests" >&2
echo -e "\033[01;33mResult: $testsSuccessful Tests ran successful and $testsFailed failed.\033[00m" >&2
echo "" >&2
