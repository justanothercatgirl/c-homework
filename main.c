#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

struct point {
	float x, y;
};

struct circle {
	// center not needed for this task
	/*struct point center;*/
	float rad;
};

char in_circle(const struct circle *c, const struct point p) {
	/*p.x -= c->x;*/
	/*p.y -= c->y;*/
	return p.x * p.x + p.y * p.y < c->rad*c->rad;
}

float calculate_pi(int iterations) {
	int in = 0, out = 0;
	int *arr[2] = {&out, &in};
	const struct circle c = {.rad = 1.0f};
	for (int i = 0; i < iterations; ++i) {
		struct point p = {
			.x = rand() % 2000000u / 1e6f - 1,
			.y = rand() % 2000000u / 1e6f - 1
		};
		++*arr[in_circle(&c, p)];
	}
	return (float)in/iterations * 4;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int iters;
	if (argc == 2) {
		iters = atoi(argv[1]);
	} else {
		fputs("Iterations: ", stdout);
		if (!scanf("%i", &iters)) {
			fputs("Input a valid number\n", stderr);
			return EXIT_FAILURE;
		}
	}
	float pi = calculate_pi(iters);
	float err = fabs(M_PI-pi)/M_PI;
	printf(	"calculated pi: %.4f, actual pi: %.4f\n"
		"error: %.2f%%\n",
		pi, M_PI, err * 100);
	return EXIT_SUCCESS;
}
