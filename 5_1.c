#include <stdio.h>
#include <time.h>

#include "matrix.h"

#define RAND_CHUNK = 0x7FFFFFFF;

enum optype { ADD, SUB, MUL, DIV, POW, NOP };

// #pragma - просто способ заткнуть компилятор, чтобы он не выдавал предупреждений
// делает lookup table для операции, основываясь на символе
// по сути выполняет роль dict() в питоне или std::unordered_map в C++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winitializer-overrides"
enum optype op_lookup[255] = {
	[0 ... 254] = NOP,
	['+'] = ADD,
	['-'] = SUB,
	['*'] = MUL,
	['/'] = DIV,
	['^'] = POW,
};
#pragma GCC diagnostic pop

// исполняет операцию над матрицами
void inplace_matrix_op(struct matrix *restrict a, struct matrix const *b, const enum optype t) {
	if (a->rows != b->rows || a->columns != b->columns) return;
	for(usz i = 0; i < b->rows; ++i) {
		for (usz j = 0; j < b->columns; ++j) {
			switch (t) {
			case ADD: a->data[i][j] += b->data[i][j]; break;	
			case SUB: a->data[i][j] -= b->data[i][j]; break;	
			case MUL: a->data[i][j] *= b->data[i][j]; break;	
			case DIV: a->data[i][j] /= b->data[i][j]; break;	
			case POW: a->data[i][j] = pow(a->data[i][j], b->data[i][j]); break;
			case NOP: return;
			}
		}
	}
}

int main(void) {
	srand(time(NULL));
	// выбираем размер матрицы
size_selection:
	printf("Input the size of the matrixes (rows, columns) separated by a whitespace:\n");
	isz row, col;
	scanf("%zu %zu", &row, &col);
	if (row < 0 || col < 0) {
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
	struct matrix A = matrix_new(row, col), B = matrix_new(row, col);
	matrix_fill(&A, a, b);
	matrix_fill(&B, a, b);
	puts("A = "); matrix_print(&A);
	puts("B = "); matrix_print(&B);
	// выбираем, что с матрицами делать
op_selection:
	SKIP_INPUT;
	printf("Input the operation ( + - * / ^ ): ");
	enum optype op = op_lookup[getchar()];
	if (op == NOP) goto op_selection;
	// выполняем операцию
	inplace_matrix_op(&A, &B, op);
	puts("result:");
	matrix_print(&A);
	// освобождаем память
	matrix_free(A);
	matrix_free(B);
	// ;)
	return 0;
}
