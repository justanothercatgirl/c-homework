
/*
 * Сотов Константин Алексеевич, 117 группа
 * sotov@twistea.su	+7 914 329 50 01
 *
 * Данный код загружает динамическую библиотеку,
 * имя которой передано в аргументах командной строки,
 * и считает интеграл функции "f", если она есть
 * в этой динамической библиотеке.
 */

#include <stdio.h> 	// printf, scanf
#include <stdlib.h> 	// exit
#include <dlfcn.h>	// dlopen, dlsym, dlclose


// Integrates function f from a to b with <sums> steps.
// Uses "naive" and straightforward integration technique
// Assumes a < b
double integral(double (*f)(double), double a, double b, int sums) {
	double result = 0;
	const double dx = (b-a)/sums;
	for (double i = a; i < b; i += dx)
		result += dx * f(i);
	return result;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Недостаточно аргументов. использование:\n"
			"%s <путь к динамической библиотеке>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// Open the dynamic library
	void* handle = dlopen(argv[1], RTLD_NOW);
	if (handle == NULL) {
		printf("ERROR: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	// Locate the function
	void* f = dlsym(handle, "f");
	if (f == NULL) {
		printf("ERROR: %s", dlerror());
		dlclose(handle);
		exit(EXIT_FAILURE);
	}

	// Input-output block
	const char* fullname 
		= "Сотов Константин Алексеевич";
	printf("Автор: %s\n", fullname);
	double a, b;
	puts("Введите нижнюю и верхнюю границы интегрирования через пробел");
	scanf("%lf %lf", &a, &b);
	printf("Интеграл f от %lf до %lf = %lf\n",
		a, b, integral(f, a, b, 10000));

	exit(EXIT_SUCCESS);
}
