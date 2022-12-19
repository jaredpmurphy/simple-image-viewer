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
    /* get width and height of image loaded */
    SDL_QueryTexture(mainTexture, NULL, NULL, &w, &h);
    /* Get size of viewport and store in displayRect */
    SDL_RenderGetViewport(vRenderer, &displayRect);
    /* Centre it, for posterity? */
    destRect.x = (int)(displayRect.w/2) - (int)(w/2);
    destRect.y = (int)(displayRect.h/2) - (int)(h/2);
    destRect.w = w;
    destRect.h = h;
    return 0;
}

int mPressed;
int xoff;
int yoff;
int processViewerEvents(SDL_Event *vEvent){
    /* Check for window resize and update displayRect for updateViewer calculations */
    if(vEvent->type == SDL_WINDOWEVENT){
        switch (vEvent->window.event){
            case SDL_WINDOWEVENT_RESIZED:
                SDL_RenderGetViewport(vRenderer, &displayRect);
                break;
        }
    } else if (vEvent->type == SDL_MOUSEWHEEL){
        if(vEvent->wheel.preciseY > 0 && zoom < 10){
            //scroll up
            zoom += vEvent->wheel.preciseY/5;
        }else if(vEvent->wheel.preciseY < 0 && zoom > 0.5){
            //scroll down
            zoom += vEvent->wheel.preciseY/5;
        }
    } else if (vEvent->type == SDL_MOUSEBUTTONDOWN){
        if(vEvent->button.button == SDL_BUTTON_LEFT)
            mPressed = 1;
    } else if(vEvent->type == SDL_MOUSEBUTTONUP){
        if(vEvent->button.button == SDL_BUTTON_LEFT)
            mPressed = 0;
    } else if(vEvent->type == SDL_MOUSEMOTION && mPressed == 1){
        xoff += vEvent->motion.xrel;
        yoff += vEvent->motion.yrel;
    }
    return 0;
}

/* Main function to call functions to process input and change
content rendered */
int updateViewer(){
    //reset xoff yoff when needed
    if((int)w*zoom <= displayRect.w || (int)h*zoom <= displayRect.y){
        destRect.x = (int)(displayRect.w/2) - (int)(w*zoom/2);
        destRect.y = (int)(displayRect.h/2) - (int)(h*zoom/2);
        xoff = 0;
        yoff = 0;
    }else{
        destRect.x = (int)(displayRect.w/2) - (int)(w*zoom/2) + (int)xoff;
        destRect.y = (int)(displayRect.h/2) - (int)(h*zoom/2) + (int)yoff;
    }
    destRect.w = (int)w*zoom;
    destRect.h = (int)h*zoom;
    SDL_RenderCopy(vRenderer, mainTexture, NULL, &destRect);
    return 0;
}

void cleanViewer(){
    SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(vRenderer);
}