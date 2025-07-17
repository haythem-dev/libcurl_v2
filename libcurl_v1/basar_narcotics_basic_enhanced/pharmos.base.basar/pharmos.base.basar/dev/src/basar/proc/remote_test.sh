#!/usr/bin/ksh

typeset -i MAX=10

echo "running 10 seconds, issuing one line per second"

typeset -i I=1

while(( $I <= $MAX  ))
do
	echo "loop $I"
	I=$I+1
	sleep 1
done