#include<math.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"
#include <sys/time.h>
#include <stdbool.h>
#include<syslog.h>

#define NUM_BARRIERS 1000


int size = -1;
int rank = -1;
char hostName[100];
int len;

int numProcesses;


int num_levels = 0;
MPI_Status status;

bool logging = false;

double timeduration(struct timeval t1, struct timeval t2)
{
	return (t2.tv_sec-t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec);
}

void gtmpi_init(int num_threads){
	numProcesses = num_threads;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(hostName, &len);
	num_levels = ceil(log(numProcesses)/log(2));
}

void gtmpi_barrier(){
	if(logging){
		openlog("OPENMPI_LOG1", LOG_NDELAY| LOG_PID |LOG_CONS, LOG_USER);
		syslog(LOG_DEBUG, "This MPI1 Host %s\t Process %d\t Size of world %d!\n", hostName, rank, size);
		printf("This MPI1 Host %s\t Process %d\t Size of world %d!\n", hostName, rank, size);
		printf("Number of levels %d\n", num_levels);
		closelog();
	}
	int i, to_send, to_recv,msg =1, tag=1;
	MPI_Request sendRequest, recvRequest;
	for(i=0; i<num_levels; i++){
		to_send = (rank + pow(2, i));
		to_send = to_send % numProcesses;

		to_recv = (rank - pow(2, i));
		to_recv = to_recv %  numProcesses;
		if(to_recv < 0) to_recv = to_recv + numProcesses;
		

		MPI_Isend(&msg, 1, MPI_INT, to_send, tag, MPI_COMM_WORLD,&sendRequest);
		MPI_Irecv(&msg, 1, MPI_INT, to_recv, tag, MPI_COMM_WORLD, &recvRequest);
		MPI_Wait (&recvRequest, &status);
		if(logging){
			openlog("OPENMPI_LOG1", LOG_NDELAY| LOG_PID |LOG_CONS, LOG_LOCAL1);
			syslog(LOG_DEBUG, "MPI1 process %d received value %d from rank %d, with tag %d and error code %d.\n", rank, msg, status.MPI_SOURCE, status.MPI_TAG,status.MPI_ERROR);
			printf("MPI1 process %d received value %d from rank %d, with tag %d and error code %d.\n", rank, msg, status.MPI_SOURCE, status.MPI_TAG,status.MPI_ERROR);
			printf("to recv: %d\t", to_recv);
			printf("to send: %d\n", to_send);	
			closelog();
		}
	}
}

void gtmpi_finalize(){
}
