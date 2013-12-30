#!/bin/bash

if [ "$1" == "travis" ]
then
	printf "    \n"
	printf "   \n"
	printf "  \n"
	printf " \n"
	
	# Workaround for travis: Other repo
	GITREPO="L2hvbWUvdHJhdmlzL2J1aWxkLzBwcTc2ci9sdW5rd2lsbC8uZ2l0"
else
	GITREPO="Li4vbHVua3dpbGwvLmdpdA=="
fi


echo "l u n k w i l l   a u t o   t e s t s"
echo "====================================="

RET=0
FAILS=0

printf "\nA)\tTesting dashboard page\t\t\t"
wget -O- http://127.0.0.1:3000/BAAAAAAAAAAAAAAAAAAA/AAAA/AA/  > /dev/null 2>&1
if [ $? -eq 0 ]
then
	printf "[ OK ]"
else
	printf "[FAIL]"
	RET=1
	FAILS=$((FAILS+1))
fi


printf "\nB)\tTesting project module\t\t\t"
wget -O- http://127.0.0.1:3000/BAAAAAAAAAAAAAAAAAAA/AAAA/BA/  > /dev/null 2>&1
if [ $? -eq 0 ]
then
	printf "[ OK ]"
else
	printf "[FAIL]"
	RET=1
	FAILS=$((FAILS+1))
fi


printf "\nC)\tTesting git module\t\t\t"
wget -O- "http://127.0.0.1:3000/#/AAAA/CA/$GITREPO"  > /dev/null 2>&1
if [ $? -eq 0 ]
then
	printf "[ OK ]"
else
	printf "[FAIL]"
	RET=1
	FAILS=$((FAILS+1))
fi


printf "\nD)\tTesting dummy module\t\t\t"
wget -O- http://127.0.0.1:3000/BAAAAAAAAAAAAAAAAAAA/AAAA/DA/  > /dev/null 2>&1
if [ $? -eq 0 ]
then
	printf "[ OK ]"
else
	printf "[FAIL]"
	RET=1
	FAILS=$((FAILS+1))
fi


printf "\nE)\tTesting lua dummy module\t\t"
wget -O- http://127.0.0.1:3000/BAAAAAAAAAAAAAAAAAAA/AAAA/EA/  > /dev/null 2>&1
if [ $? -eq 0 ]
then
	printf "[ OK ]"
else
	printf "[FAIL]"
	RET=1
	FAILS=$((FAILS+1))
fi


printf "\n\n"
if [ $RET -eq 0 ]
then
	printf "All tests passed! :-)\n"
else
	printf $FAILS" test(s) failed...\n"
fi

exit $RET
