#include <stdint.h>

#define internal static
#define local_persist static
#define global_variable static

#define Pi32 3.14159265359f

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int32 bool32;

typedef float real32;
typedef double real64;

#include "pong.h"
#include "pong.cpp"

// SDL
#if defined __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "sdl_common.h"

global_variable sdl_offscreen_buffer GlobalBackbuffer;

internal debug_read_file_result
DEBUGPlatformReadEntireFile(char *Filename)
{
    debug_read_file_result Result = {};

    int FileHandle = open(Filename, O_RDONLY);
    if(FileHandle == -1)
    {
        return Result;
    }

    struct stat FileStatus;
    if(fstat(FileHandle, &FileStatus) == -1)
    {
        close(FileHandle);
        return Result;
    }
    Result.ContentsSize = SafeTruncateUInt64(FileStatus.st_size);

    Result.Contents = malloc(Result.ContentsSize);
    if(!Result.Contents)
    {
        close(FileHandle);
        Result.ContentsSize = 0;
        return Result;
    }


    uint32 BytesToRead = Result.ContentsSize;
    uint8 *NextByteLocation = (uint8*)Result.Contents;
    while (BytesToRead)
    {
        ssize_t BytesRead = read(FileHandle, NextByteLocation, BytesToRead);
        if (BytesRead == -1)
        {
            free(Result.Contents);
            Result.Contents = 0;
            Result.ContentsSize = 0;
            close(FileHandle);
            return Result;
        }
        BytesToRead -= BytesRead;
        NextByteLocation += BytesRead;
    }

    close(FileHandle);
    return(Result);
}

internal void
DEBUGPlatformFreeFileMemory(void *Memory)
{
    free(Memory);
}

internal bool32
DEBUGPlatformWriteEntireFile(char *Filename, uint32 MemorySize, void *Memory)
{
    int FileHandle = open(Filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (!FileHandle)
        return false;

    uint32 BytesToWrite = MemorySize;
    uint8 *NextByteLocation = (uint8*)Memory;
    while (BytesToWrite)
    {
        ssize_t BytesWritten = write(FileHandle, NextByteLocation, BytesToWrite);
        if (BytesWritten == -1)
        {
            close(FileHandle);
            return false;
        }
        BytesToWrite -= BytesWritten;
        NextByteLocation += BytesWritten;
    }

    close(FileHandle);

    return true;
}

sdl_window_dimension
SDLGetWindowDimension(SDL_Window *Window)
{
    sdl_window_dimension Result;

    SDL_GetWindowSize(Window, &Result.Width, &Result.Height);

    return(Result);
}

internal int
SDLGetWindowRefreshRate(SDL_Window *Window)
{
    SDL_DisplayMode Mode;
    int DisplayIndex = SDL_GetWindowDisplayIndex(Window);
    // If we can't find the refresh rate, we'll return this:
    int DefaultRefreshRate = 60;
    if (SDL_GetDesktopDisplayMode(DisplayIndex, &Mode) != 0)
    {
        return DefaultRefreshRate;
    }
    if (Mode.refresh_rate == 0)
    {
        return DefaultRefreshRate;
    }
    return Mode.refresh_rate;
}

internal real32
SDLGetSecondsElapsed(uint64 OldCounter, uint64 CurrentCounter)
{
    return ((real32)(CurrentCounter - OldCounter) / (real32)(SDL_GetPerformanceFrequency()));
}

internal void
SDLResizeTexture(sdl_offscreen_buffer *Buffer, SDL_Renderer *Renderer, int Width, int Height)
{
    int BytesPerPixel = 4;
    if (Buffer->Memory)
    {
        munmap(Buffer->Memory,
               Buffer->Width * Buffer->Height * BytesPerPixel);
    }
    if (Buffer->Texture)
    {
        SDL_DestroyTexture(Buffer->Texture);
    }
    Buffer->Texture = SDL_CreateTexture(Renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        Width,
                                        Height);
    Buffer->Width = Width;
    Buffer->Height = Height;
    Buffer->BytesPerPixel = BytesPerPixel;
    Buffer->Pitch = Width * BytesPerPixel;
    Buffer->Memory = mmap(0,
                          Width * Height * BytesPerPixel,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1,
                          0);
}

internal void
SDLUpdateWindow(SDL_Window *Window, SDL_Renderer *Renderer, sdl_offscreen_buffer *Buffer)
{
    SDL_UpdateTexture(Buffer->Texture,
                      0,
                      Buffer->Memory,
                      Buffer->Pitch);

    SDL_RenderCopy(Renderer,
                   Buffer->Texture,
                   0,
                   0);

    SDL_RenderPresent(Renderer);
}

internal void
SDLProcessKeyboardEvent(game_button_state *NewState, bool32 IsDown)
{
    Assert(NewState->EndedDown != IsDown);
    NewState->EndedDown = IsDown;
    ++NewState->HalfTransitionCount;
}

internal bool
SDLProcessPendingEvents(game_controller_input *KeyboardController)
{
    bool ShouldQuit = false;
    SDL_Event Event;
    while(SDL_PollEvent(&Event))
    {
        switch(Event.type)
        {
            case SDL_QUIT:
            {
                printf("SDL_QUIT\n");
                ShouldQuit = true;
            } break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                SDL_Keycode KeyCode = Event.key.keysym.sym;
                bool IsDown = (Event.key.state == SDL_PRESSED);
                bool WasDown = false;
                if (Event.key.state == SDL_RELEASED)
                {
                    WasDown = true;
                }
                else if (Event.key.repeat != 0)
                {
                    WasDown = true;
                }

                // NOTE: In the windows version, we used "if (IsDown != WasDown)"
                // to detect key repeats. SDL has the 'repeat' value, though,
                // which we'll use.
                if (Event.key.repeat == 0)
                {
                    if(KeyCode == SDLK_w)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->MoveUp, IsDown);
                    }
                    else if(KeyCode == SDLK_a)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->MoveLeft, IsDown);
                    }
                    else if(KeyCode == SDLK_s)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->MoveDown, IsDown);
                    }
                    else if(KeyCode == SDLK_d)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->MoveRight, IsDown);
                    }
                    else if(KeyCode == SDLK_q)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->LeftShoulder, IsDown);
                    }
                    else if(KeyCode == SDLK_e)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->RightShoulder, IsDown);
                    }
                    else if(KeyCode == SDLK_UP)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->ActionUp, IsDown);
                    }
                    else if(KeyCode == SDLK_LEFT)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->ActionLeft, IsDown);
                    }
                    else if(KeyCode == SDLK_DOWN)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->ActionDown, IsDown);
                    }
                    else if(KeyCode == SDLK_RIGHT)
                    {
                        SDLProcessKeyboardEvent(&KeyboardController->ActionRight, IsDown);
                    }
                    else if(KeyCode == SDLK_ESCAPE)
                    {
                    }
                    else if(KeyCode == SDLK_SPACE)
                    {
                    }
                }

                bool AltKeyWasDown = (Event.key.keysym.mod & KMOD_ALT);
                if (KeyCode == SDLK_F4 && AltKeyWasDown)
                {
                    ShouldQuit = true;
                }

            } break;

            case SDL_WINDOWEVENT:
            {
                switch(Event.window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        SDL_Window *Window = SDL_GetWindowFromID(Event.window.windowID);
                        SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                        //printf("SDL_WINDOWEVENT_SIZE_CHANGED (%d, %d)\n", Event.window.data1, Event.window.data2);
                    } break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        //printf("SDL_WINDOWEVENT_FOCUS_GAINED\n");
                    } break;

                    case SDL_WINDOWEVENT_EXPOSED:
                    {
                        SDL_Window *Window = SDL_GetWindowFromID(Event.window.windowID);
                        SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                        SDLUpdateWindow(Window, Renderer, &GlobalBackbuffer);
                    } break;
                }
            } break;
        }

    }
    return(ShouldQuit);
}
