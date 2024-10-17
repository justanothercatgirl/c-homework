#include <X11/Xlib.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "drawable.h"
#include "figure.h"

long fatness = 1; // пока никак не используется

int sx = 100, sy = 100; 

int main(int argc, char *argv[]) {
	char file[256] = {};
	if (argc >= 2) {
		size_t len = strlen(argv[1]);
		memcpy(file, argv[1], len > 255 ? 255 : len);
	} else {
		printf("введите файл: ");
		scanf("%255s", file);
	}
	char x11 = 0;
	for (int i = 0; i < argc; ++i) {
		if (!strcmp("-X", argv[i])) x11 = 1;
	}
	struct figure *fs = read_file(file);
	struct drawable d = x11 ? drawable_X11(sx, sy) : drawable_plaintxt(sx, sy);
	if (!fs) {
		fprintf(stderr, "ERROR: errno %i: %s\n", errno, strerror(errno));
		return 1;
	}
	for (struct figure *i = fs; i->type != F_NONE; ++i)
		i->draw(i, &d);

	d.show(&d);
	free(fs);
	drawable_destroy(&d);
	return 0;
}

