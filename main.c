
#include <stdio.h>
#include <stdlib.h>

#include "include.h"
#include "conf.h"

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

		// convert keysym into arbritrary keyboard keys
		// convert arbritrary x and y values into actuall pixel coordinates
			for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
				keys[i].bind = XKeysymToKeycode(disp, keys[i].bind);
				keys[i].x = (keypadding * (keys[i].x + 1)) + (keys[i].x * keysize);
				keys[i].y = (keypadding * (keys[i].y + 1)) + (keys[i].y * keysize);
				if (keys[i].x > maxx) maxx = keys[i].x;
				if (keys[i].y > maxy) maxy = keys[i].y;
			}
			maxx += keypadding + keysize;
			maxy += keypadding + keysize;
		// Create window
			XVisualInfo visual;
			XMatchVisualInfo(disp, scre, 32, TrueColor, &visual);
			XSetWindowAttributes attr;
			attr.border_pixel     = 0;
			attr.background_pixel = CBG;
		    attr.colormap         = XCreateColormap(disp, root, visual.visual, None);
		    attr.bit_gravity      = NorthWestGravity;
			attr.event_mask       = ExposureMask;
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
			// Create drawing context + set fg/bg colors
			GC gc = XCreateGC(disp, wind, 0, 0);
			XMapWindow(disp, wind);
			XSync(disp, False);
			changefocus();
		
	XftFont *xftfont = XftFontOpenName(disp, scre, FONT);
	if (!xftfont) die("Failed to open font \"" FONT "\"");
	XftDraw *xftgc = XftDrawCreate(disp, wind, visual.visual, attr.colormap);
	if (!xftgc) die("cannot allocate xft color\n");
	XftColor xftcol;
	xftcol.pixel = CFG;
	xftcol.color.red   = (CFG & 255) << 8;
	xftcol.color.green = ((CFG >> 8) & 255) << 8;
	xftcol.color.blue  = ((CFG >> 16) & 255) << 8;
	xftcol.color.alpha = ((CFG >> 24) & 255) << 8;

	FcChar8 *xfttext = malloc(2 * sizeof(FcChar8));
	xfttext[1] = '\0';

	XEvent e;
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
				unsigned int flag = 0;
				for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
					if (keys[i].bind == e.xkey.keycode) {
						// printf("Got key %d!\n", e.xkey.keycode);
						keys[i].pressed = e.type;
						flag = 1;
						break;
					}
				}
				if (flag == 0) break; // don't render if nothing changed
			case Expose:
				XClearWindow(disp, wind);
				for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
					XSetForeground(disp, gc, keys[i].pressed == KeyPress ? CBP : CBR);
					XFillRectangle(
						disp, wind, gc, 
						keys[i].x, keys[i].y,
						keysize, keysize
					);
					xfttext[0] = keys[i].disp;
					XftDrawStringUtf8(
						xftgc, 
						&xftcol, xftfont, 
						keys[i].x + (FONTSIZE / 2), keys[i].y + keysize - (FONTSIZE / 2),
						xfttext, 1//sizeof(xfttext) / sizeof(xfttext[0])
					);
				}
				break;
		
		}

	}

	end: // free stuff

		free(xfttext);
		XftFontClose(disp, xftfont);
		XftDrawDestroy(xftgc);

		XDestroyWindow(disp, wind);
		XCloseDisplay(disp);
	
	return 0;
}




