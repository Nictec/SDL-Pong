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
#include "sprites.hpp"

SDL_Surface* background = NULL;

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
    
    
