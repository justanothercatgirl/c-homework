#ifndef JUSTANOTHERCATGIRL_TASK4_INTEGRAL
#define JUSTANOTHERCATGIRL_TASK4_INTEGRAL

typedef double(*func_t)(double);

extern double accrc;

double int_rect(func_t f, double a, double b);
double int_trap(func_t f, double a, double b);
double int_simp(func_t f, double a, double b);

#endif //JUSTANOTHERCATGIRL_TASK4_INTEGRAL 
