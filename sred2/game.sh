#!/bin/bash

function random(){
if (($2 + 8 < $3 )); then
echo "cos"
	tmp2=$(( ( RANDOM % ($3 - $2 - 7) )  + $2 + 8 ))
	tmp1=$(( ( RANDOM % ($3 - 3) )  + 1))
elif (($2 - 8 > 0 )); then
echo "cos2"
	tmp2=$(( ( RANDOM % ($2 - 8) )  + 1 ))
	tmp1=$(( ( RANDOM % ($3 - 3) )  + 1))
elif (($1 + 10 < $3 )); then
echo "cos3"
	tmp1=$(( ( RANDOM % (($3 - $1) - 10) )  + 8 + $1))
	tmp2=$(( ( RANDOM % ($3 - 3) )  + 1))
elif (($1 - 8 > 0 )); then
echo "cos4"
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
      echo "-n: $2" >&2
      n=$2
	  shift
      ;;
    -k)
      echo "-k : $2" >&2
      k=$2
	  shift
      ;;
    -s)
      echo "-s : $2" >&2
      s=$2
	  shift
      ;;
    -p1)
      echo "-p1 was triggered! $2" >&2
      IFS=','
      array=($2)
      ;;
    -p2)
      echo "-p2 was triggered! $2" >&2
      IFS=','
      array2=($2)
      ;;
    -a1)
      echo "-a : $2" >&2
      a=$2
	  a1=1
	  shift
      ;;
    -a2)
      echo "-b : $2" >&2
      b=$2
	  a2=1
	  shift
      ;;
	  *)
		break
  esac
  shift
done
#sprawdz poprawnosc parametrow
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
if ((a1 == 1)); then
	if [ -x "$a" ]; then
	:
	else 
	exit 1
	fi
fi
if ((a2 == 1)); then	
	if [ -x "$b" ]; then
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
