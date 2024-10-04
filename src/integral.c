#include "integral.h"

// эта переменная позволяет контролировать точность интегрирования
// и может быть установлена в файле main
double accrc = 10000.0;

// интеграл прямоугольниками
double int_rect(func_t f, double a, double b) {
	const double dx = (b-a) / accrc;
	const double dx_2 = dx / 2.0;
	double sum = 0.0;
	for (double i = a + dx; i < b; i += dx)
		sum += f(i - dx_2) * dx;
	return sum;
}

// интеграл трапециями
double int_trap(func_t f, double a, double b) {
	const double dx = (b-a) / accrc;
	double sum = 0.0;
	for (double i = a; i < b; i += dx)
		sum += dx * (f(i) + f(i+dx))/2 ;
	return sum;
}

// интегрирование параболами
double int_simp(func_t f, double a, double b) {
	const double dx = (b-a) / accrc;
	const double dx_2 = dx / 2;
	const double dx_6 = dx / 6;
	double sum = 0.0;
	for(double i = a; i < b; i += dx) {
		sum += dx_6 * (f(i) + 4 * f(i + dx_2) + f(i + dx));
	}
	return sum;
}
