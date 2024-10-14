#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOFBUF 1024

// какую функцию будем дифференцировать
double (*const func)(double) = sin;

// пункт 1
void generate_file1(FILE *f, double min, double max, double dx) {
	for (double i = min; i < max; i += dx) {
		fprintf(f, "x = %lf\tf(x) = %lf\t f'(x) = %lf\n",
				i, func(i), (func(i+dx) - func(i)) / dx
		       );
	}
}

void generate_file2(FILE *in, FILE *out) {
	char *buf = malloc(SIZEOFBUF);
	size_t bufsize = SIZEOFBUF;
	const char *delim = "xf()'= \t\n";
	while (!feof(in)) {
		getline(&buf, &bufsize, in);
		char * p = strtok(buf, delim);
		do fprintf(out, "%s\t", p);
		while ((p = strtok(NULL, delim)) != NULL);
		fputc('\n', out);
	}
}

int main(int argc, char *argv[]) {
	// стандартный цикл обработки аргументов argv
	if (argc != 5*2 + 1) {
		fprintf(stderr, 
			"usage: \n" "%s -1 FILE1 -2 FILE2 -a MIN -b MAX -x DX\n"
			"\t FILE1: пункт 1, FILE2: пункт2\n"
			"\tMIN: нижняя граница дифференцирования, MAX: верхняя\n"
			"\tDX: шаг дифференцирования\n", 
			*argv);
		return 1;
	}
	const char *f1, *f2;
	double min, max, dx;
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "-1"))
			f1 = argv[++i];
		if (!strcmp(argv[i], "-2")) 
			f2 = argv[++i];
		if (!strcmp(argv[i], "-a"))
			min = atof(argv[++i]);
		if (!strcmp(argv[i], "-b"))
			max = atof(argv[++i]);
		if (!strcmp(argv[i], "-x"))
			dx = atof(argv[++i]);
	}

	FILE *f1f = fopen(f1, "w+");
	if (!f1f) return 1;
	generate_file1(f1f, min, max, dx);
	fclose(f1f);
	f1f = fopen(f1, "r");
	FILE *f2f = fopen(f2, "w+");
	if (!f2f) return 1;
	generate_file2(f1f, f2f);
	return 0;
}
