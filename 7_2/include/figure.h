#ifndef JCG_FIGURE_H
#define JCG_FIGURE_H

#include "drawable.h"
#include "common.h"

// любая фигура
// создаётся функциями figure_point, figure_line и т.д.
// у каждой фигуры должен быть способ "нарисоваться" на 
// любое "рисовальное" поле, используя функцию drawable.put
struct figure {
	enum { 
		F_NONE, F_POINT, F_RECT, F_LINE, F_CIRCLE
	} type;
	union {
		struct {
			vector2 coords;
		} point;
		struct {
			vector2 upleft, downright;
		} rect;
		struct { 
			vector2 start, end;
		} line;
		struct {
			vector2 center;
			long rad;
		} circle;
	};
	void (*draw)(const struct figure *self, struct drawable *d);
};

// массив функций по типам (lookup table)
extern void (*const draw_lookup[])(const struct figure *, struct drawable *);

// конструктор 1
struct figure figure_point(vector2 p);
// конструктор 2
struct figure figure_rect(vector2 upleft, vector2 downright);
// конструктор 3
struct figure figure_line(vector2 start, vector2 end);
// конструктор 4
struct figure figure_circle(vector2 center, long radius);

// draw для F_POINT
void draw_point(const struct figure *self, struct drawable *d);
// draw для F_RECT
void draw_rect(const struct figure *self, struct drawable *d);
// draw для F_LINE
void draw_line(const struct figure *self, struct drawable *d);
// draw для F_CIRCLE
void draw_circle(const struct figure *self, struct drawable *f);

void print_figure(const struct figure *self);

#endif // JCG_FIGURE_H
