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

internal void resetBallAndPaddles(game_state *GameState){
    GameState->paddle_r_y = 50;
    GameState->paddle_l_y = 50;

    GameState->ball_x = 50;
    GameState->ball_y = 50;
    GameState->ball_dx = 30;
    GameState->ball_dy = 0;
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
        GameState->game_over = false;
        resetBallAndPaddles(GameState);
        // TODO(casey): This may be more appropriate to do in the platform layer
        Memory->IsInitialized = true;
    }

    game_controller_input *Keyboard = GetController(Input, 0);

    real32 paddleSpeed = 60.0f * Input->SecondsToAdvanceOverUpdate;
    real32 max_ball_dy = 60.0f;
    real32 ball_ddx = 2.0f;
    real32 paddle_width = 2.0f;
    real32 paddle_height = 20.0f;
    real32 ball_radius = 2.0f;

    if(Keyboard->ActionUp.EndedDown){
        GameState->paddle_r_y -= paddleSpeed;
    }
    if(Keyboard->ActionDown.EndedDown){
        GameState->paddle_r_y += paddleSpeed;
    }
    if(Keyboard->MoveUp.EndedDown){
        GameState->paddle_l_y -= paddleSpeed;
    }
    if(Keyboard->MoveDown.EndedDown){
        GameState->paddle_l_y += paddleSpeed;
    }

    // clamp
    if(GameState->paddle_r_y < (paddle_height / 2)){
        GameState->paddle_r_y = (paddle_height / 2);
    }
    if(GameState->paddle_r_y > 100 - (paddle_height / 2) ){
        GameState->paddle_r_y = 100 - (paddle_height / 2);
    }
    if(GameState->paddle_l_y < (paddle_height / 2)){
        GameState->paddle_l_y = (paddle_height / 2);
    }
    if(GameState->paddle_l_y > 100 - (paddle_height / 2) ){
        GameState->paddle_l_y = 100 - (paddle_height / 2);
    }

    GameState->ball_y += GameState->ball_dy * Input->SecondsToAdvanceOverUpdate;
    if(GameState->ball_y > 100-ball_radius){
        GameState->ball_y = 100-ball_radius;
        GameState->ball_dy = -GameState->ball_dy;
    }
    if(GameState->ball_y < ball_radius){
        GameState->ball_y = ball_radius;
        GameState->ball_dy = -GameState->ball_dy;
    }

    GameState->ball_x += GameState->ball_dx * Input->SecondsToAdvanceOverUpdate;

    real32 ball_left_edge = GameState->ball_x - ball_radius;
    if(ball_left_edge < paddle_width){
        if(!GameState->game_over
                && ((GameState->ball_y - ball_radius < GameState->paddle_l_y + paddle_height / 2)
                && ((GameState->ball_y + ball_radius > GameState->paddle_l_y - paddle_height / 2)))){
            if(GameState->ball_dx < 0){
                GameState->ball_dx = -GameState->ball_dx + ball_ddx;

                GameState->ball_dy =
                    (GameState->ball_y - GameState->paddle_l_y)
                    * ( max_ball_dy / (paddle_height / 2 + ball_radius));
            }
        } else {
            GameState->game_over = true;
            if(GameState->ball_x < 0 - ball_radius){
                // restart
                GameState->game_over = false;
                resetBallAndPaddles(GameState);
            }
        }
    }

    real32 ball_right_edge = GameState->ball_x + ball_radius;
    if(ball_right_edge > 100 - paddle_width){
        if(!GameState->game_over
                && ((GameState->ball_y - ball_radius < GameState->paddle_r_y + paddle_height / 2)
                && ((GameState->ball_y + ball_radius > GameState->paddle_r_y - paddle_height / 2)))){
            if(GameState->ball_dx > 0){
                GameState->ball_dx = -GameState->ball_dx - ball_ddx;

                GameState->ball_dy =
                    (GameState->ball_y - GameState->paddle_r_y)
                    * ( max_ball_dy / (paddle_height / 2 + ball_radius));
            }
        } else {
            GameState->game_over = true;
            if(GameState->ball_x > 100 + ball_radius){
                // restart
                GameState->game_over = false;
                resetBallAndPaddles(GameState);
            }
        }
    }




    // background
    DrawRectangle(Buffer, 0.0f, 0.0f, (real32)Buffer->Width, (real32)Buffer->Height, 0x00000000);

    real32 width_correction = ((real32) Buffer->Width) / 100.0f;
    real32 height_correction = ((real32) Buffer->Height) / 100.0f;

    // paddles
    DrawRectangle(Buffer,
            0 * width_correction,
            ( GameState->paddle_l_y - (paddle_height/2)) * height_correction,
            paddle_width * width_correction,
            ( GameState->paddle_l_y + (paddle_height/2)) * height_correction,
            0x00FF0000);

    DrawRectangle(Buffer,
            (100 - paddle_width) * width_correction,
            ( GameState->paddle_r_y - (paddle_height/2)) * height_correction,
            100 * width_correction,
            ( GameState->paddle_r_y + (paddle_height/2)) * height_correction,
            0x00FF0000);

    DrawRectangle(Buffer,
            GameState->ball_x * width_correction  - ball_radius * height_correction,
            GameState->ball_y * height_correction - ball_radius * height_correction,
            GameState->ball_x * width_correction  + ball_radius * height_correction,
            GameState->ball_y * height_correction + ball_radius * height_correction,
            0x00FF0000);
}
