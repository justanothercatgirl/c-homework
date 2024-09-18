#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define GEN_MAX 1000000000

static int tenpow(int pow) {
	int base = 10;
	int ret = 1;
	while (pow) {
		if (pow & 0b1)
			ret *= base;
		base *= base;
		pow >>= 1;
	}
	return ret;
}

static char get_at_pos(unsigned int n, int pos) {
	n /= tenpow(pos);
	return n % 10;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int arr[10] = {0};
	unsigned int gen;

	int iters, pos;
	if (argc == 3) {
		iters = atoi(argv[1]);
		pos = atoi(argv[2]);
		if (!iters || !pos) goto fail;
	} else {
		fputs("Iterations: ", stdout);
		if (!scanf("%i", &iters)) goto fail;
		fputs("Position (10^N): ", stdout);
		if (!scanf("%i", &pos)) goto fail;
	}

	for (int _ = 0; _ < iters; ++_) {
		gen = rand() % GEN_MAX;
#ifdef PRINT_GENERATED
		printf("%i\n", gen);
#endif
		switch (get_at_pos(gen, pos)) {
			case 0: ++arr[0]; break;
			case 1: ++arr[1]; break;
			case 2: ++arr[2]; break;
			case 3: ++arr[3]; break;
			case 4: ++arr[4]; break;
			case 5: ++arr[5]; break;
			case 6: ++arr[6]; break;
			case 7: ++arr[7]; break;
			case 8: ++arr[8]; break;
			case 9: ++arr[9]; break;
			default: __builtin_unreachable();
		}
	}
	
	printf("count of numbers at position %i:\n", pos);
	for (int i = 0; i < 10; ++i) 
		printf("\t%i: %i\n", i, arr[i]);

	return EXIT_SUCCESS;
fail:
	fputs("Input a valid number\n", stderr);
	return EXIT_FAILURE;
}

