#!/bin/bash

for j in `seq 1 100`;
do
	echo $1, `../bin/test_kakInitiator`
	sleep 1 
done
