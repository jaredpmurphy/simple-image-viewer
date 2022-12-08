#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_image.h>
#include <SDL_events.h>
#include "viewer.h"

static char *imgPath;

int processArgs(int argCA, char *argVA[]){
    if (argCA != 2){
        fprintf(stderr, "Error: Invalid arguments. Enter only one file path.\n");
        return -1;
    }
    /* Temp string for manipualtion */
    char *tempPath;
    /* Length of path entered by user */
    int pathLen = strlen(argVA[1]);
    /* Length of root path to dir where program was called */
    int absLen;

    /* Check if it is valid. at minimum it would be x.ext */
    if(pathLen < 5){
        fprintf(stderr, "Error: Invalid Arguments. Path too short.\n");
        return -1;
    }
    /* Allocate memory for tempPath so we don't segfault */
    tempPath = malloc(sizeof(char) * pathLen+1);
    /* Clean path so imgPath will end up as absolute path to image */
    if(argVA[1][0] == '/'){
        if((imgPath = malloc(sizeof(char)*(pathLen+1))) == NULL){
            fprintf(stderr, "Error: Failed to allocate memory for filepath.\n");
            return -1;
        }
        strcpy(imgPath, argVA[1]);
    }else{
        absLen = strlen(SDL_GetBasePath());
        if((imgPath = malloc(sizeof(char)*(pathLen+absLen+1))) == NULL){
            fprintf(stderr, "Error: Failed to allocate memory for filepath.\n");
            return -1;
        }
        strcpy(imgPath, SDL_GetBasePath());

        if(argVA[1][0] == '.' && argVA[1][1] == '/'){
            for(int i = 2; i < pathLen+1; ++i){
                tempPath[i-2] = argVA[1][i];
            }
            strcat(imgPath, tempPath);
        }else{
            strcat(imgPath, argVA[1]);
        }
    }

    /* final check for sanity */
    if(imgPath == NULL){
        fprintf(stderr, "Error: Something went very wrong. Check provided file path and try again.\n");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]){
    atexit(SDL_Quit);
    /* Initialize SDL - loads the Video, Event File I/O,
    and Threading subsystems */
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "ERROR: SDL init failed: %s\n", SDL_GetError());
        return -1;
    }

    /* Init the image subsystem separately */
    if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF|IMG_INIT_WEBP|IMG_INIT_JXL|IMG_INIT_AVIF) < 0){
        fprintf(stderr, "Error: Failed to initialize IMG sublibrary: %s\n", SDL_GetError());
        return -1;
    }

    /* Create main window */
    SDL_Window *mainWindow = SDL_CreateWindow("Simple Image Viewer", 0, 0, 400, 200, SDL_WINDOW_RESIZABLE);
    if(mainWindow == NULL){
        fprintf(stderr, "Error: Failed to create SDL window: %s\n", SDL_GetError());
        return -1;
    }

    /* Create renderer */
    SDL_Renderer *mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    if(mainRenderer == NULL){
        fprintf(stderr, "Error: Failed to create renderer: %s\n", SDL_GetError());
        return -1;
    }

    /* Process and sanitize user arguments */
    if(processArgs(argc, argv) < 0){
        return -1;
    }

    /* Initialize viewer stuff*/
    if(initViewer(mainRenderer, imgPath) < 0){
        fprintf(stderr, "Error: Failed to initialize viewer.\n");
        return -1;
    }

    int shouldExit = 0;
    double lastTime, deltaTime;
    deltaTime = 0.000016;
    lastTime = SDL_GetTicks();
    /* Main while loop*/
    SDL_Event mEvent;
    while(shouldExit == 0){
        /* Clear the renderer */
        SDL_RenderClear(mainRenderer);

        while(SDL_PollEvent(&mEvent) == 1){
            /* check for close and maybe other stuff */
            if(mEvent.type == SDL_WINDOWEVENT && mEvent.window.event == SDL_WINDOWEVENT_CLOSE){
                shouldExit = 1;
            }

            if(processViewerEvents(&mEvent) < 0){
                fprintf(stderr, "Error: Error processing Viewer Events.\n");
                return -1;
            }
        }

        /* Allow Viewer to update image and viewer stuff*/
        if(updateViewer() < 0){
            fprintf(stderr, "Error: Error processing viewer.\n");
            return -1;
        }
        if(SDL_GetTicks() >= lastTime + deltaTime){
            /* Update screen */
            SDL_RenderPresent(mainRenderer);
            lastTime = SDL_GetTicks();
        }
    }

    /* Quit SDL subsystems safely before returning */
    cleanViewer();
    SDL_DestroyWindow(mainWindow);
    IMG_Quit();
    SDL_Quit();
    return 0;
}