#include "matrix.h"

// Почему так? Числа более рандомные получаются. А ещё это быстро.
double frand(double min, double max) {
	double ret = 0;
	unsigned long *ret_mem = (unsigned long*)&ret;
	*ret_mem = (	((unsigned long)rand() << 21) 
			^ (unsigned long)rand()
		) & ~(1UL << 63);
	int expa, expb;
	frexp(min, &expa);
	frexp(max, &expb);
	int _exp = rand() % (expb-expa + 2) + expa - 1 ;
	*ret_mem |= ((unsigned long)abs(_exp) << 52) | 
		(_exp < 0 ? 0UL << 62 : 1UL << 62);
	return fmod(ret, max-min) + min;
}

struct matrix matrix_new(usz rows, usz columns) {
	double ** data = malloc(rows * sizeof (double*));
	for (usz i = 0; i < rows; ++i)
		data[i] = malloc(columns * sizeof (double));
	return (struct matrix) {
		.data = data,
		.rows = rows,
		.columns = columns,
	};
}

void matrix_free(struct matrix m) {
	for (usz i = 0; i < m.rows; ++i) 
		free(m.data[i]);
	free(m.data);
}

void matrix_print(struct matrix *m) {
	for (usz i = 0; i < m->rows; ++i) {
		printf("| ");
		for (usz j = 0; j < m->columns; ++j) {
			printf("%10.3g ", m->data[i][j]);
		}
		printf("|\n");
	}
}

void matrix_fill(struct matrix *m, double min, double max) {
	for (usz i = 0; i < m->rows; ++i) 
		for (usz j = 0; j < m->columns; ++j)
			m->data[i][j] = frand(min, max);
}

void matrix_apply(struct matrix *m, const struct fmatrix *fm) {
	if (m->rows != fm->rows || m->columns != fm->columns) return;
	for (usz i = 0; i < fm->rows; ++i)
		for (usz j = 0; j < fm->columns; ++j)
			m->data[i][j] = (fm->funcs[i][j])(m->data[i][j]);
}


struct tumatrix tumatrix_new(usz size) {
	double **data = malloc(sizeof (double*) * size);
	for (usz i = 0; i < size; ++i) 
		data[i] = malloc(sizeof (double) * (size - i));
	return (struct tumatrix){.data = data, .size = size};
}

void tumatrix_free(struct tumatrix m) {
	for (usz i = 0; i < m.size; ++i)
		free(m.data[i]);
	free(m.data);
}

double tumatrix_at(const struct tumatrix *m, usz row, usz column) {
	if (row > column) return 0.0;
	return m->data[row][column - row];
}

struct tumatrix tumatrix_mul(const struct tumatrix *a, const struct tumatrix *b) {
	if (a->size != b->size) return (struct tumatrix){0};
	struct tumatrix c = tumatrix_new(a->size);
	for (usz i = 0; i < a->size; ++i) {
		for(usz j = i; j < a->size; ++j) {
			double psum = 0;
			// maybe r should go from i to j instead, will see
			for (usz r = 0; r < a->size; ++r)
				psum += tumatrix_at(a, i, r) * tumatrix_at(b, r, j);
			c.data[i][j-i] = psum;
		}
	}
	return c;
}

// Определитель треугольной матрицы равен произведению элементов её главной диагонали 
// [https://ru.wikipedia.org/wiki/Треугольная_матрица]
double tumatrix_det(const struct tumatrix *m) {
	double prod = 1.0;
	for (usz i = 0; i < m->size; ++i)
		prod *= tumatrix_at(m, i, i);
	return prod;
}

void tumatrix_fill(struct tumatrix *m, double min, double max) {
	for (usz i = 0; i < m->size; ++i)
		for (usz j = 0; j < m->size - i; ++j)
			m->data[i][j] = frand(min, max);
}

void tumatrix_print(const struct tumatrix *m) {
	for (usz i = 0; i < m->size; ++i) {
		printf("| ");
		for (usz j = 0; j < m->size; ++j) {
			printf("%10.3g ", tumatrix_at(m, i, j));
		}
		printf("|\n");
	}
}
