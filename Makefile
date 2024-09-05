
all: main libs

# the main program
main: main.c
	cc -o main main.c

# available libraries 
.PHONY: libs
libs: libs/*
	cc -shared libs/libsq.c -lm -lc -o libsq
	cc -shared libs/libsin.c -lm -lc -o libsin
	cc -shared libs/libpoly.c -lm -lc -o libpoly

# clear build
.PHONY: clean
clean:
	rm -f main libsq libsin libpoly
