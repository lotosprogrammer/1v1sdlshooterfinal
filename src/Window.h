#pragma once
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Player.h"
#include "Enviorment.h"
#include "Gun.h"

void InitWindow();
void Render();
void TimeDisplay();

extern SDL_Renderer* renderer;
extern SDL_Window* window;

extern int sinceLastTextUpdate;
extern int textUpdateRate;

extern int sinceLastTimeUpdate;

extern SDL_Surface* timeSurface;
extern SDL_Texture* timeTex;
extern SDL_Rect timeRect;

extern SDL_Surface* signatureSurface;
extern SDL_Texture* signatureTex;
extern SDL_Rect signatureRect;

extern TTF_Font* pixelFont;

extern int startTime;

extern Mix_Music* music;