#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
typedef double(*f_t)(double);

enum optype {
	// решение уравнений
	SOL_BINSR, // дихтомии
	SOL_HORDE, // хорды
	SOL_NEWTN, // касательные
	SOL_ITERN, // итерации
	// численное интегрирование
	INT_RECT, // прямоугольники
	INT_TRAP, // трапеции
	INT_SIMP, // симпсон
};

// Function that reads Your function, compiles and loads it.
f_t input_and_compile(void) {
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
	f_t ret = dlsym(sohandle, SYMNAME);

	// free resources and return
	free(buffer);
	free(source);
	return ret;
}

// Prompts user to choose input method
enum optype get_optype(void) {
	return INT_SIMP;
}

int main(int argc, char *argv[]) {
	(void)argc; (void)argv;
	f_t f = input_and_compile();
	if (!f) goto exit;
	double sln = sol_binsr(f, 0, 10, 0.001, 0.001);
	printf("%lf\n", sln);
	sln = sol_binsr(f, 0, 10, 0.001, 0.001);
	printf("%lf\n", sln);
	sln = sol_newtn(f, 5, 0.001, 0.001);
	printf("%lf\n", sln);
	sln = sol_itern(f, 0, 0.001, 0.001);
	printf("%lf\n", sln);

exit:
	dlclose(sohandle);
	return 0;
}
