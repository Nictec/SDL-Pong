#include <stdio.h> 
#include <unistd.h> 
#include <cstdint>
#include <SDL2/SDL.h> 
#include "window.hpp"
#include "renderer.hpp" 


int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() ){
      printf( "Failed to initialize!\n" );
    }else{ 
        if( !loadBG() || !loadPaddle() ){
         printf( "Failed to load Background or Sprites!\n" );
        }else{
            // setting external running flag to true
            bool running = true;
            //SDL Event Handler
            SDL_Event e; 
            //run loop 
            int user_y = 0;
            while(running){ 
                //boolean for only creating the paddle once
                //event loop
                while( SDL_PollEvent( &e ) != 0 ){
                    //sets running = false wich leads to the termination of the "running" loop.
                    if( e.type == SDL_QUIT ){ 
                        // Todo(Nicholas): Maybe warn the user before closing the App?
                        running=false;
                    } 
                    //get the user input from the arrow keys. 
                    if(e.type == SDL_KEYDOWN){ 
                        const Uint8 *state = SDL_GetKeyboardState(NULL);  
                        if (state[SDL_SCANCODE_UP]) { 
                            if(user_y > -250){
                                user_y = user_y - 25;
                            }
                        }
                        if (state[SDL_SCANCODE_DOWN]) { 
                            if(user_y < 325.5){
                                user_y = user_y + 25; 
                            }
                        }

                    } 
                    //end of event loop
                } 
                //render or rerender of the paddles and background with user input values (depending on the programm state). 
                renderSurface(user_y); 
        }
    }
}
    close();
    return 0;
    
}





