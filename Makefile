
VAR ?= 1

all: 6_1 6_2

gen: gen.c
	$(CC) -o $@ $<

6_1: 6_1.c gen
	$(CC) -o $@ $<

6_2: 6_2.c
	$(CC) -o $@ $< -lm

.PHONY: clean run1 run2

run1: gen 6_1
	./gen 100 in.1
	./6_1 -i in.1 -o out.1 $(VAR)

run2: 6_2
	./6_2 -1 t2.1 -2 t2.2 -a 0 -b 3.141592657 -x 0.1

clean:
	rm -f gen 6_1 6_2 in.1 out.1 t2.1 t2.2
