numrep: main.c
	cc $^ -o $@

.PHONY: clean
clean: 
	rm -rf numrep
