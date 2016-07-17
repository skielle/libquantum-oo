#!/bin/bash
for i in `seq 0 20`;
do
	for j in `seq 1 10`;
	do
		../bin/test_bb84Determiner $((i*5))
	done
done
