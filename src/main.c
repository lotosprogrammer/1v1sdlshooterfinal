#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Window.h"
#include "Player.h"
#include "Enviorment.h"
#include "Gun.h"

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    
    InitWindow();
    InitPlayer();
    InitGun(playerRect);

    bool isRunning = true;
    SDL_Event e;

    int deltaTime = 0;
    int prevFrame = 0;

    int sinceLastCheck;
    int fpsCounter;

    //Creates the level
    AddObject(0, 786, 1366, 25);
    AddObject(658, 675, 50, 50);
    AddObject(658, 625, 150, 75);
    AddObject(658, 165, 50, 185);
    AddObject(825, 165, 250, 50);
    AddObject(633, 325, 150, 25);
    AddObject(175, 500, 300, 75);
    AddObject(475, 650, 25, 500);
    AddObject(-50, 0, 50, 786);
    AddObject(1366, 0, 50, 786);
    AddObject(900, 711, 300, 25);
    AddObject(100, 200, 100, 100);
    AddObject(300, 200, 100, 100);
    AddObject(80, 600, 75, 25);
    AddObject(410, 275, 50, 25);
    AddObject(1000, 500, 200, 50);

    sinceLastCheck = SDL_GetTicks();

    while(isRunning){
        
        deltaTime = SDL_GetTicks() - prevFrame;
        prevFrame = SDL_GetTicks(); 
        if(deltaTime > 10){
            deltaTime = 0;
        }

        fpsCounter++;
        if(SDL_GetTicks() - sinceLastCheck >= 1000){
            printf("FPS: %i\n", fpsCounter);
            fpsCounter = 0;
            sinceLastCheck = SDL_GetTicks();
        }
        //uncomment to get fps


        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                isRunning = false;
             }
        }
        PlayerMovement(deltaTime);
        UpdateBullet(deltaTime);
        CheckForBulletCollision();
        RotateGun(deltaTime);
        Shoot(playerRect);

        Render();

        if(SDL_GetTicks() - winTime >= winCooldown && hasWon){
            hasWon = false;
            ResetPlayer();
            startTime = SDL_GetTicks();
            
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyTexture(winTex);
    SDL_DestroyTexture(timeTex);
    SDL_DestroyTexture(scoreTex);
    SDL_DestroyTexture(signatureTex);

    SDL_FreeSurface(winSurface);
    SDL_FreeSurface(signatureSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_FreeSurface(timeSurface);

    TTF_CloseFont(font);
    TTF_CloseFont(pixelFont);

    Mix_FreeChunk(hurtSFX);
    Mix_FreeChunk(shootSFX);

    Mix_FreeMusic(music);

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}