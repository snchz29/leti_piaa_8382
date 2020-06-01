#!/bin/sh
for t in Tests/shift/*;
do 
	echo "Input:"
	cat $t;
	echo "\n\nOutput:"
	Bin/stepik2.out < $t ;
	echo "\n\n" 
done
