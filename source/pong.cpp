#include "pong.h"

internal int32
RoundReal32ToInt32(real32 Real32)
{
    int32 Result = (int32)(Real32 + 0.5f);
    // TODO(casey): Intrinsic????
    return(Result);
}

internal void
DrawRectangle(game_offscreen_buffer *Buffer,
              real32 RealMinX, real32 RealMinY, real32 RealMaxX, real32 RealMaxY,
              uint32 Color)
{
    // TODO(casey): Floating point color tomorrow!!!!!!

    int32 MinX = RoundReal32ToInt32(RealMinX);
    int32 MinY = RoundReal32ToInt32(RealMinY);
    int32 MaxX = RoundReal32ToInt32(RealMaxX);
    int32 MaxY = RoundReal32ToInt32(RealMaxY);

    if(MinX < 0)
    {
        MinX = 0;
    }

    if(MinY < 0)
    {
        MinY = 0;
    }

    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }


    uint8 *Row = ((uint8 *)Buffer->Memory +
                  MinX*Buffer->BytesPerPixel +
                  MinY*Buffer->Pitch);
    for(int Y = MinY;
        Y < MaxY;
        ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = MinX;
            X < MaxX;
            ++X)
        {
            *Pixel++ = Color;
        }

        Row += Buffer->Pitch;
    }
}

    /*
internal void renderSurface(game_offscreen_buffer *Buffer, float yModificatorL, float yModificatorR, float bX, float bY)
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
}
*/

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

    real32 paddleSpeed = 3.0f;

    if(Keyboard->ActionUp.EndedDown){
        if(GameState->user_yR > 0){
            GameState->user_yR = GameState->user_yR -paddleSpeed;
        }
    }
    if(Keyboard->ActionDown.EndedDown){
        if(GameState->user_yR < 100){
            GameState->user_yR = GameState->user_yR +paddleSpeed;
        }
    }
    if(Keyboard->MoveUp.EndedDown){
        if(GameState->user_yL > 0){
            GameState->user_yL = GameState->user_yL -paddleSpeed;
        }
    }
    if(Keyboard->MoveDown.EndedDown){
        if(GameState->user_yL < 100){
            GameState->user_yL = GameState->user_yL +paddleSpeed;
        }
    }


    // background
    DrawRectangle(Buffer, 0.0f, 0.0f, (real32)Buffer->Width, (real32)Buffer->Height, 0x00FF00FF);

    real32 paddle_width = 40.0f;
    real32 paddle_height = 100.0f;
    DrawRectangle(Buffer,
            0.0f,
            ( GameState->user_yL * (Buffer->Height - paddle_height) / 100 ),
            paddle_width,
            ( GameState->user_yL * (Buffer->Height - paddle_height) / 100 ) + paddle_height,
            0x0000FFFF);
    DrawRectangle(Buffer,
            Buffer->Width - paddle_width,
            ( GameState->user_yR * (Buffer->Height - paddle_height) / 100 ),
            Buffer->Width,
            ( GameState->user_yR * (Buffer->Height - paddle_height) / 100 ) + paddle_height,
            0x0000FFFF);
    //renderSurface(Buffer, GameState->user_yL, GameState->user_yR, 0, 0);
}
