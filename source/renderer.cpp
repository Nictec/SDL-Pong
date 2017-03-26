//
//  media.cpp
//  SDL_Test
//
//  Created by Nicholas Lamprecht on 24.03.17.
//  Copyright © 2017 Nicholas Lamprecht. All rights reserved.
//

#include <stdio.h> 
#include <SDL2/SDL.h> 
#include "renderer.hpp"
#include "window.hpp"

SDL_Surface* background = NULL; 
SDL_Surface* paddle = NULL; 
SDL_Surface* paddle2 = NULL;

bool loadBG(){
    //loading success flag
    bool success = true;
    //load splash image
    background = SDL_LoadBMP("images/background.bmp");
    if(background == NULL){
        printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/press.bmp", SDL_GetError() );
        success = false;
    }
    return success;
} 

bool loadPaddle(){
    //loading success flag
    bool success = false;
    //load sprite image
    paddle = SDL_LoadBMP("images/paddle.bmp"); 
    paddle2 = SDL_LoadBMP("images/paddle.bmp");
    //check if image is successfully loaded
    if(paddle == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Unable to load paddle.bmp",
                                 "File is missing. Please reinstall the program.",
                                 NULL);

        success = false;
    }else{
        success = true;
    }
    return success;
} 

void renderSurface(int yModificatorL, int yModificatorR)
{
	        //SDL Rectangle for the paddles 
            SDL_Rect src; 
            src.x = 0; 
            src.y = 0; 
            src.w = 37; 
            src.h = 191; 

            SDL_Rect dest; 
            dest.x = 20; 
            dest.y = 276.5 + yModificatorL; 
            dest.w = 0; 
            dest.h = 0; 

            //SDL Rectangle for the paddles 
            SDL_Rect srcR; 
            srcR.x = 0; 
            srcR.y = 0; 
            srcR.w = 37; 
            srcR.h = 191; 

            SDL_Rect destR; 
            destR.x = 1350; 
            destR.y = 276.5 + yModificatorR; 
            destR.w = 0; 
            destR.h = 0;

            //apply Background image
            SDL_BlitSurface(background, NULL, gScreenSurface, NULL); 
            //apply left paddle
            SDL_BlitSurface(paddle, &src, gSpriteSurface, &dest); 
            SDL_BlitSurface(paddle2, &srcR, gSpriteSurface, &destR);
            //Update Window
            SDL_UpdateWindowSurface(gWindow); 
}