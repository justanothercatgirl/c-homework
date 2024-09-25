#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef PIPE_GNUPLOT
	#include <unistd.h>
	#include <fcntl.h>
#endif // PIPE_GNUPLOT

#ifdef DYNAMIC_LOAD
	#include <dlfcn.h>
	void* handle = NULL;
#endif // DYNAMIC_LOAD

struct dval { double x, y; };

typedef double (*func_t)(double);

double sin2(double x) {
	double y = sin(x);
	return y*y;
}

double cos2(double x) {
	double y = cos(x);
	return y*y;
}

double __sincos(double x) {
	// sin(x)cos(x) = sin(2x)/2
	return sin(2*x)/2;
}

double sin2x(double x) {
	return sin(2*x);
}

/// generates the table as asked in the task.
/// The core function for task 2
struct dval *get_table(func_t f, double start, double end, double step, long *length) {
	long tlength = lround((end-start)/step);
	struct dval *vals = calloc(tlength, sizeof(struct dval));
	for (long i = 0; i < tlength; ++i) {
		double x = start + step * i;
		// Not accumulative because if step is too small, 
		// start will never increment
		vals[i] = (struct dval){.x = x, .y = f(x)};
	}
	*length = tlength;
	return vals;
}

/// generates the table for derivative of function f.
/// The core function for task 3
struct dval *differentiate(func_t f, double start, double end, double step, long *length) {
	long tlength = lround((end-start)/step);
	struct dval *vals = calloc(tlength, sizeof(struct dval));
	for (long i = 0; i < tlength; ++i) {
		double x = start + step * i;
		double xi = start + step * (1+i);
		double y = (f(xi) - f(x))/step;
		vals[i] = (struct dval){.x = x, .y = y};
	}
	*length = tlength;
	return vals;
}

void print_help(int status) {
	const char *help = 
		"This program is Licensed under 'the unlicense license'\n\n"
		"Usage: ./t23 -f func_name -o output_file [-x number -h -d]\n"
		"\t-h: this help\n"
		"\t-d: differentiate function (do task 3)\n"
		"\t-f: function: either 'sin2', 'cos2', 'sincos', 'tg' or 'sin2x' .\n"
		"\t\tif compiled with -DDYNAMIC_LOAD, file name with dynamic function.\n"
		"\t-o: output file.\n"
		"\t-x: only needed with -d flag - differentiation step (the dx)\n\n"
		"NOTE: the function in dynamic library must be named 'f'.\n"
		"NOTE 2: with flag -DPIPE_GNUPLOT output will be piped in gnuplot\n";
	puts(help);
	exit(status);
}

/// Get the function based on the command line flags - either use 
/// existing or load one from dynamic library
func_t get_function(const char* name) {
	if (!strcmp(name, "cos2"))
		return &cos2;
	if (!strcmp(name, "sin2"))
		return &sin2;
	if (!strcmp(name, "sincos"))
		return &__sincos;
	if (!strcmp(name, "tg"))
		return &tan;
	if (!strcmp(name, "sin2x"))
		return &sin2x;
#ifndef DYNAMIC_LOAD
	fputs("ERROR: unknown function. To use dynamic loading, compile with `-DDYNAMIC_LOAD`.\n", stderr);
	exit(EXIT_FAILURE);
#else  // DYNAMIC_LOAD
	handle = dlopen(name, RTLD_NOW);
	if (!handle) goto fail;
	func_t f = dlsym(handle, "f");
	if (!f) goto fail;
	return f;
fail:
	fputs(dlerror(), stderr);
	exit(EXIT_FAILURE);
#endif // DYNAMIC_LOAD
}

void print_table(FILE* stream, const struct dval *table, long size) {
	for (long i = 0; i < size; ++i) {
		fprintf(stream, "%lg %lg\n", table[i].x, table[i].y);
	}
}

int main(int argc, char *argv[]) {
	func_t function = NULL;
	const char *out = NULL;
	char task2 = 1;
	float dx = 0.0;
	// casual argument loop
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], "-h"))
			print_help(EXIT_SUCCESS);
		else if (!strcmp(argv[i], "-f"))
			function = get_function(argv[++i]);
		else if (!strcmp(argv[i], "-o"))
			out = argv[++i];
		else if (!strcmp(argv[i], "-d"))
			task2 = 0;
		else if (!strcmp(argv[i], "-x"))
			dx = atof(argv[++i]);
		else print_help(EXIT_FAILURE);
	}
	// if function or output file is not set, or if dx is not set for task3,
	// then exit with failure
	if ((!function || !out) || (!task2 && dx == 0.0))
		print_help(EXIT_FAILURE);

	long table_len;
	struct dval *table = NULL;
	if (task2)
		table = get_table(function, 0, 3, 0.01, &table_len);
	else // task 3
		table = differentiate(function, -3, 3, dx, &table_len);

	// print data to stdout and to file
	print_table(stdout, table, table_len);
	FILE *f = fopen(out, "w+");
	if (f) print_table(f, table, table_len);
	else perror(out);

	// This section is creating a 'gnuplot' pipe 
	// and writing data to it to be plotted
#ifdef PIPE_GNUPLOT 
	FILE* gp = popen("gnuplot", "w");
	if (!gp) goto cleanup;
	fputs("plot '-' with lines\n", gp);
	print_table(gp, table, table_len);
	fputs("e\n", gp);
	fflush(gp);

	getchar();
	pclose(gp);
#endif // PIPE_GNUPLOT

cleanup:
	fclose(f);
	free(table);
#ifdef DYNAMIC_LOAD
	if (handle) dlclose(handle);
#endif // DYNAMIC_LOAD
	exit(EXIT_SUCCESS);
}

