# input overlay
When you want input overlay but can't be asked to use OBS but can be asked to use some random's bad code

## Config
Config can be found in `config.h`

`CBG`: Color used for background pixel of window (ARGB)  
`CFG`: Color used for the color of text (ARGB)  
`CBR`: Color used for the background of buttons (ARGB)  
`CBP`: Color used for the background of buttons when pressed (ARGB)  
`FONT`: The `Xft` font to use  
`FONTSIZE`: The size of the font  
`NAME`: The name of the window  
`ONTOP`: If should attempt to keep ontop (requires a wm which allows `XRaiseWindow`)  
`F4EXIT`: If should exit on `F4`  

### Binds
`binds` is a list of keys which are in the window, they are in the following format:  
`pressed`: This is an internal value, can be left at 0  
`x`: The x position of this key (starts from 0)  
`y`: The y position of this key (starts from 0)  
`key`: The `Xlib` keycode to use for this key  
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
