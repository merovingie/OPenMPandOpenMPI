CC = gcc
MPICC = mpicc

OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CFLAGS = -g -std=gnu99 -Wall $(OMPFLAGS)
LDLIBS = $(OMPLIBS) -lm

INCLUDES = . ../omp ../mpi /usr/lib/x86_64-linux-gnu/mpich/include /usr/include/mpich

all: combined_omp1_mp1 combined_omp1_mp2 combined_omp2_mp1 combined_omp2_mp2

SRCS_OMP1_MPI1 := $(wildcard *.c) ../mpi/gtmpi1.c ../omp/gtmp1.c
OBJS_OMP1_MPI1 = $(SRCS_OMP1_MPI1:.c=.o)

SRCS_OMP2_MPI2 := $(wildcard *.c) ../omp/gtmp2.c ../mpi/gtmpi2.c
OBJS_OMP2_MPI2 = $(SRCS_OMP2_MPI2:.c=.o)

SRCS_OMP1_MPI2 := $(wildcard *.c) ../omp/gtmp1.c ../mpi/gtmpi2.c
OBJS_OMP1_MPI2 = $(SRCS_OMP1_MPI2:.c=.o)

SRCS_OMP2_MPI1 := $(wildcard *.c) ../omp/gtmp2.c ../mpi/gtmpi1.c
OBJS_OMP2_MPI1 = $(SRCS_OMP2_MPI1:.c=.o)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES:%=-I%) $< -o $@

combined: combined_omp1_mp1

combined_omp1_mp1: $(OBJS_OMP1_MPI1) 
	$(MPICC) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

combined_omp2_mp2: $(OBJS_OMP2_MPI2) 
	$(MPICC) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

combined_omp1_mp2: $(OBJS_OMP1_MPI2) 
	$(MPICC) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

combined_omp2_mp1: $(OBJS_OMP2_MPI1) 
	$(MPICC) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -rf *.o $(OBJS_OMP1_MPI1) $(OBJS_OMP2_MPI2) $(OBJS_OMP1_MPI2) $(OBJS_OMP2_MPI1) *.dSYM combined

