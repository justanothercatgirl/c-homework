#ifndef JAC_INPUT_H
#define JAC_INPUT_H

#include "common.h"
#include "figure.h"

extern void(*settings_handler)(const char *sett_string);

// пропустить пробелы 
void skipw(char const **s);
// проспустит цифры
void skipd(char const **s);
// проерить наличие символа
char chktok(char const **str, char tok);
// прочитать число
char parsel(char const **str, long *l);
// прочитать вектор из 2 чисел
char parsev(char const **str, vector2 *v);
// прочитать одну фигуру
struct figure read_figure(const char *str);
// прочитать целый файл
struct figure *read_file(const char *path);

#endif // JAC_INPUT_H
