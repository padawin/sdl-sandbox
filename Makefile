all: test1.c
	gcc -Wall -o "test1" test1.c `sdl-config --cflags --libs`
