#ifndef JUSTANOTHERCATGIRL_TASK4_ROOTS
#define JUSTANOTHERCATGIRL_TASK4_ROOTS

typedef double(*f_t)(double);

extern char root_ok;

char is_root_ok(void);
double sol_binsr(f_t f, double a, double b, double ex, double ey);
double sol_chord(f_t f, double a, double b, double ex, double ey);
double sol_newtn(f_t f, double x0, double ex, double ey);
double sol_itern(f_t f, double x0, double ex, double ey);


#endif // JUSTANOTHERCATGIRL_TASK4_ROOTS
