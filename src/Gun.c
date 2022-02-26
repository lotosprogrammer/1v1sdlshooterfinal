#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "Gun.h"
#include "Enviorment.h"

float rotSpeed = 0.75;

float gunAngle[2];
int laserOffset[2];
int laserPos[8];
SDL_Rect* bullets;
float* bulletAngle;
float* dirX;
float* dirY;
float* bulletX;
float* bulletY;
int* bulletOwnership;
int amountOfBullets = 0;
int sinceLastShot[2];
float bulletSpeed = 4;
int sinceLastBulletUpdate;
Mix_Chunk* shootSFX = NULL;

void InitGun(SDL_Rect* playerRect){

    laserOffset[0] = playerRect[0].w / 2;
    laserOffset[1] = playerRect[0].h / 2;
    sinceLastBulletUpdate = SDL_GetTicks();

    bullets = calloc(0, sizeof(SDL_Rect));
    bulletAngle = calloc(0, sizeof(float));
    dirX = calloc(0, sizeof(float));
    dirY = calloc(0, sizeof(float));
    bulletX = calloc(0, sizeof(float));
    bulletY = calloc(0, sizeof(float));
    bulletOwnership = calloc(0, sizeof(int));

    amountOfBullets = 0;
    shootSFX = Mix_LoadWAV("sfx/shoot.wav");
    Mix_VolumeChunk(shootSFX, 20);

    gunAngle[0] = 0;
    gunAngle[1] = 0;
}

void RotateGun(int deltaTime){
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if(keys[SDL_SCANCODE_E]){
        gunAngle[0] += rotSpeed * deltaTime;
    }

    if(keys[SDL_SCANCODE_LEFTBRACKET]){
        gunAngle[1] += rotSpeed * deltaTime;
    }


}


void Shoot(SDL_Rect* playerRect){
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        float angle1 = gunAngle[0] / 360;
    
        laserPos[0] = playerRect[0].x + laserOffset[0];
        laserPos[1] = playerRect[0].y + laserOffset[1];
        laserPos[2] = laserPos[0] + 2000 * cos(angle1);
        laserPos[3] = laserPos[1] + 2000 * sin(angle1);

        float angle2 = gunAngle[1] / 360;

        angle2 += 3.1415; // PI = 180˚

        laserPos[4] = playerRect[1].x + laserOffset[0];
        laserPos[5] = playerRect[1].y + laserOffset[1];
        laserPos[6] = laserPos[4] + 2000 * cos(angle2);
        laserPos[7] = laserPos[5] + 2000 * sin(angle2);

        if(keys[SDL_SCANCODE_R] && SDL_GetTicks() - sinceLastShot[0] >= FIRERATE){
            sinceLastShot[0] = SDL_GetTicks();
            SpawnBullets(laserPos[0], laserPos[1], angle1, 0);
            Mix_PlayChannel( -1, shootSFX, 0 );
        }
        if(keys[SDL_SCANCODE_RIGHTBRACKET] && SDL_GetTicks() - sinceLastShot[1] >= FIRERATE){
            sinceLastShot[1] = SDL_GetTicks();
            SpawnBullets(laserPos[4], laserPos[5], angle2, 1);
            Mix_PlayChannel( -1, shootSFX, 0 );
        }
        


}

void SpawnBullets(int x, int y, float angle, int p){
    //T H I C C is the only way  i can describe this 
    amountOfBullets++;
    SDL_Rect newBullet;
    newBullet.x = x;
    newBullet.y = y - BULLET_SIZE / 2;
    newBullet.w = BULLET_SIZE;
    newBullet.h = BULLET_SIZE;
    bullets = realloc(bullets, sizeof(SDL_Rect) * amountOfBullets);
    bulletAngle = realloc(bulletAngle, sizeof(float) * amountOfBullets);
    dirX = realloc(dirX, sizeof(float) * amountOfBullets);
    dirY = realloc(dirY, sizeof(float) * amountOfBullets);
    bulletX = realloc(bulletX, sizeof(float) * amountOfBullets);
    bulletY = realloc(bulletY, sizeof(float) * amountOfBullets);
    bulletOwnership = realloc(bulletOwnership, sizeof(int) * amountOfBullets);

    bullets[amountOfBullets - 1] = newBullet;
    bulletAngle[amountOfBullets - 1] = angle;
    dirX[amountOfBullets - 1] = bulletSpeed * cos(angle);
    dirY[amountOfBullets - 1] = bulletSpeed * sin(angle);
    bulletX[amountOfBullets - 1] = x;
    bulletY[amountOfBullets - 1] = y - BULLET_SIZE / 2;
    bulletOwnership[amountOfBullets - 1] = p;  
}

void DestroyBullet(int i){
    int x = 0;
    for(int y = 0; y < amountOfBullets; y++){
        if(i == y){
            y++;
        }
        bullets[x] = bullets[y];
        bulletAngle[x] = bulletAngle[y];
        dirX[x] = dirX[y];
        dirY[x] = dirY[y];
        bulletX[x] = bulletX[y];
        bulletY[x] = bulletY[y];
        bulletOwnership[x] = bulletOwnership[y];
        x++;
    }
    amountOfBullets--;
}

void UpdateBullet(int deltaTime){
        for(int i = 0; i < amountOfBullets; i++){
            bulletX[i] += dirX[i] * deltaTime;
            bulletY[i] += dirY[i] * deltaTime;

            bullets[i].x = bulletX[i];
            bullets[i].y = bulletY[i];
        }
        if(SDL_GetTicks() - sinceLastBulletUpdate >= FIRERATE){
            for(int i = 0; i < amountOfBullets; i++){
                for(int j = 0; j < amountOfObjects; j++){
                     if (bullets[i].x < enviorment[j].x + enviorment[j].w &&
                        bullets[i].x + bullets[i].w > enviorment[j].x &&
                        bullets[i].y < enviorment[j].y + enviorment[j].h &&
                        bullets[i].h + bullets[i].y > enviorment[j].y){
                            DestroyBullet(i);
                        }
                    else if(bullets[i].x > 1366 || bullets[i].x < 0 || bullets[i].y < 0 || bullets[i].y > 786){
                        DestroyBullet(i);
                    }
                }
            }
        }
    

}