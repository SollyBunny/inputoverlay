NAME=inputoverlay
SOURCE=main.c
CFLAGS=-lX11 -lXft -I/usr/include/freetype2 -O4

build:
	cc $(SOURCE) -o $(NAME) $(CFLAGS)

install:
	rm -f "/usr/local/bin/$(NAME)"
	ln -s "`pwd`/$(NAME)" "/usr/local/bin/$(NAME)"

uninstall:
	rm -f "/usr/local/bin/$(NAME)"

