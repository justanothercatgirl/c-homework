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

struct dval {
	double x, y;
};

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

struct dval* get_table(func_t f, double start, double end, double step, long *length) {
	long tlength = lround((end-start)/step);
	struct dval* vals = calloc(tlength, sizeof(struct dval));
	for (long i = 0; i < tlength; ++i) {
		double x = start + step * i;
		// Not accumulative because if step is too small, 
		// start will never increment
		vals[i] = (struct dval){.x = x, .y = f(x)};
	}
	*length = tlength;
	return vals;
}

void print_help(int status) {
	const char *help = 
		"Usage: ./task2 -f func_name -o output_file [-h]\n"
		"\t-h: this help\n"
		"\t-f: function: either 'sin2', 'cos2' or 'sincos'.\n"
		"\t\tif compiled with -DDYNAMIC_LOAD, file name with dynamic function.\n"
		"\t-o: output file.\n\n"
		"NOTE: the function in dynamic library must be named 'f'.\n"
		"NOTE 2: with flag -DPIPE_GNUPLOT output will be piped in gnuplot\n.";
	puts(help);
	exit(status);
}

func_t get_function(const char* name) {
	if (!strcmp(name, "cos2"))
		return &cos2;
	if (!strcmp(name, "sin2"))
		return &sin2;
	if (!strcmp(name, "sincos"))
		return &__sincos;
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
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], "-h"))
			print_help(EXIT_SUCCESS);
		else if (!strcmp(argv[i], "-f"))
			function = get_function(argv[++i]);
		else if (!strcmp(argv[i], "-o"))
			out = argv[++i];
		else print_help(EXIT_FAILURE);
	}
	if (!function || !out) print_help(EXIT_SUCCESS);
	long table_len;
	struct dval *table = get_table(function, 0, 3, 0.01, &table_len);
	print_table(stdout, table, table_len);
	FILE *f = fopen(out, "w+");
	if (f) { 
		print_table(f, table, table_len);
		fflush(f);
	}
	else perror(out);

#ifdef PIPE_GNUPLOT 
	FILE* gp = popen("gnuplot", "w");
	if (!gp) goto exit;
	fputs("plot '-' with lines\n", gp);
	print_table(gp, table, table_len);
	fputs("e\n", gp);
	fflush(gp);

	getchar();
	pclose(gp);
#endif // PIPE_GNUPLOT

exit:
	fclose(f);
	free(table);
#ifdef DYNAMIC_LOAD
	if (handle) dlclose(handle);
#endif // DYNAMIC_LOAD
	exit(EXIT_SUCCESS);
}

