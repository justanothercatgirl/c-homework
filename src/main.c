#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <dlfcn.h>

#include "integral.h"
#include "roots.h"

#define BLINK_TERM "\x1b[5m"
#define RESET_TERM "\x1b[0m"
#define FIFO_NAME "/tmp/compiler.fifo"
#define SYMNAME "f"
#define RDCHUNK 1024

void *sohandle;

// This will be the type of the function
typedef double(*func_t)(double);

typedef double(*calc2_t)(func_t, double, double);
typedef double(*calc3_t)(func_t, double, double, double);
typedef double(*calc4_t)(func_t, double, double, double, double);

// для выбора того, что делать с функцией
enum optype {
	// решение уравнений
	SOL_BINSR, // дихтомии
	SOL_CHORD, // хорды
	SOL_NEWTN, // касательные
	SOL_ITERN, // итерации
	// численное интегрирование
	INT_RECT, // прямоугольники
	INT_TRAP, // трапеции
	INT_SIMP, // симпсон
};

struct funcdescr {
	void *f;
	int argc;
	const char *descr;
};

#define DESCR2 "начало и конец области интегрирования\n"
#define DESCR3 "начальное приближение, допустимую погрешность корня и невязку\n"
#define DESCR4 "левую и правую границы поиска, допустимую погрешность корня и невязку\n"

static struct funcdescr functions[] = {
	[SOL_BINSR] = {&sol_binsr, 4, DESCR4},
	[SOL_CHORD] = {&sol_chord, 4, DESCR4},
	[SOL_NEWTN] = {&sol_newtn, 3, DESCR3},
	[SOL_ITERN] = {&sol_itern, 3, DESCR3},
	[INT_RECT] = {&int_rect, 2, DESCR2},
	[INT_TRAP] = {&int_trap, 2, DESCR2},
	[INT_SIMP] = {&int_simp, 2, DESCR2},
};

// Function that reads Your function, compiles and loads it.
func_t input_and_compile(void) {
	puts("Напечатайте функцию от x используя синтаксис С и функции библиотеки cmath: \n"	
	     "(например, 'atan(x) / (1-exp(2*x))').\n"
	     "Закончите ввод, нажав enter." BLINK_TERM);

	// input the function
	size_t blen = 1024;
	char *buffer = malloc(sizeof (char) * blen);
	blen = read(STDIN_FILENO, buffer, 1024);
	buffer[--blen] = '\0';
	if (blen <= 0) return NULL;

	// format the source
	const char *fmt = 
		"#include <math.h>\n"
		"double " SYMNAME "(double x) { return %s; }\n";
	size_t sourcelen = strlen(fmt) - strlen("%s") + blen + 1;
	char *source = malloc(sizeof (char) * sourcelen);
	snprintf(source, sourcelen, fmt, buffer);

	// spawn the compiler process
	FILE *cc = popen("cc -shared -fPIE -fPIC -o " FIFO_NAME " -xc - -lm", "w");
	fwrite(source, sizeof (char), sourcelen - 1, cc);
	fflush(cc);
	if (ferror(cc)) return NULL;
	if (pclose(cc) != 0) return NULL;

	// load dynamic library 
	sohandle = dlopen(FIFO_NAME, RTLD_NOW);
	if (!sohandle) return NULL;
	func_t ret = dlsym(sohandle, SYMNAME);

	// free resources and return
	free(buffer);
	free(source);
	return ret;
}

// Prompts user to choose input method
enum optype get_optype(void) {
prompt_init:
	fputs("Что вы хотите сделать с функцией?\n"
		"\t1: решить уравнение f(x) = x\n"
		"\t2: решить уравнение f(x) = 0\n"
		"\t3: численно вычислить интеграл f(x)\n"
		"Введите число: ", stdout);
	char c;
	switch (getchar()-'0') {
		case 1: return SOL_ITERN;

		case 2: 
		prompt_sol:
		fputs("\nКаким методом?\n"
			"\t1: дихтомии (двоичный поиск)\n"
			"\t2: хорд\n"
			"\t3: касательных (Ньютона)\n"
			"Введите число:", stdout);
		c = getchar() - '0';
		if (c <= 0 || c > 3) goto prompt_sol;
		return SOL_BINSR + c - 1;

		case 3: 
		prompt_int:
		fputs("\nКаким методом?\n"
			"\t1: прямоугольников\n"
			"\t2: трапеций\n"
			"\t3: Симпсона (парабол)\n"
			"Введите число: ", stdout);
		c = getchar() - '0';
		if (c <= 0 || c > 3) goto prompt_int;
		return INT_RECT + c - 1;

		default: 
		puts("\nОшибка: ведите число от 1 до 3");
		goto prompt_init;
	}
}

int main(void) {
	func_t f = input_and_compile();
	if (!f) goto exit;
	struct funcdescr descr = functions[get_optype()];
	double a, b, c, d;
	printf("Введите %i аргумента через пробел:\n\t%s\n", descr.argc, descr.descr);
	double res;
	switch (descr.argc) {
	case 2:
		scanf("%lf %lf", &a, &b);
		res = ((calc2_t)descr.f)(f, a, b);
		break;
	case 3:
		scanf("%lf %lf %lf", &a, &b, &c);
		res = ((calc3_t)descr.f)(f, a, b, c);
		break;
	case 4:
		scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
		res = ((calc4_t)descr.f)(f, a, b, c, d);
		break;
	default: __builtin_unreachable();
	}
	if (!is_root_ok()) {
		fputs("Корень не посчитан из-за ограничений метода\n", stderr);
		goto exit;
	}
	printf("Результат: %lf\n", res);
exit:
	dlclose(sohandle);
	return 0;
}
