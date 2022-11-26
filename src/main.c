#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_surface.h>




int main(int *argc, char *argv[]){
    /* Initialize SDL - loads the Video, Event File I/O,
    and Threading subsystems */
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "ERROR: SDL init failed: %s", SDL_GetError());
        return -1;
    }

    /* Create main window */
    SDL_Window *mainWindow = SDL_CreateWindow("Simple Image Viewer", 0, 0, 100, 100, SDL_WINDOW_MAXIMIZED|SDL_WINDOW_RESIZABLE|SDL_WINDOW_INPUT_GRABBED);
    if(mainWindow == NULL){
        fprintf(stderr, "Error: Failed to create SDL window: %s", SDL_GetError());
        return -1;
    }

    /* Create renderer */
    SDL_Renderer *mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    if(mainRenderer == NULL){
        fprintf(stderr, "Error: Failed to create renderer: %s", SDL_GetError());
        return -1;
    }

    /* Variables to store the current width and height of window */
    int w, h;
    SDL_GetWindowSize(mainWindow, &w, &h);

    int shouldExit = 0;
    /* Main while loop*/
    while(shouldExit == 0){
        /* Update screen */
        SDL_RenderPresent(mainRenderer);
    }

    /* Quit SDL subsystems safely before returning */
    SDL_Quit();
    return 0;
}