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
        if( !loadBG() || !loadPaddle() || !loadBall() ){
         printf( "Failed to load Background or Sprites!\n" );
        }else{
            // setting external running flag to true
            bool running = true;
            //SDL Event Handler
            SDL_Event e; 
            //run loop 
            int user_yL = 0; 
            int user_yR = 0;
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
                        //checking if the paddle is inside of the playfield.  
                        if (state[SDL_SCANCODE_W]) { 
                            if(user_yL > -250){ 
                                //calculation of the position modifier
                                user_yL = user_yL - 25;
                            }
                        }
                        if (state[SDL_SCANCODE_S]) { 
                        //checking if the paddle is inside of the playfield. 
                            if(user_yL < 325.5){ 
                                //calculation of the position modifier
                                user_yL = user_yL + 25; 
                            }
                        } 
                        if(state[SDL_SCANCODE_UP]){ 
                            if(user_yR > -250){ 
                                user_yR = user_yR -25;
                            }
                        } 
                        if(state[SDL_SCANCODE_DOWN]){ 
                            if(user_yR < 325.5){ 
                                user_yR = user_yR +25;
                            }
                        }

                    } 
                    //end of event loop (Info for me, so that i don´t have to search for it.)
                } 
                //render or re-render of the paddles and background with user input values (depending on the state of the programm).  
                renderSurface(user_yL, user_yR, 0, 0); 
        }
    }
}
    close();
    return 0;
    
}
