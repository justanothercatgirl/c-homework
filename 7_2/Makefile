
all: main

build: 
	mkdir -p build

build/%.o: src/%.c include/%.h build
	$(CC) -c -g -ggdb -o $@ $< -Iinclude

main: main.c build/drawable.o build/figure.o build/input.o
	$(CC) -o $@ $^ -lm -lX11 -Iinclude

.PHONY: clean

clean:
	$(RM) *.o main build/*
