//
//  window.h
//  SDL_Test
//
//  Created by Nicholas Lamprecht on 24.03.17.
//  Copyright © 2017 Nicholas Lamprecht. All rights reserved.
//

#ifndef window_h 
#include <SDL2/SDL.h>
#define window_h 
#include "renderer.hpp"

//Screen Dimmensions
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 780;
//The Window in wich the programm is rendered
extern SDL_Window* gWindow;
//The surface cotained by the window
extern SDL_Surface* gScreenSurface;


bool init();

void close();





#endif /* window_h */ 

