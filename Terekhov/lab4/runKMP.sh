#!/bin/sh
for t in Tests/kmp/*;
do 
	echo "Input:"
	cat $t;
	echo "\n\nOutput:"
	Bin/kmp.out < $t ;
	echo "\n\n" 
done
