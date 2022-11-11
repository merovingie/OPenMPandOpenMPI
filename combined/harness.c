#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <sys/time.h>
#include "combined.h"



int main(int argc, char** argv)
{
  int num_processes,  num_threads, num_rounds = 10;
  
  MPI_Init(&argc, &argv);
  
  if (argc < 3){
    fprintf(stderr, "Usage: ./combined [NUM_PROCS] [NUM_THREADS] [NUM_ROUNDS]\n");
    exit(EXIT_FAILURE);
  }
  
  num_processes = strtol(argv[1], NULL, 10);
  num_threads = strtol(argv[2], NULL, 10);
  
  if (argc < 4){
    fprintf(stdout, "Warning: [NUM_ROUNDS] wasn't provided, executing with default: 10 rounds\n");
  }
  else
  {
      num_rounds = strtol(argv[3], NULL, 10);
  }

  omp_set_dynamic(0);
  if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");

  omp_set_num_threads(num_threads);
  
  combined_init(num_processes, num_threads);
  
  struct timeval start, end;
  gettimeofday(&start, NULL);
  
  #pragma omp parallel shared(num_threads)
  {
    int k;
    for(k = 0; k < num_rounds; k++)
    {
      combined_barrier();
    }
  }
  
  gettimeofday(&end, NULL);
  combined_finalize();  

  MPI_Finalize();
  
  printf("Total Time for %d rounds:     %ld\n", num_rounds, ((end.tv_sec * 1000000 + end.tv_usec)-(start.tv_sec * 1000000 + start.tv_usec)));
	
  return 0;
}
