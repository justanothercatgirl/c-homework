
#include "figure.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void (*const draw_lookup[])(const struct figure *, struct drawable *) = {
	[F_NONE] = NULL,
	[F_POINT] = draw_point,
	[F_RECT] = draw_rect,
	[F_LINE] = draw_line,
	[F_CIRCLE] = draw_circle
};

void swapl(long *x, long *y) {
	long tmp = *x;
	*x = *y;
	*y = tmp;
}

struct figure figure_point(vector2 p) {
	return (struct figure) {
		.type = F_POINT,
		.point.coords = p,
		.draw = draw_point
	};
}

struct figure figure_rect(vector2 upleft, vector2 downright) {
	return (struct figure) {
		.type = F_RECT,
		.rect = {upleft, downright},
		.draw = draw_rect
	};
}

struct figure figure_line(vector2 start, vector2 end) {
	return (struct figure) {
		.type = F_LINE,
		.line = {start, end},
		.draw = draw_line
	};
}

struct figure figure_circle(vector2 center, long radius) {
	return (struct figure) {
		.type = F_CIRCLE,
		.circle = {center, radius},
		.draw = draw_circle
	};
}


void draw_point(const struct figure *self, struct drawable *d) {
	d->put(d, self->point.coords.x, self->point.coords.y);
}

void draw_rect(const struct figure *self, struct drawable *d) {
	vector2 upleft = self->rect.upleft;
	vector2 downright = self->rect.downright;
	for (long i = upleft.x; i < downright.x + 1; ++i) {
		d->put(d, i, upleft.y);
		d->put(d, i, downright.y);
	} 
	for (long j = upleft.y + 1; j < downright.y; ++j) {
		d->put(d, upleft.x, j);
		d->put(d, downright.x, j);
	}
}

void draw_line(const struct figure *self, struct drawable *d) {
	vector2 start = self->line.start;
	vector2 end = self->line.end;
	char gt45 = labs(end.y - start.y) > labs(end.x - start.x);
	long x0 = start.x, y0 = start.y, 
	       x1 = end.x, y1 = end.y;
	if (gt45) {
		swapl(&x0, &y0);
		swapl(&x1, &y1);
	}
	if (x0 > x1) {
		swapl(&x0, &x1);
		swapl(&y0, &y1);
	}
	long dy = labs(y1-y0),
	     dx = x1 - x0,
	     err = dx >> 1, // *2
	     yinc = (y1 > y0 ? 1 : -1);

	for (long x = x0, y = y0; x <= x1; ++x) {
		if (gt45) d->put(d, y, x);
		else d->put(d, x, y);
		err -= dy;
		if (err < 0) {
			y += yinc;
			err += dx;
		}
	}
}

void draw_circle(const struct figure *self, struct drawable *f) {
	const vector2 p = self->circle.center;
	const long rad = self->circle.rad - 1;
	const long pi4 = ceil(M_SQRT1_2 * rad);
	long x0 = 0, y0 = rad;
	do {
		// выглядит коряво, зато в 8 раз меньше повторений цикла
		f->put(f, p.x-x0, p.y-y0); // 3 четверть
		f->put(f, p.x-x0, p.y+y0); // 2 чет.
		f->put(f, p.x+x0, p.y-y0); // 4 ч.
		f->put(f, p.x+x0, p.y+y0); // 1 ч
		f->put(f, p.x-y0, p.y-x0); 
		f->put(f, p.x-y0, p.y+x0);
		f->put(f, p.x+y0, p.y-x0);
		f->put(f, p.x+y0, p.y+x0);
		++x0;
		// вычисляем x^2 + (y+1/2)^2 - r^2
		// раскрыли скобки и умножили на 4, чтобы не иметь дело 
		// с дробными числами (они медленные)
		long func = 4*x0*x0 + 4*y0*y0 - 4*y0 + 1 - 4*rad*rad;
		y0 = (func >= 0 ? y0 - 1 : y0);
	} while (x0 < pi4);
}

void print_figure(const struct figure *self) {
	printf("figure {\n\t.type = %i\n\t{%li, %li},\n\t", self->type, self->line.start.x, self->line.start.y);
	switch (self->type) {
	case F_LINE:
	case F_RECT:
		printf("{%li, %li}\n\t", self->line.end.x, self->line.end.y);
		break;
	case F_CIRCLE:
		printf("%li\n\t", self->circle.rad);
		// fallthrough
	case F_NONE:
	case F_POINT:;		
	}
	printf(".draw = %p\n}\n", self->draw);
}
