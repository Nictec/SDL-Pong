  //
//  window.cpp
//  SDL_Test
//
//  Created by Nicholas Lamprecht on 24.03.17.
//  Copyright © 2017 Nicholas Lamprecht. All rights reserved.
//

#include <stdio.h> 
#include "window.hpp" 

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL; 
SDL_Surface* gSpriteSurface = NULL; 
SDL_Renderer *gRenderer = NULL; 


bool init(){
    //Initialization flag
    bool success = true;
    //init SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }else{ 
        // note(Nicholas): screen size constants are in the window.hpp file
        //create Window
        gWindow = SDL_CreateWindow("SDL pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); 
        if(gWindow == NULL){
            printf( "Window or Renderer Error! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }else{
            //get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow); 
            gSpriteSurface = SDL_GetWindowSurface(gWindow);
        }
        
    }
    
return success;
    
}

void close(){    
    //Destroy Window
    SDL_DestroyWindow(gWindow);
    gWindow=NULL;
    
    //quit SDL Subsystems
    SDL_Quit();
}