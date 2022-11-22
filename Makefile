
CC=gcc
CFLAGS=-I.
DEPS = umalloc.h
OBJ = memgrind.o umalloc.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

memgrind: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o memgrind

all: memgrind
