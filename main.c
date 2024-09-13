#include <stdio.h>

#define EPSILON	0.001*0.001
#define MAX_ITERATIONS 100

int main(void) {
	double x;
	double result;
	double last_elem;
	int i;
	
	printf("Input a number\n");
	scanf("%lf", &x);
	last_elem = result = x;

	for (i = 1; i < MAX_ITERATIONS; ++i) {
		last_elem = last_elem * x * x / (2*i * (2*i+1));
		if (last_elem*last_elem <= EPSILON) break;
		result += last_elem * ( (i%2 * -2) + 1 );
	}
	printf("result = %lf; iterations = %i; last_elem = %lf\n", result, i, last_elem);
	return 0;
}
