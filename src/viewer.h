#ifndef VIEWER_H
#define VIEWER_H

#include <SDL.h>
#include <SDL_events.h>

/* Initialize viewer and loads image */
int initViewer(SDL_Renderer *mRenderer, char *filePath);

int processViewerEvents(SDL_Event *vEvent);

/* Main function to call functions to process input and change
content rendered */
int updateViewer();

/* Cleans up viewer */
void cleanViewer();

#endif