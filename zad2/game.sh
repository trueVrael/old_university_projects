#!/bin/bash

tura=1
function random(){
if (($2 + 8 < $3 )); then
	tmp2=$(( ( RANDOM % ($3 - $2 - 7) )  + $2 + 8 ))
	tmp1=$(( ( RANDOM % ($3 - 3) )  + 1))
elif (($2 - 8 > 0 )); then
	tmp2=$(( ( RANDOM % ($2 - 8) )  + 1 ))
	tmp1=$(( ( RANDOM % ($3 - 3) )  + 1))
elif (($1 + 10 < $3 )); then
	tmp1=$(( ( RANDOM % (($3 - $1) - 10) )  + 8 + $1))
	tmp2=$(( ( RANDOM % ($3 - 3) )  + 1))
elif (($1 - 8 > 0 )); then
	tmp1=$(( ( RANDOM % ($1 - 8) ) +1 ))
	tmp2=$(( ( RANDOM % ($3 - 3) )  + 1))
else
	exit 1
fi
}
function random2(){
if (($n < 9)); then
	exit 1
else
	rand=$(( ( RANDOM % 2 ) ))
	if ((rand == 0)); then
		tmp3=$(( ( RANDOM % ($1 - 8) ) +1 ))
		tmp4=$(( ( RANDOM % ($1 - 8) ) +1 ))
		tmp6=$(( ( RANDOM % ($1 - tmp4 - 7) )  + tmp4 + 8 ))
		tmp5=$(( ( RANDOM % ($3 - 3) )  + 1))
	elif ((rand == 1)); then
		tmp3=$(( ( RANDOM % ($1 - 8) ) +1 ))
		tmp4=$(( ( RANDOM % ($1 - 8) ) +9 ))
		tmp6=$(( ( RANDOM % (tmp4 - 8) )  + 1 ))
		tmp5=$(( ( RANDOM % ($1 - 3) )  + 1))
	fi
	
fi
}

n=10;
k=100;
s=1;
a1=0;
a2=0;
while :; do
  case $1 in
    -n)
      n=$2
	  shift
      ;;
    -k)
      k=$2
	  shift
      ;;
    -s)
      s=$2
	  shift
      ;;
    -p1)
      IFS=','
      array=($2)
      ;;
    -p2)
      IFS=','
      array2=($2)
      ;;
    -ai1)
      a=$2
	  a1=1
	  shift
      ;;
    -ai2)
      b=$2
	  a2=1
	  shift
      ;;
	  *) 
		break;
  esac
  shift
done
#sprawdz poprawnosc parametrow
if (($n >= 2147483648)); then
	exit 1
fi
if (($k >= 2147483648)) || (($k < 1)); then
	exit 1
fi
if (($s < 0 )); then
	exit 1
fi
if ((${#array[@]} != 2 )) && ((${#array[@]} > 0 )); then
	exit 1
fi
if ((${#array2[@]} !=2 )) && ((${#array2[@]} > 0 )); then
	exit 1
fi
#czy to na pewno plik wykonywalny
if (($a1 > 0)); then
	if [ -x $a ]; then
	:
	else 
	exit 1
	fi
fi
if (($a2 > 0)); then	
	if [ -x $b ]; then
	:
	else 
	exit 1
	fi
fi	
#czy da sie ustawic prezciwnika
if ((${#array[@]} == 2 )) && ((${#array2[@]} == 0 )); then
	random ${array[0]} ${array[1]} $n
	array2=($tmp1 $tmp2)
elif ((${#array[@]} == 0 )) && ((${#array2[@]} == 2 )); then
	random ${array2[0]} ${array2[1]} $n
	array=($tmp1 $tmp2)
elif ((${#array[@]} == 0 )) && ((${#array2[@]} == 0 )); then
	random2 $n
	array=($tmp3 $tmp4)
	array2=($tmp5 $tmp6)
fi

PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec 50<>${PIPE}"
rm $PIPE
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec 51<>${PIPE}"
rm $PIPE
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec 52<>${PIPE}"
rm $PIPE
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec 53<>${PIPE}"
rm $PIPE
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec 54<>${PIPE}"
rm $PIPE
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec 55<>${PIPE}"
rm $PIPE

if (($a1 > 0)); then
$a <&52 >&53 &
pid1=$!
fi
if (($a2 > 0)); then
$b <&54 >&55 &
pid2=$!
fi
if (($a1 > 0)) && (($a2 > 0)); then
	./sredniowiecze_gui_with_libs.sh <&50 >&51 &
elif (($a1 > 0)); then
	./sredniowiecze_gui_with_libs.sh -human2 <&50 >&51 &
elif (($a2 > 0)); then
	./sredniowiecze_gui_with_libs.sh -human1 <&50 >&51 &
else
	./sredniowiecze_gui_with_libs.sh -human1 -human2 <&50 >&51 &
fi
pid3=$!
echo "INIT $n $k 1 ${array[0]} ${array[1]} ${array2[0]} ${array2[1]}" >&50
echo "INIT $n $k 2 ${array[0]} ${array[1]} ${array2[0]} ${array2[1]}" >&50
if (($a2 > 0)); then
	echo "INIT $n $k 2 ${array[0]} ${array[1]} ${array2[0]} ${array2[1]}" >&54
fi
if (($a1 > 0)); then
	echo "INIT $n $k 1 ${array[0]} ${array[1]} ${array2[0]} ${array2[1]}" >&52
fi
while kill =0 $pid3 2> /dev/null;
do
if (($a1 > 0 )); then
	if kill -0 "$pid1"; then
	:
	else
		wait $pid1
		my_status=$?
		if(($my_status > 2)) || (($my_status < 0)); then
			kill $pid1
			if (($a2 > 0)); then
				kill $pid2
			fi
			kill $pid3
			exit 1
		fi
	fi
fi
if (($a2 > 0 )); then
	if kill -0 "$pid2"; then
	:
	else
		wait $pid2
		my_status=$?
		if(($my_status > 2)) || (($my_status < 0)); then
			kill $pid2
			if (($a1 > 0)); then
				kill $pid1
			fi
			kill #pid3
			exit 1
		fi
	fi
fi
if (($tura < 2)); then
	if (($a1 > 0)); then
		read b <&53
		echo "$b" >&50
	else 
		read b <&51
	fi
	if (($a2 > 0)); then
		echo "$b" >&54
	fi
	if (("$b" == "END_TURN")); then
		tura=2
	fi
	if (($a1 > 0)) && (($a2 > 0)); then
		sleep $s
	fi
else
	if (($a2 >0)); then
		read a <&55
		echo "$a" >&50
	else
		read a<&51
	fi
	if (($a1 > 0)); then
		echo "$a" >&52
	fi
	if (("$a" == "END_TURN")); then
		tura=1
	fi
	if (($a1 > 0)) && (($a2 > 0)); then
		sleep $s
	fi
fi
done
wait $pid3
my_status=$?
if(($my_status > 2)) || (($my_status < 0)); then
	if kill -0 $pid2; then
		kill $pid2;
	fi
	if kill -0 $pid1; then
		kill $pid1;
	fi
	if kill -0 $pid3; then
		kill $pid3;
	fi
	exit 1
else
	if kill -0 $pid2; then
		kill $pid2;
	fi
	if kill -0 $pid1; then
		kill $pid1;
	fi
	if kill -0 $pid3; then
		kill $pid3;
	fi
	exit 0
fi
