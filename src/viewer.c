#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_image.h>
#include <SDL_events.h>

SDL_Renderer *vRenderer;
SDL_Texture *mainTexture;
int w, h, fw, fh;
SDL_Rect srcRect;
SDL_Rect destRect;

int initViewer(SDL_Renderer *mRenderer, char *filePath){
    vRenderer = mRenderer;
    if((mainTexture = IMG_LoadTexture(mRenderer, filePath)) == NULL){
        fprintf(stderr, "Error: Failed to load image. Likely improper file path: %s\n", SDL_GetError());
        return -1;
    }
    if(SDL_QueryTexture(mainTexture, NULL, NULL, &w, &h) < 0){
        fprintf(stderr, "Error: Could not query texture: %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

int processViewerEvents(SDL_Event *vEvent){
    /* Check for input to drag image or zoom image */
    if(vEvent->type == SDL_WINDOWEVENT){
        switch (vEvent->window.event){
            case SDL_WINDOWEVENT_RESIZED:
                break;
        }
    }
    return 0;
}

/* Main function to call functions to process input and change
content rendered */
int updateViewer(){
    SDL_RenderCopy(vRenderer, mainTexture, NULL, NULL);
    return 0;
}

void cleanViewer(){
    SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(vRenderer);
}