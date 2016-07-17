#!/bin/bash
i=$1
for j in `seq 1 100`;
do
	../bin/test_kakResponder $((i*5))
done
