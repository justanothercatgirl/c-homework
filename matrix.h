#ifndef JAC_MATRIX_H
#define JAC_MATRIX_H

#include <stdio.h>
#include <stddef.h> 
#include <stdlib.h>
#include <math.h>

// Черная магия, чтобы сделать ввод более безопасным
#define SKIP_INPUT do { 		\
	int c; 				\
	do { c = getchar(); } 		\
	while (c != '\n' && c != EOF); 	\
} while(0)


typedef size_t usz;
typedef ssize_t isz;

typedef double(*func_t)(double);

// матрица
struct matrix {
	double **data;
	usz rows, columns;
};

// матрица функций (Function MATRIX)
struct fmatrix {
	func_t **funcs;
	usz rows, columns;
};

// треугольная верхняя матрица. (Triangle Up MATRIX)
struct tumatrix {
	double **data;
	usz size;
};

// генерирует случайное число в диапазоне, используя внутреннее представление типа double в памяти
double frand(double min, double max);

// создаёт матрицу
struct matrix matrix_new(usz rows, usz columns);
// удаляет матрицу
void matrix_free(struct matrix m);
// выводит матрицу
void matrix_print(struct matrix *m);
// заполняет матрицу случайными числами
void matrix_fill(struct matrix *m, double min, double max);
// поэлементно применяет функцию из fm на m
void matrix_apply(struct matrix *m, const struct fmatrix *fm);

// создаёт новую треугольную матрицу
struct tumatrix tumatrix_new(usz size);
// удаляет треугольную матрицу
void tumatrix_free(struct tumatrix m);
// берёт i,j-ый элемент треугольной матрицы
double tumatrix_at(const struct tumatrix *m, usz row, usz column);
// умножает треугольные матрицы
struct tumatrix tumatrix_mul(const struct tumatrix *a, const struct tumatrix *b);
// определитель треугольной матрицы
double tumatrix_det(const struct tumatrix *m);
// заполняет треугольную матрицу
void tumatrix_fill(struct tumatrix *m, double min, double max);
// выводит треугольную матрицу на экран
void tumatrix_print(const struct tumatrix *m);

#endif // JAC_MATRIX_H
