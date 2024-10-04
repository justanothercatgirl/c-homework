// Это пример использования функций высшего порядка
#include <stdio.h>
#include <math.h>

// Синтаксис непонятный, но так я создаю "синоним"
// к указателю на функцию, принимающую 2 double и возвращающую double
typedef double(*func_t)(double, double);


// Эта функция "передаёт" аргументы в функцию и выводит результат
void calculate_and_print(func_t function, double arg1, double arg2) {
	printf("F(%lf, %lf) = %lf\n", arg1, arg2, function(arg1, arg2));	
}

// считает сумму x и y
double sum(double x, double y) {
	return x + y;
}

// считает произведение x и y
double prod(double x, double y) {
	return x * y;
}

int main(void) {
	// передаёшь функцию в функцию по имени,
	// как самый обычный аргумент
	calculate_and_print(sum,  420, 69);
	calculate_and_print(prod, 420, 69);
	// fmax - функция стандартной библиотеки
	// возвращает максимальное из двух чисел
	calculate_and_print(fmax, 420, 69);
}
