#!/bin/bash

# Usage: ./test.sh [-v] prog directory
VERBOSE=false && [[ "$1" == "-v" ]] && VERBOSE=true

if [ "$VERBOSE" = true ]; then
	PROG=$2
	DIR=$3
else
	PROG=$1
	DIR=$2
fi

TEMP_OUT=tmp.out
TEMP_ERR=tmp.err

if [ ! -f $PROG ] || [ ! -x $PROG ] || [ ! -d $DIR ]; then
	exit
fi

for i in $DIR/*.in; do

	if [ ! -f $i ]; then
		continue
	fi

	PASS=false
	FAIL_REASON=""

	if [[ "$VERBOSE" = true ]]; then
		`"$PROG" -v < "$i" >"$TEMP_OUT" 2>"$TEMP_ERR"`
	else
			`"$PROG" < "$i" > "$TEMP_OUT"`
	fi

	# Check stdout
	diff "$TEMP_OUT" ${i/%.in}.out
	if [ $? -eq 0 ]; then
		PASS=true
	elif [ $? -eq 1 ]; then
		FAIL_REASON=".out mismatch"
	fi

	# Check stderr
	if [[ "$VERBOSE" = true ]]; then
		diff "$TEMP_ERR" ${i/%.in}.err
		if [ $? -eq 0 ]; then
			PASS=true
		elif [ $? -eq 1 ]; then
			PASS=false
			FAIL_REASON=$FAIL_REASON" & .err mismatch"
		fi
	fi

	if [[ "$PASS" = true ]]; then
		echo -e "[\033[0;32mPASS\033[0m]" $i
	else
		echo -e "[\033[0;31mFAIL\033[0m]" $i - $FAIL_REASON
	fi
done

if [[ -e $TEMP_OUT ]]; then
	rm $TEMP_OUT
fi

if [[ -e $TEMP_ERR ]]; then
	rm $TEMP_ERR
fi
