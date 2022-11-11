#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "gtmpi.h"
#include <sys/time.h>

int main(int argc, char** argv)
{
  int num_processes, num_rounds = 100;
  struct timeval start, end;
  MPI_Init(&argc, &argv);
  
  if (argc < 2){
    fprintf(stderr, "Usage: ./harness [NUM_PROCS]\n");
    exit(EXIT_FAILURE);
  }

  num_processes = strtol(argv[1], NULL, 10);
  
  gettimeofday(&start, NULL);
  
  gtmpi_init(num_processes);
  
  int k;
  for(k = 0; k < num_rounds; k++){
    gtmpi_barrier();
    
  }
  
  gettimeofday(&end, NULL);
  
  gtmpi_finalize();
  
  printf("Total Time for %d rounds:     %ld\n", num_rounds, ((end.tv_sec * 1000000 + end.tv_usec)-(start.tv_sec * 1000000 + start.tv_usec)));  

  MPI_Finalize();

  return 0;
}
