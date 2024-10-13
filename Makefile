
all: 5_1 5_2 5_3

5_1: 5_1.c matrix.c
	cc -o $@ $^ -lm 

5_2: 5_2.c matrix.c
	cc -o $@ $^ -lm 

5_3: 5_3.c matrix.c
	cc -o $@ $^ -lm 

.PHONY: clean

clean:
	rm -f 5_1 5_2 5_3
