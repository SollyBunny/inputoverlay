
// Size of each key including padding (pixels)
#define KEYSIZE 50
// Padding around each key (pixels)
#define KEYPAD 5
// Color used for background pixel of window (ARGB)
#define CBG 0x00000000
// Color used for the color of text (ARGB)
#define CFG 0xFFFFFFFF
// Color used for the background of buttons (ARGB)
#define CBR 0xB2220022
// Color used for the background of buttons when pressed (ARGB)
#define CBP 0x59AAAAAA
// The Xft font to use
#define FONT "SourceCodePro-ExtraLight:pixelsize=30:antialias=false:autohint=true"
// The name of the window
#define NAME "Celeste Keys"
// If should attempt to keep ontop (requires a wm which allows `XRaiseWindow`)  
#define ONTOP
// If should exit on F4
// #define F4EXIT

#define BIND(x, y, w, h, key, disp) { (unsigned int)(x * KEYSIZE), (unsigned int)(y * KEYSIZE), (unsigned int)(w * KEYSIZE), (unsigned int)(h * KEYSIZE), key, disp, 0, 0, 0, 0 }
 
Bind keys[] = {
//       x, y, w, h, bind,     disp
    BIND(1, 0, 1, 1, XK_w,     "↑"), // up
    BIND(0, 1, 1, 1, XK_a,     "←"), // left
    BIND(1, 1, 1, 1, XK_s,     "↓"), // down
    BIND(2, 1, 1, 1, XK_d,     "→"), // right
    BIND(3, 0, 1, 1, XK_space, "C"), // jump
    BIND(3, 1, 1, 1, XK_j,     "X"), // dash
    BIND(4, 0, 1, 1, XK_k,     "Z"), // grab
    BIND(4, 1, 1, 1, XK_l,     "B"), // demo
};

