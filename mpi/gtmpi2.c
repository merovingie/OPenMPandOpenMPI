#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"
#include <stdbool.h>
#include <sys/time.h> 
#include<math.h> 
#include<syslog.h>

int size = -1;
int rank = -1;
bool havechild[4];
bool childnotready[4];
bool awakechild[2];

bool logging = false;

void gtmpi_init(int num_processes){
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}

void gtmpi_barrier(){
	//init
	for (int i = 0; i < 4; i++) {
		if ( 4 * rank + i + 1 < size) 
			havechild[i] = true;
		childnotready[i] = havechild[i];
	}
	for (int i = 0; i < 2; i++) 
		awakechild[i] = (2 * rank + 1 + i < size);
	//prepare for next round
	while (childnotready[0] || 
			childnotready[1] || 
			childnotready[2] || 
			childnotready[3]) {
		int msg;
		MPI_Status status;
		MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		if(logging){
			openlog("OPENMPI_LOG2", LOG_NDELAY| LOG_PID |LOG_CONS, LOG_LOCAL1);
			syslog(LOG_DEBUG, "MPI2 process %d received value %d from rank %d, with tag %d and error code %d.\n", rank, msg, status.MPI_SOURCE, status.MPI_TAG,status.MPI_ERROR);
			closelog();
		}
		childnotready[status.MPI_SOURCE - 4 * rank - 1] = false;
	}
	for (int i = 0; i < 4; i++)
		childnotready[i] = havechild[i];

	if (rank != 0) {
		//tell parent i am ready
		int msg = 1;
		MPI_Send(&msg, 1, MPI_INT, (rank - 1) / 4, 0, MPI_COMM_WORLD);
		// wait for parent
		MPI_Status status;	
		MPI_Recv(&msg, 1, MPI_INT, (rank - 1) / 2, 1, MPI_COMM_WORLD, &status);

		if(logging){
			openlog("OPENMPI_LOG2", LOG_NDELAY| LOG_PID |LOG_CONS, LOG_LOCAL1);
			syslog(LOG_DEBUG, "MPI2 process %d received value %d from rank %d, with tag %d and error code %d.\n", rank, msg, status.MPI_SOURCE, status.MPI_TAG,status.MPI_ERROR);
			closelog();
		}
	}

	//awaken children
	int msg = 1;
	if (awakechild[0])
		MPI_Send(&msg, 1, MPI_INT, 2 * rank + 1, 1, MPI_COMM_WORLD);
	if (awakechild[1])
		MPI_Send(&msg, 1, MPI_INT, 2 * rank + 2, 1, MPI_COMM_WORLD);

}

void gtmpi_finalize(){

}
