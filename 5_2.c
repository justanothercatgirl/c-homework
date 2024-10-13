#include <stdio.h>
#include <time.h>
#include "matrix.h"

#define MRX_SZ 6
#define add_func(name, func) 	\
	double name (double x){ \
		return func;	\
	}

add_func(x2, x*x)
add_func(x2_sinx, x2(sin(x)))
add_func(sqrtx_x2, sqrt(x) * x2(x))
add_func(sqrt_sin_x2, sqrt(sin(x2(x))))

const func_t funcs[MRX_SZ] = {
	x2, sin, sqrt, x2_sinx, sqrtx_x2, sqrt_sin_x2
};

int main(void) {
	srand(time(NULL));
	// выбираем интервал для генерации случайных чисел
interval_selection:
	printf("Input the borders of the interval for generated values, separated by a whitespace:\n");
	double a, b;
	if (scanf("%lf %lf", &a, &b) != 2)  {
		SKIP_INPUT;
		goto interval_selection;
	}
	// создаём матрицы
	struct matrix m = matrix_new(1, MRX_SZ);
	matrix_fill(&m, a, b);
	struct matrix f_mem = matrix_new(1, MRX_SZ);
	struct fmatrix f = *(struct fmatrix*)&f_mem;
	for (usz i = 0; i < f.columns; ++i) f.funcs[0][i] = funcs[i];
	// делаем грязную работу
	puts("A = "); matrix_print(&m);
	matrix_apply(&m, &f);
	puts("f(A) = "); matrix_print(&m);
	// очищаем память
	matrix_free(m);
	matrix_free(f_mem);
	return 0;
}
