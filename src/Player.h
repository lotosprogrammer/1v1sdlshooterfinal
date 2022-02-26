#pragma once
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Enviorment.h"
#include "Gun.h"

#define PLAYER_DIMENSIONS 75

void InitPlayer();
void PlayerMovement(int deltaTime);
void CheckForBulletCollision();
void ResetPlayer();

extern SDL_Rect playerRect[2];
extern float pos[4];
extern float vel[4];

extern float speedCap;
extern float acc;
extern float jumpFrc;

extern float grvCap;
extern float grvRate;

extern bool inputRecieved[2];
extern bool isGrounded[2];

extern Mix_Chunk* hurtSFX;

extern int score;

extern TTF_Font* font;

extern SDL_Rect scoreRect;

extern SDL_Surface* scoreSurface;

extern SDL_Texture* scoreTex;

extern SDL_Rect winRect;

extern SDL_Surface* winSurface;

extern SDL_Texture* winTex;

extern bool hasWon;

extern int winTime;

extern int winCooldown;

