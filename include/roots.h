#ifndef JUSTANOTHERCATGIRL_TASK4_ROOTS
#define JUSTANOTHERCATGIRL_TASK4_ROOTS

typedef double(*func_t)(double);

extern char root_ok;

char is_root_ok(void);
double sol_binsr(func_t f, double a, double b, double ex, double ey);
double sol_chord(func_t f, double a, double b, double ex, double ey);
double sol_newtn(func_t f, double x0, double ex, double ey);
double sol_itern(func_t f, double x0, double ex, double ey);


#endif // JUSTANOTHERCATGIRL_TASK4_ROOTS
