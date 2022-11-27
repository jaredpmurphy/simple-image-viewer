simple-image-viewer:
	gcc -o siv src/main.c src/viewer.c `sdl2-config --cflags --libs` -lSDL2_image