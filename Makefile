# Should build library code and memgrind with a makeall and delete all files it created with make clean
# Should build library as compiled object

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