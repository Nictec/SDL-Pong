//
//  sprites.cpp
//  SDL_Test
//
//  Created by Nicholas Lamprecht on 24.03.17.
//  Copyright © 2017 Nicholas Lamprecht. All rights reserved.
// 

#include <SDL2/SDL.h>
#include <stdio.h>
#include "sprites.hpp"
#include "window.hpp"

//global vars
SDL_Surface* paddle = NULL; 

bool loadPaddle(){
    //loading success flag
    bool success = false;
    //load sprite image
    paddle = SDL_LoadBMP("images/paddle.bmp");
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
