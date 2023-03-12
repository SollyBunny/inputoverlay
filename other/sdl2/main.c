#pragma GCC diagnostic ignored "-Wmultichar"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glx.h>

#include "include.h"
#include "conf.h"

SDL_Window   *m_window;
SDL_Renderer *m_window_renderer;
SDL_Surface  *m_key_surface[sizeof(keys) / sizeof(keys[0])];
SDL_Texture  *m_key_texture[sizeof(keys) / sizeof(keys[0])];
TTF_Font     *m_font;
//Window        m_x_window;

Display      *disp;
Window        root;
Window        focs;
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
	//printf("Change focus!\n");
	if (focs != root) XSelectInput(disp, focs, 0);
	XGetInputFocus(disp, &focs, &revt);
	if (focs == PointerRoot) focs = root;
	XSelectInput(disp, focs, KeyPressMask|KeyReleaseMask|FocusChangeMask);
}

int main(void) {

	// Init X11
		XSetErrorHandler(xerrorhandle);
		disp = XOpenDisplay(NULL);
		root = DefaultRootWindow(disp);
		XGetInputFocus(disp, &focs, &revt);
		XSelectInput(disp, focs, KeyPressMask|KeyReleaseMask|FocusChangeMask);
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
		
	// Init SDL
		if(SDL_Init(SDL_INIT_VIDEO) < 0) die("Failed to initialize the SDL2 library");
		/*// thank you https://forums.libsdl.org/viewtopic.php?p=44939
		XVisualInfo visualinfo;
		XMatchVisualInfo(disp, XDefaultScreen(disp), 32, TrueColor, &visualinfo);
		//GC gc;
		XSetWindowAttributes attr;
	    attr.event_mask = ExposureMask | KeyPressMask; // TODO
	    //attr.background_pixmap = None;
	    // thanks suckless terminal
	    attr.border_pixel = 0;
	    attr.background_pixel = 0x00FFFFFF | ((unsigned char)(0xff * 0.7) << 24);
	    attr.colormap   = XCreateColormap(disp, root, visualinfo.visual, None);
	    attr.bit_gravity = NorthWestGravity;
	    m_x_window = XCreateWindow(disp, root,
			0, 0, 500, 500, // TODO w/h
			0,
			visualinfo.depth,
			InputOutput,
			visualinfo.visual,
			CWBackPixel | CWBorderPixel | CWBitGravity | CWEventMask | CWColormap,
			&attr
		);
		GC gc = XCreateGC(disp, m_x_window, 0, 0);

		XMapWindow(disp, m_x_window);
		XSync(disp, False);
		m_window = SDL_CreateWindowFrom((void*)m_x_window);*/
		if (!(m_window = SDL_CreateWindow(
			"Keys", 
			0, 0, maxx, maxy,
			SDL_WINDOW_BORDERLESS
		))) die("Failed to create window");
		SDL_SetWindowResizable(m_window, SDL_FALSE);
		SDL_SetWindowOpacity  (m_window, 0.6);

		if (!(m_window_renderer = SDL_CreateRenderer(
			m_window, -1, 
			SDL_RENDERER_ACCELERATED && SDL_HINT_RENDER_VSYNC && SDL_HINT_RENDER_SCALE_QUALITY
		))) die("Failed to create renderer");
		SDL_Rect m_key_rect;
		m_key_rect.w = keysize;
		m_key_rect.h = keysize;

		// Init Font
			TTF_Init();
			if (!(m_font = TTF_OpenFont(
				FONT, keysize * 8
			))) die("Failed to open font \"" FONT "\"");

	// Init keys

		// create a surface for each key to display
		SDL_Color color_white = {255, 255, 255, 255};
		char *text = malloc(4 * sizeof(unsigned int));
		text[0] = ' ';
		text[2] = ' ';
		text[3] = '\0';
		for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
			text[1] = keys[i].disp;
			m_key_surface[i] = TTF_RenderText_Solid(m_font, text, color_white);
			m_key_texture[i] = SDL_CreateTextureFromSurface(m_window_renderer, m_key_surface[i]);
		}


	while (1) {

		XEvent e;
		XNextEvent(disp, &e);
		switch (e.type) {
			case FocusOut:
				changefocus();
				break;
			case KeyPress:
			case KeyRelease:
				for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
					if (keys[i].bind == e.xkey.keycode) {
						//printf("Got key %d!\n", e.xkey.keycode);
						keys[i].pressed = e.type;
						break;
					}
				}
				break;
		}

		SDL_Event m_e;
		while (SDL_PollEvent(&m_e) > 0) {
			if (m_e.type == SDL_QUIT) goto end;
			/*switch (m_e.type) {
				case SDL_QUIT:
					goto end;
			}*/
		}

		//printf("Event loop!\n");

		SDL_SetRenderDrawColor(m_window_renderer, 30, 30, 30, 255);
		SDL_RenderClear(m_window_renderer);

		for (unsigned int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
			m_key_rect.x = keys[i].x;
			m_key_rect.y = keys[i].y;
			SDL_SetRenderDrawColor(m_window_renderer, 100, 100, 100, 100);
			if (keys[i].pressed == KeyPress) {
				SDL_RenderFillRect(m_window_renderer, &m_key_rect);
				SDL_SetRenderDrawColor(m_window_renderer, 128, 128, 128, 255);
			}
			SDL_RenderDrawRect(m_window_renderer, &m_key_rect);
			m_key_rect.y -= keysize * 0.2;
			m_key_rect.h += keysize * 0.4;
			SDL_RenderCopy(m_window_renderer, m_key_texture[i], NULL, &m_key_rect);
			m_key_rect.y += keysize * 0.2;
			m_key_rect.h -= keysize * 0.4;
		}

		SDL_RenderPresent(m_window_renderer);
	}

	end:
	return 0;
}
