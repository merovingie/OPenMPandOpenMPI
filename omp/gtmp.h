#ifndef GTMP_H
#define GTMP_H

void gtmp_init(int num_threads);
void gtmp_barrier();
void gtmp_finalize();
void set_integration_callout(void (*func)());

#endif
