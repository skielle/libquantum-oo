for i in `seq 0 20`;
do
	for j in `seq 1 10`;
	do
		echo $i,`../bin/test_bb84Generator`
		sleep 1 
	done
done
