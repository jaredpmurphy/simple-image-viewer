#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_image.h>
#include <SDL_events.h>

SDL_Renderer *vRenderer;
SDL_Texture *mainTexture;
int w, h;
SDL_Rect destRect;
SDL_Rect displayRect;
double zoom = 1.0;

int initViewer(SDL_Renderer *mRenderer, char *filePath){
    vRenderer = mRenderer;
    if((mainTexture = IMG_LoadTexture(mRenderer, filePath)) == NULL){
        fprintf(stderr, "Error: Failed to load image. Likely improper file path: %s\n", SDL_GetError());
        return -1;
    }
    SDL_QueryTexture(mainTexture, NULL, NULL, &w, &h);
    SDL_RenderGetViewport(vRenderer, &displayRect);
    destRect.x = (int)(displayRect.w/2) - (int)(w/2);
    destRect.y = (int)(displayRect.h/2) - (int)(h/2);
    destRect.w = w;
    destRect.h = h;
    return 0;
}

int processViewerEvents(SDL_Event *vEvent){
    /* Check for input to drag image or zoom image */
    if(vEvent->type == SDL_WINDOWEVENT){
        switch (vEvent->window.event){
            case SDL_WINDOWEVENT_RESIZED:
                SDL_RenderGetViewport(vRenderer, &displayRect);
                break;
        }
    }
    return 0;
}

/* Main function to call functions to process input and change
content rendered */
int updateViewer(){
    destRect.x = (int)(displayRect.w/2) - (int)(w/2);
    destRect.y = (int)(displayRect.h/2) - (int)(h/2);
    destRect.w = w;
    destRect.h = h;
    SDL_RenderCopy(vRenderer, mainTexture, NULL, &destRect);
    return 0;
}

void cleanViewer(){
    SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(vRenderer);
}