#!/bin/sh
for t in Tests/*;
do 
	echo "Input:"
	cat $t;
	echo "\n\nOutput:"
	python3 Source/main.py < $t ;
	echo "\n\n" 
done
