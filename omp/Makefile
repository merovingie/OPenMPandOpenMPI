.PHONY: clean test1 test2

OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
LINK = mpicc

CFLAGS = -g -std=gnu99 -I. -Wall $(OMPFLAGS)
LDLIBS = $(OMPLIBS) -lm
INCLUDES = /usr/include/mpich

MP_SRC1 = gtmp1.c
MP_SRC2 = gtmp2.c

all: mp1 mp2

mp1: gtmp1.c harness.o
	$(LINK) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

mp2: gtmp2.c harness.o
	$(LINK) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

%.o: %.c
	$(CC) -c -v $(CFLAGS) -I$(INCLUDES) $(OMPFLAGS)  $< -o $@

test1: gtmp1.c assert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	( \
		for i in 0 5 10 20 50; do        \
			echo testing with $$i threads; \
			./test1 $$i || exit 1 && echo passed ;   \
		done \
	) && (echo cleaning up test files && rm -f ./test1 ./assert.o)

test2: gtmp2.c assert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	( \
		for i in 0 5 10 20 50; do        \
			echo testing with $$i threads; \
			./test2 $$i || exit 1 && echo passed ;   \
		done \
	) && (echo cleaning up test files && rm -f ./test2 ./assert.o)

clean:
	rm -rf *.o *.dSYM mp1 mp2 test1 test2
