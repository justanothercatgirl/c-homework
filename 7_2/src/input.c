#include "input.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *const unexpchtok = "Unexpected token: %c\n";
const char *const invalidint = "Invalid integer: %s\n";

#define error(fmt, ...) do {		\
	fprintf(stderr, fmt, __VA_ARGS__);	\
	return 0;			\
} while(0)	

#define maybe_realloc(size, cap, pointer, type) do {		\
	if (size >= cap) {					\
		cap <<= 1;					\
		pointer = realloc(pointer, cap * sizeof (type));\
	}							\
} while(0)

void(*settings_handler)(const char*) = NULL;

void skipw(char const **s) { while(isspace(**s)) ++*s; }
void skipd(char const **s) { while(isdigit(**s)) ++*s; }

char chktok(char const **str, char tok) {
	skipw(str);
	long tmp = atol(*str);
	if (**str != tok) error(unexpchtok, **str);
	++*str;
	return 1;
}

char parsel(char const **str, long *l) {
	skipw(str);
	long tmp = atoi(*str);
	if (tmp == 0 && **str != '0') error(invalidint, *str);
	*l = tmp;
	skipd(str);
	return 1;
}

char parsev(char const **str, vector2 *v) {
	long x, y;
	// я ща кончу на этот код
	char suc = chktok(str, '{')
		&& parsel(str, &x)
		&& chktok(str, ',')
		&& parsel(str, &y)
		&& chktok(str, '}');
	if (suc) {
		v->x = x;
		v->y = y;
		return 1;
	} else	return 0;
}

struct figure read_figure(const char *str) {
	struct figure ret = {.type = F_NONE};
	const char *strp = str;
	skipw(&strp);
	++strp;
	switch(strp[-1]) {
	case '.': {
		if (settings_handler != NULL) settings_handler(strp);
	} break;
	case 'p': {
		if (parsev(&strp, &ret.point.coords) 
		    && chktok(&strp, ';')) ret.type = F_POINT;
	} break;
	case 'r': {
		if (parsev(&strp, &ret.rect.upleft)
		    && parsev(&strp, &ret.rect.downright)
		    && chktok(&strp, ';')) ret.type = F_RECT;
	} break;
	case 'l': {
		if (parsev(&strp, &ret.line.start)
		    && parsev(&strp, &ret.line.end)
		    && chktok(&strp, ';')) ret.type = F_LINE;
	} break;
	case 'c': {
		if (parsev(&strp, &ret.circle.center)
		    && parsel(&strp, &ret.circle.rad)
		    && chktok(&strp, ';')) ret.type = F_CIRCLE;
	} break;
	default:;
	}
	ret.draw = draw_lookup[ret.type];
	return ret;
}

struct figure *read_file(const char *path) {
	FILE *f = fopen(path, "r");
	if (!f) return NULL;
	size_t bs = 1024;
	size_t fs = -1, fcap = 16;
	char *buf = malloc(fcap * sizeof(struct figure));
	struct figure *figures = malloc(fcap * sizeof (struct figure));
	while (!feof(f)) {
		if (getdelim(&buf, &bs, ';', f) < 0) break;
		struct figure f = read_figure(buf);
		if (f.type == F_NONE) continue;
		++fs;
		maybe_realloc(fs, fcap, figures, struct figure);
		memcpy(figures + fs, &f, sizeof (struct figure));
	}
	++fs;
	maybe_realloc(fs, fcap, figures, struct figure);
	struct figure END = {.type = F_NONE, 0};
	memcpy(figures + fs, &END, sizeof (struct figure));
	free(buf);
	return figures;
}

