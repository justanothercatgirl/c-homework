#ifndef JUSTANOTHERCATGIRL_TASK4_INTEGRAL
#define JUSTANOTHERCATGIRL_TASK4_INTEGRAL

typedef double(*f_t)(double);

extern double accrc;

double int_rect(f_t f, double a, double b);
double int_trap(f_t f, double a, double b);
double int_simp(f_t f, double a, double b);

#endif //JUSTANOTHERCATGIRL_TASK4_INTEGRAL 
