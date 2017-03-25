#include <stdio.h>
#include <SDL2/SDL.h> 
#include "window.hpp"
#include "renderer.hpp" 
#include "sprites.hpp"


int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() ){
      printf( "Failed to initialize!\n" );
    }else{
        if(!loadBG() && !loadPaddle()){
         printf( "Failed to load Background or Sprites!\n" );
        }else{
            //main quit flag
            bool quit = false;
            //SDL Event Handler
            SDL_Event e; 
            //run loop
            while(!quit){
                //event loop
                while( SDL_PollEvent( &e ) != 0 ){
                    //sets quit to true wich leads to fall out of the run loop
                    if( e.type == SDL_QUIT ){
                        quit=true;
                    }
                }
            
            //apply Background image
            SDL_BlitSurface(background, NULL, gScreenSurface, NULL);
            //render paddles
                //load source coordinates
                SDL_Rect src;
                src.x = 0;
                src.y = 0;
                src.w = 37;
                src.h = 191;
                //load the destination coordinates
                SDL_Rect dest;
                dest.x = 640/2;
                dest.y = 480/2;
                dest.w = 37;
                dest.h = 191;
            SDL_BlitSurface(paddle, NULL, gScreenSurface, &dest);
            //Update Window
            SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    close();
    return 0;
    
}





