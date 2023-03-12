# input overlay
When you want input overlay but can't be asked to use OBS but can be asked to use some random's bad code

## Config
Config can be found in `config.h`
`KEYSIZE`: Size of each key including padding (pixels)  
`KEYPAD`: Padding around each key (pixels)  
`POS`: Default pos (0, 1, 2, 3 clockwise from top left)  
`CBG`: Color used for background pixel of window (ARGB)  
`CFG`: Color used for the color of text (ARGB)  
`CBR`: Color used for the background of buttons (ARGB)  
`CBP`: Color used for the background of buttons when pressed (ARGB)  
`FONT`: The `Xft` font to use  
`NAME`: The name of the window  
`F4EXIT`: If should exit on `F4`  

### Binds
`binds` is a list of keys which are in the window, a macro is provided which has the following args:  
`x`: The x position of this key starting from 0 (units)  
`y`: The y position of this key starting from 0 (units)  
`w`: The width of this key (units)  
`h`: The height of this key (units)  
`key`: The `Xlib` keycode to use for this key (found in `/usr/include/X11/keysymdef.h`)  
`display`: A char placed on this key (can only be a char not a string)

## Compiling
Run `make`

## Installing
Run `make install` to install  
Run `make uninstall` to uninstall

## Usage
Run `inputoverlay`

## Problems
Window managers with background blur, and/or borders are not told to disable these features  
The display char can only be a char so multichar's (such as emojis or CJK) cannot be used
