#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern SDL_Rect* enviorment;

void AddObject(int x, int y, int w, int h);
extern int amountOfObjects;