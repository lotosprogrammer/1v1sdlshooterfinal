#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Player.h"
#include "Enviorment.h"
#include "Gun.h"

SDL_Rect playerRect[2];
float pos[4];
float vel[4];

float speedCap = 0.7;
float acc = 0.02;
float jumpFrc = 2;

float grvCap = 1.5;
float grvRate = 0.01;

bool inputRecieved[2];
bool isGrounded[2];

Mix_Chunk* hurtSFX;

int score = 0;

TTF_Font* font = NULL;

SDL_Rect scoreRect;

SDL_Surface* scoreSurface = NULL;

SDL_Texture* scoreTex = NULL;

SDL_Color color = {0, 255, 0};

SDL_Rect winRect;

SDL_Surface* winSurface;

SDL_Texture* winTex;

bool hasWon = false;

int winTime;

int winCooldown = 5000;


char temp[64];//lets say its temporary
void InitPlayer()
{
    // a whole lot of initialazation
    playerRect[0].x = 0;
    playerRect[0].y = 786 - PLAYER_DIMENSIONS;
    playerRect[0].w = PLAYER_DIMENSIONS;
    playerRect[0].h = PLAYER_DIMENSIONS;

    playerRect[1].x = 750;
    playerRect[1].y = 786 - PLAYER_DIMENSIONS;
    playerRect[1].w = PLAYER_DIMENSIONS;
    playerRect[1].h = PLAYER_DIMENSIONS;

    pos[0] = playerRect[0].x;
    pos[1] = playerRect[0].y;
    pos[2] = playerRect[1].x;
    pos[3] = playerRect[1].y;

    vel[0] = 0;
    vel[1] = 0;
    vel[2] = 0;
    vel[3] = 0;

    scoreRect.x = 633;
    scoreRect.y = 0;
    scoreRect.w = 100;
    scoreRect.h = 100;

    inputRecieved[0] = false,
    inputRecieved[1] = false;

    isGrounded[0] = false;
    isGrounded[1] = false;

    winRect.x = 0;
    winRect.y = 0;
    winRect.w = 1366;
    winRect.h = 500;

    hurtSFX = Mix_LoadWAV("sfx/hurt.wav");
    Mix_VolumeChunk(hurtSFX, 60);

    font = TTF_OpenFont("Pixelmania.ttf", 25);
    scoreSurface = TTF_RenderText_Solid(font, "SCORE", color);
    winSurface = TTF_RenderText_Solid(font, "GREEN LOST", color);

}

void PlayerMovement(int deltaTime)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_Q])
    {
        vel[0] -= acc * deltaTime;
        inputRecieved[0] = true;
    }
    if (keys[SDL_SCANCODE_W])
    {
        vel[0] += acc * deltaTime;
        inputRecieved[0] = true;
    }
    if (keys[SDL_SCANCODE_O])
    {
        vel[2] -= acc * deltaTime;
        inputRecieved[1] = true;
    }
    if (keys[SDL_SCANCODE_P])
    {
        vel[2] += acc * deltaTime;
        inputRecieved[1] = true;
    }
    for (int i = 0; i < 2; i++){
        int index = i*2;
        if (!inputRecieved[i]){
            if (vel[index] != 0){
                if (vel[index] > 0){
                    vel[index] -= acc;
                    if (vel[index] < 0){
                        vel[index] = 0;
                    }
                }
                else{
                    vel[index] += acc;
                    if (vel[index] > 0)
                    {
                        vel[index] = 0;
                    }
                }
            }
        }
            if (vel[index] > speedCap)
            {
                vel[index] = speedCap;
            }
            else if (vel[index] < -speedCap)
            {
                vel[index] = -speedCap;
            }
    }
        bool hasTouched[2];
        for (int i = 0; i < amountOfObjects; i++){
            for (int j = 0; j < 2; j++){
                int indexY = j*2+1;
                int indexX = j*2;
                if (playerRect[j].x < enviorment[i].x + enviorment[i].w - 1 &&
                playerRect[j].x + playerRect[j].w > enviorment[i].x + 1 &&
                playerRect[j].y < enviorment[i].y + jumpFrc &&
                playerRect[j].h + playerRect[j].y > enviorment[i].y) {
                    hasTouched[j] = true;
                    vel[indexY] = 0;
                    playerRect[j].y = enviorment[i].y  - playerRect[j].h;
                }
                else if(playerRect[j].x < enviorment[i].x + enviorment[i].w - 1 &&
                playerRect[j].x + playerRect[j].w > enviorment[i].x + 1 &&
                playerRect[j].y <= enviorment[i].y + enviorment[i].h &&
                playerRect[j].h + playerRect[j].y > enviorment[i].y) {
                    vel[indexY] = 0;
                    hasTouched[j] = false;
                    playerRect[j].y = enviorment[i].y + enviorment[i].h;
                }
                else if(playerRect[j].x < enviorment[i].x + speedCap + 1 &&
                playerRect[j].x + playerRect[j].w > enviorment[i].x &&
                playerRect[j].y < enviorment[i].y + enviorment[i].h &&
                playerRect[j].h + playerRect[j].y > enviorment[i].y) {
                    if(vel[indexX] > 0){
                        vel[indexX] = 0;
                        playerRect[j].x = enviorment[i].x - playerRect[j].w;
                    }
                }
                else if(playerRect[j].x < enviorment[i].x + enviorment[i].w &&
                    playerRect[j].x + playerRect[j].w > enviorment[i].x &&
                    playerRect[j].y < enviorment[i].y + enviorment[i].h &&
                    playerRect[j].y + playerRect[j].h > enviorment[i].y) {
                    if(vel[indexX] < 0){
                        vel[indexX] = 0;
                        playerRect[j].x = enviorment[i].x + enviorment[i].w;
                    }
                }
                else{
                   //if no collisions are detected
                }
            }
        }
        isGrounded[0] = hasTouched[0];
        isGrounded[1] = hasTouched[1];
            for(int i = 0; i < 2; i++){
                int index = i*2+1;
                if(!isGrounded[i]){
                    vel[index] += grvRate * deltaTime;
                    if(vel[index] > grvCap){
                        vel[index] = grvCap;
                    }
                }  
            }   
    if(keys[SDL_SCANCODE_LALT] && isGrounded[0]){
        vel[1] = -jumpFrc;
    }
    if(keys[SDL_SCANCODE_RALT] && isGrounded[1]){
        vel[3] = -jumpFrc;
    }

    //x
    pos[0] += vel[0] * deltaTime;
    pos[2] += vel[2] * deltaTime;

    //y
    pos[1] += vel[1] * deltaTime;
    pos[3] += vel[3] * deltaTime;

    playerRect[0].x = floor(pos[0]);
    playerRect[1].x = floor(pos[2]);

    playerRect[0].y = floor(pos[1]);
    playerRect[1].y = floor(pos[3]);

    inputRecieved[0] = false;
    inputRecieved[1] = false;
}

void CheckForBulletCollision(){
    if(SDL_GetTicks() - sinceLastBulletUpdate > FIRERATE){
        for(int i = 0; i < amountOfBullets; i++){
            for(int y = 0; y < 2; y++){
                if (playerRect[y].x < bullets[i].x + bullets[i].w &&
                    playerRect[y].x + playerRect[y].w > bullets[i].x &&
                    playerRect[y].y < bullets[i].y + bullets[i].h &&
                    playerRect[y].h + playerRect[y].y > bullets[i].y) {
                        if(bulletOwnership[i] != y){
                            Mix_PlayChannel(-1, hurtSFX, 0);

                            if(bulletOwnership[i] == 0){
                                if(score != 0)
                                    score -= 5;
                            }
                            if(bulletOwnership[i] == 1){
                                score += 5;
                            }
                            if(score >= 100){
                                hasWon = true;
                                winTime = SDL_GetTicks();
                            }
                            sprintf(temp, "%i", score);
                            SDL_FreeSurface(scoreSurface);
                            scoreSurface = TTF_RenderText_Solid(font, temp, color);
                            DestroyBullet(i);
                        }
                    }
            }
        }
    }
}

void ResetPlayer(){
    InitPlayer();
    InitGun(playerRect);
    scoreSurface = TTF_RenderText_Solid(font, "SCORE", color);
    score = 0;
}