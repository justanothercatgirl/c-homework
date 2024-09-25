
all: t1_a t1_b t23 test.so

T2FLAGS := -Wall

ifneq ($(OS),Windows_NT)
	T2FLAGS += -DDYNAMIC_LOAD
	T2LINKS = -lm -ldl
	ifneq (, $(shell which gnuplot))
		T2FLAGS += -DPIPE_GNUPLOT
		ifeq (, ${DISPLAY})
			T2FLAGS += -DTERMINAL_PLOT
		endif
	endif
endif

t1_a: main_1.c
	$(CC) -DPROG_SINUS_SUM $^ -o $@

t1_b: main_1.c
	$(CC) -DPROG_MONTE_CARLO $^ -o $@

t23: main_23.c
	$(CC) $(T2FLAGS) $^ -o $@ $(T2LINKS)

test.so: test_f.c
	$(CC) -fPIC -fPIE -shared $^ -o $@ 

.PHONY: clean

clean:
	rm -rf t1_a t1_b t23 test.so

