
CFLAGS = -O2 -Wall -Wextra -Werror -Iinclude

all: task4

build:
	mkdir -p build

build/roots.o: src/roots.c build
	$(CC) $(CFLAGS) -c -o $@ $<

build/integral.o: src/integral.c build 
	$(CC) $(CFLAGS) -c -o $@ $<

task4: build/roots.o build/integral.o src/main.c 
	$(CC) $(CFLAGS) -o $@ $^ -lm -ldl 

.PHONY: run clean

run: task4
	./task4

clean:
	rm -f build/*.o task4
