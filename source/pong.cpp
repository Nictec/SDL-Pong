#include "pong.h"

internal void renderSurface(game_offscreen_buffer *Buffer, float yModificatorL, float yModificatorR, float bX, float bY)
{
    // TODO(casey): Let's see what the optimizer does

    int BlueOffset = (int) yModificatorL;
    int GreenOffset = (int) yModificatorR;
    uint8 *Row = (uint8 *)Buffer->Memory;
    for(int Y = 0;
            Y < Buffer->Height;
            ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = 0;
                X < Buffer->Width;
                ++X)
        {
            uint8 Blue = (uint8)(X + BlueOffset);
            uint8 Green = (uint8)(Y + GreenOffset);

            *Pixel++ = ((Green << 8) | Blue);
        }

        Row += Buffer->Pitch;
    }
    /*
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
    //SDL Rectangle for the Ball
    SDL_Rect srcBall;
    srcBall.x = 0;
    srcBall.y = 0;
    srcBall.w = 100;
    srcBall.h = 104;

    SDL_Rect destBall;
    destBall.x = 675 + bX;
    destBall.y = 276.5 + bY;
    destBall.w = 100;
    destBall.h = 104;

    //apply Background image
    SDL_BlitSurface(background, NULL, gScreenSurface, NULL);
    //apply the paddles
    SDL_BlitSurface(paddle, &src, gSpriteSurface, &dest);
    SDL_BlitSurface(paddle2, &srcR, gSpriteSurface, &destR);
    //apply ball
    SDL_BlitSurface(ball, &srcBall, gSpriteSurface, &destBall);
    //Update Window
    SDL_UpdateWindowSurface(gWindow);
    */
}

internal void GameUpdateAndRender(game_memory *Memory, game_input *Input, game_offscreen_buffer *Buffer){
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);

    game_state *GameState = (game_state *)Memory->PermanentStorage;
    if(!Memory->IsInitialized)
    {
#if 0
        char *Filename = __FILE__;

        debug_read_file_result File = DEBUGPlatformReadEntireFile(Filename);
        if(File.Contents)
        {
            DEBUGPlatformWriteEntireFile("test.out", File.ContentsSize, File.Contents);
            DEBUGPlatformFreeFileMemory(File.Contents);
        }

#endif
        GameState->user_yR = 0;
        GameState->user_yL = 0;
        // TODO(casey): This may be more appropriate to do in the platform layer
        Memory->IsInitialized = true;
    }

    game_controller_input *Keyboard = GetController(Input, 0);
    if(Keyboard->ActionUp.EndedDown){
        if(GameState->user_yR > -250){
            GameState->user_yR = GameState->user_yR -25;
        }
    }
    if(Keyboard->ActionDown.EndedDown){
        if(GameState->user_yR < 325.5){
            GameState->user_yR = GameState->user_yR +25;
        }
    }
    if(Keyboard->MoveUp.EndedDown){
        if(GameState->user_yL > -250){
            GameState->user_yL = GameState->user_yL -25;
        }
    }
    if(Keyboard->MoveDown.EndedDown){
        if(GameState->user_yL < 325.5){
            GameState->user_yL = GameState->user_yL +25;
        }
    }


    renderSurface(Buffer, GameState->user_yL, GameState->user_yR, 0, 0);
}
