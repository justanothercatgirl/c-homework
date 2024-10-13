#include <stdio.h>
#include <time.h>

#include "matrix.h"

int main(void) {
	srand(time(NULL));
	// выбираем размер матрицы
size_selection:
	printf("Input the size of the matrixes:\n");
	isz size;
	scanf("%zu", &size);
	if (size < 0) {
		SKIP_INPUT;
		goto size_selection;
	}
	// выбираем интервал для генерации случайных чисел
interval_selection:
	SKIP_INPUT;
	printf("Input the borders of the interval for generated values, separated by a whitespace:\n");
	double a, b;
	if (scanf("%lf %lf", &a, &b) != 2) goto interval_selection;
	// создаём матрицы
	struct tumatrix A = tumatrix_new(size);
	struct tumatrix B = tumatrix_new(size);
	tumatrix_fill(&A, a, b);
	tumatrix_fill(&B, a, b);
	puts("A = "); tumatrix_print(&A);
	puts("B = "); tumatrix_print(&B);
	printf("det A = %lf\n", tumatrix_det(&A));
	printf("det B = %lf\n", tumatrix_det(&B));
	struct tumatrix C = tumatrix_mul(&A, &B);
	puts("C = A*B = "); tumatrix_print(&C);
	printf("det C = %lf\n", tumatrix_det(&C));
	tumatrix_free(A);
	tumatrix_free(B);
	tumatrix_free(C);
	return 0;
}
