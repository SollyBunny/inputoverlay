
#include "include.h"

#define FONT "/usr/share/fonts/noto/NotoSans-Light.ttf"

Bind keys[] = {
    { 0, 1, 0, XK_w,     'w' },  // ↑ // up
    { 0, 0, 1, XK_a,     'a' },  // ← // left
    { 0, 1, 1, XK_s,     's' },  // ↓ // down
    { 0, 2, 1, XK_d,     'd' },  // → // right
    { 0, 3, 0, XK_space, 'c' }, // jump
    { 0, 3, 1, XK_j ,    'x' }, // dash
    { 0, 4, 0, XK_k ,    'z' }, // grab
    { 0, 4, 1, XK_l ,    'b' }  // demo
};

// all measurements are in px (pixels)
unsigned int keysize = 50;
unsigned int keypadding = 2; 
