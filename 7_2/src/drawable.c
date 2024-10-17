#include "drawable.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static struct {
	enum {CTX_NO, CTX_INIT, CTX_FAIL} init;
	Display *d;
	Window r, w;
	int s;
	GC gctx;
	XVisualInfo v;
	XImage *i;
	int* data;
} X11 = { .init=CTX_NO };

struct drawable drawable_plaintxt(long width, long height) {
	struct drawable ret;
	// with, height +1 because inclusive
	drawable_init(&ret, put_plaintxt, show_plaintxt, width + 1, height + 1, sizeof (char));
	memset(ret.data, 0, height*width*1);
	return ret;
}
struct drawable drawable_X11(long width, long height) {
	struct drawable ret;
	drawable_init(&ret, put_X11, show_X11, width, height, sizeof (int));
	int* data = ret.data;
	for (size_t i = 0; i < width*height; ++i)
		data[i] = 0x00FFFFFF;
	return ret;
}

void drawable_init(struct drawable* self, 
		   void(*put_f)(struct drawable *, long, long), 
		   void(*show_f)(const struct drawable *), 
		   long width, 
		   long height, 
		   long pix_s)
{
	self->x = width;
	self->y = height;
	self->pix_s = pix_s;
	self->data = malloc(height*width*pix_s);
	self->put = put_f;
	self->show = show_f;
}

void drawable_destroy(struct drawable *self) {
	free(self->data);
}

/// plaintxt methods ///

void put_plaintxt(struct drawable *self, long x, long y) {
	if (x >= self->x || y >= self->y || x < 0 || y < 0) return;
	char *data = self->data;
	data[self->x * y + x] = 1;
}

void show_plaintxt(const struct drawable *self) {
	char map[2] = {SPACE_CHR, FILL_CHR};
	for (int i = 0; i < self->y; ++i) {
		for (int j = 0; j < self->x; ++j) {
			char *d = self->data;
			putchar(map[d[i*self->x + j]]);
		}
		putchar('\n');
	}
}


/// X11 from now on ///

static void init_X11(const struct drawable *self) {
	X11.d = XOpenDisplay(NULL);
	if (X11.d == NULL) {
		X11.init = CTX_FAIL;
		return;
	}
	X11.s = DefaultScreen(X11.d);
	X11.r = RootWindow(X11.d, X11.s);
	X11.gctx = DefaultGC(X11.d, X11.s);
	Window w = XCreateSimpleWindow(X11.d, X11.r, 0, 0, self->x, self->y, 0, 0, 0x00FFFFFF);
	if (!XMatchVisualInfo(X11.d, X11.s, 24, TrueColor, &X11.v)) {
		XDestroyWindow(X11.d, X11.w);
		XCloseDisplay(X11.d);
		X11.init = CTX_FAIL;
		return;
	};
	XMapWindow(X11.d, X11.w);
	XSelectInput(X11.d, X11.w, ExposureMask);
	X11.data = malloc(self->x * self->y * sizeof (int));
	X11.i = XCreateImage(X11.d, X11.v.visual, X11.v.depth, ZPixmap, 0, (void*)X11.data, self->x, self->y, 8*sizeof (int), 0);
}

void put_X11(struct drawable *self, long x, long y) {
	if (x >= self->x || y >= self->y || x < 0 || y < 0) return;
	int *data = self->data;
	data[self->x * y + x] = 0x00000000;
}

void show_X11(const struct drawable *self) {
start:
	switch (X11.init) {
	case CTX_NO: init_X11(self); goto start;
	case CTX_INIT: break;
	case CTX_FAIL: 
		fprintf(stderr, "Error: Could not initialize Xlib\n");
		return;
	}
	XEvent e = {};
	XNextEvent(X11.d, &e);
	XPutImage(X11.d, X11.w, X11.gctx, X11.i, 0, 0, 0, 0, self->x, self->y);
}

