#!/bin/bash

TMP_OUT=tmp.out
TMP_ERR=tmp.err
VFLAG=false

if [[ "$1" == "-v" ]]; then
	VFLAG=true
fi

if [ "$VFLAG" = false ]; then
	PROG=$1
	DIR=$2
else
	PROG=$2
	DIR=$3
fi

for i in $DIR/*.in; do

	if [ ! -f $i ]; then
		continue
	fi
	
	if [ ! -f ${i/%.in}.out ]; then
		continue
	fi

	PASS=false

	if [[ "$VFLAG" = false ]]; then
		"./$PROG" < "$i" > "$TMP_OUT"
	else
		"./$PROG" -v < "$i" >"$TMP_OUT" 2>"$TMP_ERR"
	fi

	diff "$TMP_OUT" ${i/%.in}.out

	if [ $? != 0 ]; then
		PASS=false
	else
		PASS=true
	fi

	if [[ "$VFLAG" = true ]]; then
		diff "$TMP_ERR" ${i/%.in}.err
		if [ $? -eq 0 ]; then
			PASS=true
		elif [ $? -eq 1 ]; then
			PASS=false
		fi
	fi

	if [[ "$PASS" = true ]]; then
		echo -e "PASS" $i
	else
		echo -e "WRONG" $i
	fi
done

#clean
if [[ -e $TMP_OUT ]]; then
	rm $TMP_OUT
fi

if [[ -e $TMP_ERR ]]; then
	rm $TMP_ERR
fi
