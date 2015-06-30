CFLAGS=-Wall -g

all: ex1/ex1 ex3/ex3 ex4/ex4

clean:
	rm -f ex1/ex1
	rm -f ex3/ex3
	rm -f ex4/ex4

.PHONY: all clean
