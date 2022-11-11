#!/bin/bash
for i in $(seq 2 1 12)
do
	qsub  ~/mpi/cb-mpi2-p$i-t1.pbs;
	sleep 60; 
done

