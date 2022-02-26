#include "Enviorment.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Rect* enviorment;
int amountOfObjects;

void AddObject(int x, int y, int w, int h){
    amountOfObjects++;
    SDL_Rect newObject;
    newObject.x = x;
    newObject.y = y;
    newObject.w = w;
    newObject.h = h;

    enviorment = realloc(enviorment, sizeof(SDL_Rect) * amountOfObjects);
    enviorment[amountOfObjects - 1] = newObject;
}