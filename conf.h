
#include "include.h"

// Color used for background pixel of window (ARGB)
#define CBG 0x00000000
// Color used for the color of text (ARGB)
#define CFG 0xFFFFFFFF
// Color used for the background of buttons (ARGB)
#define CBR 0xB2000000
// Color used for the background of buttons when pressed (ARGB)
#define CBP 0x59AAAAAA
// The Xft font to use
#define FONT "SourceCodePro-ExtraLight:pixelsize=30:antialias=false:autohint=true"
// The size of the font  
#define FONTSIZE 30
// The name of the window
#define NAME "Celeste Keys"
// If should attempt to keep ontop (requires a wm which allows `XRaiseWindow`)  
#define ONTOP
// If should exit on F4
// #define F4EXIT

Bind keys[] = {
    { 0, 1, 0, XK_w,     'W' },  // ↑ // up
    { 0, 0, 1, XK_a,     'A' },  // ← // left
    { 0, 1, 1, XK_s,     'S' },  // ↓ // down
    { 0, 2, 1, XK_d,     'D' },  // → // right
    { 0, 3, 0, XK_space, 'C' }, // jump
    { 0, 3, 1, XK_j ,    'X' }, // dash
    { 0, 4, 0, XK_k ,    'Z' }, // grab
    { 0, 4, 1, XK_l ,    'B' }  // demo
};

// all measurements are in px (pixels)
unsigned int keysize = 50;
unsigned int keypadding = 2; 
