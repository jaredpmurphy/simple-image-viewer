simple-image-viewer:
	gcc -o siv src/main.c `sdl2-config --cflags --libs`