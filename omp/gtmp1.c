#include <omp.h>
#include <assert.h>
#include <stdio.h>
#include "gtmp.h"

unsigned int count;
unsigned int p;
unsigned char sense;

void (*callout_func)();

/* A sense reversing centralized barrier */

void gtmp_init(int num_threads)
{
    p = num_threads;
    //No threads means only one thread?
    if(p == 0) p = 1;
    count = p;
    sense = 1;
    
    callout_func = NULL;
}

void gtmp_barrier()
{
    //Check invariants
    assert(0<count);
    assert(0<p);
    
    //Runtime error in case OpenMP parallel block, but could be a barrier for a subset of the threads in the blocks
    if(p > omp_get_num_threads() || p != omp_get_num_threads())
    {
      fprintf(stdout, "Warning: Barrier initialized with %d actual %d\n", p, omp_get_num_threads());
    }
    
    static char local_sense;
    #pragma omp threadprivate(local_sense)
    {
      local_sense = 1 & (~sense);
      #pragma omp atomic
      count = count - 1;
      
      //printf("Thread %d barrier local %d sense %d count %d out of %d\n", omp_get_thread_num(), local_sense, sense, count, omp_get_num_threads());
      
      if((0 == count))
      {
          //Callout integration point, used for further sync with other barrier implementations
          if(NULL != callout_func)
          {
              callout_func();
          }
          
          count = p;
          sense = local_sense;
          //printf("#### Thread %d barrier local %d sense %d count %d out of %d\n", omp_get_thread_num(), local_sense, sense, count, omp_get_num_threads());
      }
      else
      {
          while(sense != local_sense)
          {
              {
                //printf("Thread %d barrier local %d but sense %d count %d out of %d\n", omp_get_thread_num(), local_sense,sense, count, omp_get_num_threads());
                //sleep(2);
              }
          }
          //printf("**** Thread %d barrier local %d sense %d count %d out of %d\n", omp_get_thread_num(), local_sense, sense, count, omp_get_num_threads());
      }
   }
}

void set_integration_callout(void (*func)())
{
    callout_func = func;
}

void gtmp_finalize(){
    count = 0;
    p = 0;
}

