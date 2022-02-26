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

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int sinceLastTextUpdate = 0;
int textUpdateRate = 200;
int sinceLastTimeUpdate = 0;

SDL_Surface* timeSurface;
SDL_Texture* timeTex;
SDL_Rect timeRect;

SDL_Surface* signatureSurface;
SDL_Texture* signatureTex;
SDL_Rect signatureRect;

int startTime = 0;

SDL_Color purple = {255, 0, 255};
SDL_Color dark = {0, 0, 0};

char temp0[64];
TTF_Font* pixelFont;

Mix_Music* music;

void InitWindow(){
    window = SDL_CreateWindow("1v1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 786, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    sinceLastTextUpdate = SDL_GetTicks();

    timeRect.x = 0;
    timeRect.y = 0;
    timeRect.w = 100;
    timeRect.h = 100;

        pixelFont = TTF_OpenFont("Pixelmania.ttf", 25);

    timeSurface = TTF_RenderText_Solid(pixelFont, "TIME", purple);

    sinceLastTimeUpdate = SDL_GetTicks();


    timeTex = SDL_CreateTextureFromSurface(renderer, timeSurface);

    signatureRect.x = 0;
    signatureRect.y = 736;
    signatureRect.w = 100,
    signatureRect.h = 50;

    signatureSurface = TTF_RenderText_Solid(pixelFont, "MADE BY JTV", dark);

    signatureTex = SDL_CreateTextureFromSurface(renderer, signatureSurface);
    
    music = Mix_LoadMUS("music.mp3");

    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(20);
}
void Render(){

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRects(renderer, enviorment, amountOfObjects);
    SDL_RenderFillRects(renderer, bullets, amountOfBullets);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &playerRect[0]);
    SDL_RenderDrawLine(renderer, laserPos[0], laserPos[1], laserPos[2], laserPos[3]);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &playerRect[1]);
    SDL_RenderDrawLine(renderer, laserPos[4], laserPos[5], laserPos[6], laserPos[7]);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    if(SDL_GetTicks() - textUpdateRate >= sinceLastTextUpdate){
        if(hasWon){
            
            winTex = SDL_CreateTextureFromSurface(renderer, winSurface);
            SDL_RenderCopy(renderer, winTex, NULL, &winRect);
        }
        SDL_DestroyTexture(scoreTex);
        scoreTex = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    }
    TimeDisplay();

    if(!hasWon){
        SDL_RenderCopy(renderer, scoreTex, NULL, &scoreRect);
        SDL_RenderCopy(renderer, timeTex, NULL, &timeRect); 
    }
    SDL_RenderCopy(renderer, signatureTex, NULL, &signatureRect);

 

    SDL_RenderPresent(renderer);

}  

void TimeDisplay(){
    if(SDL_GetTicks() - sinceLastTimeUpdate >= 200){
        sinceLastTimeUpdate = SDL_GetTicks();
        sprintf(temp0, "%d", (int)floor((SDL_GetTicks() - startTime) / 1000));
        SDL_FreeSurface(timeSurface);
        timeSurface = TTF_RenderText_Solid(pixelFont, temp0, purple);
        SDL_DestroyTexture(timeTex);

        timeTex = SDL_CreateTextureFromSurface(renderer, timeSurface);
    }
}