#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "gtmp.h"

void check_values(int* array, int length, int value) {
    for (int i = 0; i < length; i++) {
        if(array[i]!=value)
        {
          printf("Thread %d Expected %d at index %d found %d\n",omp_get_thread_num(),value,i,array[i]);
          //while(1);
        }
        assert(array[i]==value);
    }
}

int main(int argc, char** argv)
{
  int num_threads, num_iter=10;

  if (argc < 2){
    fprintf(stderr, "Usage: ./harness [NUM_THREADS]\n");
    exit(EXIT_FAILURE);
  }
  num_threads = strtol(argv[1], NULL, 10);

  omp_set_dynamic(0);
  if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");

  omp_set_num_threads(num_threads);

  gtmp_init(num_threads);

  int* array = calloc(sizeof(int), num_threads);
  if (array == NULL) {
    fprintf(stderr,"Failed to allocate memory for sentinel array");
    exit(EXIT_FAILURE);
  }
  int index = 0;

#pragma omp parallel shared(num_threads)
   {
     int my_index = __atomic_fetch_add(&index, 1, __ATOMIC_SEQ_CST);
     int i;
     //printf("Thread %d starting\n",my_index);
     for(i = 0; i < num_iter; i++){
       fflush(stdout);
       //printf(">>>>>>>>>>>> Thread %d Iteration %d current %d<<<<<<<<<<<<\n",my_index,i,array[my_index]);
       check_values(array, num_threads, i);
       gtmp_barrier();
       //printf("Thread %d went ahead 1\n",my_index);

       array[my_index] = i + 1;
       //printf("oooooooooooo Thread %d Iteration %d new %doooooooooooo\n",my_index,i,array[my_index]);
       gtmp_barrier();
       //printf("Thread %d went ahead 2\n",my_index);
     }
   }

   gtmp_finalize();

   return 0;
}
