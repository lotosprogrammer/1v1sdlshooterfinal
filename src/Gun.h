#pragma once

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h> 
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "Enviorment.h"

#define BULLET_SIZE 10
#define FIRERATE 200

extern float rotSpeed;

extern float gunAngle[2];

extern int laserPos[8];

extern int laserOffset[2];

extern SDL_Rect* bullets;
extern float* bulletAngle;
extern float* dirX;
extern float* dirY;
extern float* bulletX;
extern float* bulletY;
extern int* bulletOwnership;

extern int amountOfBullets;

extern float bulletSpeed;

extern int sinceLastBulletUpdate;

extern int sinceLastShot[2];

extern Mix_Chunk* shootSFX;




void InitGun(SDL_Rect* playerRect);
void RotateGun(int deltaTime);
void Shoot(SDL_Rect* playerRect);
void SpawnBullets(int x, int y, float angle, int p);
void DestroyBullet(int i);
void UpdateBullet(int deltaTime);