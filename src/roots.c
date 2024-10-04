#include "roots.h"
#include <math.h>
#include <stdio.h>

char root_ok = 1;

// функция для проверки того, успешно ли выполнилась последняя функция
char is_root_ok(void) {
	return root_ok;
}

// метод дихтомии
double sol_binsr(func_t f, double a, double b, double ex, double ey) {
	if (copysign(1.0, f(a)) == copysign(1.0, f(b))) {
		root_ok = 0;
		return NAN;
	}
	
	double mid;
	do {
		mid = (a + b) / 2;
		if (copysign(1.0, f(a)) != copysign(1.0, f(mid))) b = mid;
		else a = mid;
	} while (f(mid) > ey || (b-a) > ex);
	root_ok = 1;
	return mid;
}

// метод хорд
double sol_chord(func_t f, double a, double b, double ex, double ey) {
	if (copysign(1.0, f(a)) == copysign(1.0, f(b))) {
		root_ok = 0;
		return NAN;
	}
	double mid = a + fabs( f(a) / (f(b)-f(a)) ) * (b-a);
	double diff = mid;
	while (f(mid) > ey || fabs(diff - mid) > ex) {
		diff = mid;
		mid = a + fabs( f(a) / (f(b)-f(a)) ) * (b-a);
		if (copysign(1.0, f(a)) != copysign(1.0, f(mid))) b = mid;
		else a = mid;
	}
	root_ok = 1;
	return mid;

}

// метод касательных (Ньютона)
double sol_newtn(func_t f, double x0, double ex, double ey) {
	const double dx = 1e-6;
	double diff = x0+2*ex;

	// check for divergence
	double d2x0 = (f(x0 + 2*dx) - 2*f(x0 + dx) + f(x0))/(dx*dx);
	if (copysign(1.0, d2x0) != copysign(1.0, f(x0))) {
		root_ok = 0;
		return NAN;
	}
	while ((fabs(diff - x0) > ex || f(x0) > ey)) {
		double fx0 = f(x0);
		double dx0 = (f(x0+dx) - fx0) / dx;
		diff = x0;
		x0 -= fx0/dx0;
		if (isinf(x0) || isnan(x0)) {
			root_ok = 0;
			return NAN;
		}
	}
	root_ok = 1;
	return x0;
}

// метод итераций
double sol_itern(func_t f, double x0, double ex, double ey) {
	const double dx = 1e-6;
	double diff = x0 + 2*ex;

	do {
		double dx0 = (f(x0+dx) - f(x0))/dx;
		if (fabs(dx0) > 1.0) {
			root_ok = 0;
			return NAN;
		}
		diff = x0;
		x0 = f(x0);
		printf("x0=%lf, f(x0)=%lf\n", x0, f(x0));
		getchar();
	} while (fabs(diff - x0) > ex || fabs(f(x0) - x0) > ey);
	root_ok = 1;
	return x0;
}



