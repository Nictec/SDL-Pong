//
//  media.h
//  SDL_Test
//
//  Created by Nicholas Lamprecht on 24.03.17.
//  Copyright © 2017 Nicholas Lamprecht. All rights reserved.
//

#ifndef media_h
#define media_h 
#include <SDL2/SDL.h>
#include <stdio.h> 

//global vars
extern SDL_Surface* background; 
extern SDL_Surface* paddle_right;


bool loadBG(); 
bool loadPaddle(); 
void renderSurface(int yModificatorL, int yModificatorR);

#endif /* media_h */