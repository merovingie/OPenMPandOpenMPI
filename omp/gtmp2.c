#include <omp.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gtmp.h"

struct treenode {
  char havechild [4];
  char childnotready [4];
  char * parentpointer;
  char * childpointers [4];
  char parentsense;
  char flag;
} treenode;

unsigned int P = 0;
struct treenode * nodes;

void (*callout_func)();

void gtmp_init(int num_threads){
    callout_func = NULL;
    P = num_threads;
    if(P<2)
    {
    P = 1;
    }
    nodes = calloc(P,sizeof(struct treenode));
    
    for(int i=0; i<P; i++)
    {
        for(int j=0; j<4; j++)
        {
            if((4*i + j) < (P-1))
            {
              nodes[i].havechild[j] = 1;
            }
            else
            {
              nodes[i].havechild[j] = 0;
            }
        }
        
        /*
        printf("nodes[i].havechild bytes: %d %d %d %d int value %d\n",nodes[i].havechild[0]
          ,nodes[i].havechild[1]
          ,nodes[i].havechild[2]
          ,nodes[i].havechild[3], * ( (int *) (&nodes[i].havechild[0]) ) );
        */
        
        nodes[i].parentpointer = i > 0 ? &(nodes[(int)floor((i-1)/4)].childnotready[(i-1)%4]) : NULL;
        
        for(int j=0; j<2; j++)
        {
          int child_idx = ((2*i) + 1 + j);
          nodes[i].childpointers[j] = ( child_idx < P ) ? &(nodes[child_idx].parentsense) : NULL;
        }
        
        * ( (int *) (&nodes[i].childnotready[0]) ) =  * ( (int *) (&nodes[i].havechild[0]) );
        
        nodes[i].parentsense = 0;
    }
}

void gtmp_barrier()
{
    //Check invariants
    assert( 0 < P) ;
    
    {
        static char local_sense = 1;
        #pragma omp threadprivate(local_sense)
        
        int vpid = omp_get_thread_num();
        
        //Runtime error in case OpenMP parallel block, but could be a barrier for a subset of the threads in the blocks
        if(P > omp_get_num_threads() || P != omp_get_num_threads())
        {
          fprintf(stdout, "Warning: Barrier initialized with %d actual %d\n", P, omp_get_num_threads());
        }
        int * spin_ptr = (int *) &nodes[vpid].childnotready[0];
        
        while( 0 != *spin_ptr);
        *spin_ptr =  * ( (int *) (&nodes[vpid].havechild[0]) );
        
        if(NULL != nodes[vpid].parentpointer)
        {
          (* nodes[vpid].parentpointer) = 0;
        }
        
        //If not root wait for sense reversal by parent
        if(vpid != 0)
        {
            while(nodes[vpid].parentsense != local_sense);
        }
        else
        {
            //Callout integration point, used for further sync with other barrier implementations
            if(NULL != callout_func)
            {
                callout_func();
            }
        }
        
        //Signal children nodes
        for(int j=0; j<2; j++)
        {
          if(nodes[vpid].childpointers[j]!= NULL)
          {
            *(nodes[vpid].childpointers[j]) = local_sense;
          }
        }
        
        local_sense = 1 & (~local_sense);
    }
}

void set_integration_callout(void (*func)())
{
    callout_func = func;
}

void gtmp_finalize(){
    free(nodes);
}

