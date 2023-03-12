
#include <stdio.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Xft/Xft.h>

typedef struct {
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
	unsigned int bind;
	char        *disp;
	unsigned int dispsize;
	unsigned int dispw;
	unsigned int disph;
	unsigned int pressed;
} Bind;

#include "config.h"

Display      *disp;
int           scre;
Window        root;
Window        wind;
Window        focs = 0;
int           revt;

unsigned int maxx = 0;
unsigned int maxy = 0;

void die(const char msg[]);
int xerrorhandle(Display *_, XErrorEvent *e);
void changefocus();

void die(const char msg[]) {
	printf("Error: %s\n", msg);
	exit(1);
}

int xerrorhandle(Display *_, XErrorEvent *e) {
	(void)_;
	if (e->error_code == BadWindow) // the focused window has been deleted
		changefocus();
	else
		printf("XError: %uc\n", e->error_code);
	return 0;
}

void changefocus() {
	if (focs != 0 && focs != root) XSelectInput(disp, focs, 0);
	XGetInputFocus(disp, &focs, &revt);
	if (focs == PointerRoot) focs = root;
	XSelectInput(disp, focs, KeyPressMask | KeyReleaseMask | FocusChangeMask);
	#ifdef ONTOP
		XRaiseWindow(disp, wind);
	#endif
}

int main(void) {

	// Init X11
		XSetErrorHandler(xerrorhandle);
		disp = XOpenDisplay(NULL);
		root = XDefaultRootWindow(disp);
		scre = XDefaultScreen(disp);
		XkbSetDetectableAutoRepeat(disp, True, NULL);
	// Setup visual / attrs
		XVisualInfo visual;
		XMatchVisualInfo(disp, scre, 32, TrueColor, &visual);
		XSetWindowAttributes attr;
		attr.border_pixel     = 0;
		attr.background_pixel = CBG;
	    attr.colormap         = XCreateColormap(disp, root, visual.visual, None);
	    attr.bit_gravity      = NorthWestGravity;
		attr.event_mask       = ExposureMask;
	// Setup font
		XftFont *xftfont = XftFontOpenName(disp, scre, FONT);
		if (!xftfont) die("Failed to open font \"" FONT "\"");
	// Setup keys
		for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
			// Convert keysym to keycode
				keys[i].bind = XKeysymToKeycode(disp, keys[i].bind);
			// Calculate true x, y, w, h values
				keys[i].x = keys[i].x + KEYPAD;
				keys[i].y = keys[i].y + KEYPAD;
				if (keys[i].x + keys[i].w > maxx) maxx = keys[i].x + keys[i].w; // expand window if neccesary
				if (keys[i].y + keys[i].h > maxy) maxy = keys[i].y + keys[i].h;
				keys[i].w -= KEYPAD;
				keys[i].h -= KEYPAD;
			// Calculate string size of disp
				static char *p;
				for (p = keys[i].disp; *p != '\0'; ++p)
					keys[i].dispsize += 1;
			// Calculate visual size of disp
				static XGlyphInfo extents;
				XftTextExtentsUtf8(disp, xftfont, (FcChar8*)keys[i].disp, keys[i].dispsize, &extents);
				keys[i].dispw = extents.width / 2;
				keys[i].disph = extents.height / 2;
		}
	// Create window
		#if POS == 0
			X = 0
			Y = 0
		#elif POS == 1
			X = XDisplayWidth(disp, scre) - maxx;
			Y = 0
		#elif POS == 2
			X = XDisplayWidth(disp, scre) - maxx;
			Y = XDisplayHeight(disp, scre) - maxy;
		#elif POS == 3
			X = 0;
			Y = XDisplayHeight(disp, scre) - maxy;
		#else
			#error "Invalid POS value"
		#endif
		wind = XCreateWindow(
			disp, root,
			0, 0, maxx, maxy,
			0,
			visual.depth,
			InputOutput,
			visual.visual,
			CWBackPixel | CWBorderPixel | CWBitGravity | CWEventMask | CWColormap,
			&attr
		);
		XStoreName(disp, wind, NAME);
		XMapWindow(disp, wind);
		XSync(disp, False);
		XSelectInput(disp, wind, KeyPressMask | KeyReleaseMask | FocusChangeMask | ExposureMask);
	// Setup GC
		GC gc = XCreateGC(disp, wind, 0, 0);
		XftDraw *xftgc = XftDrawCreate(disp, wind, visual.visual, attr.colormap);
		if (!xftgc) die("Failed to create font GC\n");
		XftColor xftcol;
		xftcol.pixel = CFG;
		xftcol.color.red   = (CFG & 255) << 8;
		xftcol.color.green = ((CFG >> 8) & 255) << 8;
		xftcol.color.blue  = ((CFG >> 16) & 255) << 8;
		xftcol.color.alpha = ((CFG >> 24) & 255) << 8;

	XSync(disp, False);
	static XEvent e;
	while (!XNextEvent(disp, &e)) {
		switch (e.type) {
			case DestroyNotify:
				goto end;
			case FocusOut:
			case ButtonPress:
				changefocus();
				break;
			case KeyPress:
				#ifdef F4EXIT
					if (e.xkey.keycode == XKeysymToKeycode(disp, XK_F4)) goto end;
				#endif
			case KeyRelease:
				static unsigned int i;
				for (i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
					if (keys[i].bind == e.xkey.keycode) {
						if (keys[i].pressed == e.type) break; // don't render if key is already (un)pressed
						// printf("Got key %d %d!\n", e.type, e.xkey.keycode);
						keys[i].pressed = e.type;
						goto l_expose;
					}
				}
				break;
			case Expose: l_expose:
				XClearWindow(disp, wind);
				for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
					XSetForeground(disp, gc, keys[i].pressed == KeyPress ? CBP : CBR);
					XFillRectangle(
						disp, wind, gc, 
						keys[i].x, keys[i].y,
						keys[i].w, keys[i].h
					);
					XftDrawStringUtf8(
						xftgc, &xftcol, xftfont, 
						keys[i].x + keys[i].w / 2 - keys[i].dispw, keys[i].y + keys[i].h - keys[i].disph,
						(FcChar8*)keys[i].disp, keys[i].dispsize
					);
				}
				break;
		}

	}

	end: // free stuff

		XUnmapWindow(disp, wind);

		XftFontClose(disp, xftfont);
		XftDrawDestroy(xftgc);
		
		XFreeGC(disp, gc);
		XDestroyWindow(disp, wind);
		XCloseDisplay(disp);
	
	return 0;
}




