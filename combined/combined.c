#include "gtmp.h"
#include "gtmpi.h"
#include <stdio.h>

void wait_for_node()
{
    //printf("OMP barrier finished awaiting MPI barrier\n");
    gtmpi_barrier();
}

void combined_init(int num_processes, int num_threads)
{
    gtmpi_init(num_processes);
    gtmp_init(num_threads);
    set_integration_callout(&wait_for_node);
}

void combined_barrier()
{
    gtmp_barrier();
}

void combined_finalize()
{
    gtmpi_finalize();
    gtmp_finalize();
}
