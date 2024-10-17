#ifndef JCG_DRAWABLE_H
#define JCG_DRAWABLE_H

#define FILL_CHR '#'
#define SPACE_CHR ' '

#include <X11/Xlib.h>
#include <X11/Xutil.h>
extern struct X11context {
	enum {CTX_NO, CTX_INIT, CTX_FAIL} init;
	Display *d;
	Window r, w;
	int s;
	GC gctx;
	XVisualInfo v;
	XImage *i;
	int* data;
} X11;

// что угодно, на чём можно ''нарисовать''
// должно иметь размер, размер пикселей, данные
// функция put записывает пиксель по координатам x, y на поле
// функция show выводит данные на экран/куда угодно ещё
struct drawable {
	long x, y, pix_s;
	void *data;
	void (*put)(struct drawable *self, long x, long y);
	void(*show)(const struct drawable *self);
};


// конструктор
void drawable_init(struct drawable* self, 
		   void(*put_f)(struct drawable *, long, long), 
		   void(*show_f)(const struct drawable *), 
		   long width, 
		   long height, 
		   long pix_s);
// деструктор
void drawable_destroy(struct drawable *self);
// конструктор для консольного поля
struct drawable drawable_plaintxt(long width, long height);
// конструктор для графического поля
struct drawable drawable_X11(long width, long height);

// put для консольного drawable
void put_plaintxt(struct drawable *self, long x, long y);
// show для консольного drawable
void show_plaintxt(const struct drawable *self);
// put для графического drawable
void put_X11(struct drawable *self, long x, long y); 
// show для графического drawable
void show_X11(const struct drawable *self);

#endif // JCG_DRAWABLE_H
