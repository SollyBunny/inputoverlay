#ifndef INCLUDE_HEADER
#define INCLUDE_HEADER

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
	unsigned int pressed;
	unsigned int x;
	unsigned int y;
	unsigned int bind;
	char         disp;
} Bind;

#endif
