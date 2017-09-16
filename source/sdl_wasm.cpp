#include "sdl_common.cpp"
#include <emscripten.h>
// supposedly, this is the right header for _rdtsc, but it doesn't work?
// cannot use _rdtsc here cause wasm doesn't support it afaik
//#include <immintrin.h>


internal SDL_Window *Window;
internal game_input Input[2] = {};
internal game_input *NewInput;
internal game_input *OldInput;
internal SDL_Renderer *Renderer;
internal game_memory GameMemory = {};
internal int GameUpdateHz = 30;
internal real32 TargetSecondsPerFrame = 1.0f / (real32)GameUpdateHz;
internal uint64 LastCounter;
internal uint64 PerfCountFrequency;

void main_loop();

int main( int argc, char* args[] ) {
    SDL_Init( SDL_INIT_VIDEO );
    PerfCountFrequency = SDL_GetPerformanceFrequency();
    Window = SDL_CreateWindow("SDL pong",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_RESIZABLE);
    //Start up SDL and create window
    if( Window ){
        // Create a "Renderer" for our window.
        Renderer = SDL_CreateRenderer(Window,
                                        -1,
                                        SDL_RENDERER_PRESENTVSYNC);

        printf("Refresh rate is %d Hz\n", SDLGetWindowRefreshRate(Window));
        if (Renderer) {
            sdl_window_dimension Dimension = SDLGetWindowDimension(Window);
            SDLResizeTexture(&GlobalBackbuffer, Renderer, Dimension.Width, Dimension.Height);
            int XOffset = 0;
            int YOffset = 0;
            NewInput = &Input[0];
            OldInput = &Input[1];
            NewInput->SecondsToAdvanceOverUpdate = TargetSecondsPerFrame;
            OldInput->SecondsToAdvanceOverUpdate = TargetSecondsPerFrame;
#if HANDMADE_INTERNAL
            // TODO: This will fail gently on 32-bit at the moment, but we should probably fix it.
            void *BaseAddress = (void *)Terabytes(2);
#else
            void *BaseAddress = (void *)(0);
#endif

            GameMemory.PermanentStorageSize = Megabytes(64);
            //GameMemory.TransientStorageSize = Gigabytes(4);
            GameMemory.TransientStorageSize = Megabytes(64);

            uint64 TotalStorageSize = GameMemory.PermanentStorageSize + GameMemory.TransientStorageSize;

            GameMemory.PermanentStorage = mmap(BaseAddress, TotalStorageSize,
                                               PROT_READ | PROT_WRITE,
                                               MAP_ANON | MAP_PRIVATE,
                                               -1, 0);

            Assert(GameMemory.PermanentStorage);

            GameMemory.TransientStorage = (uint8*)(GameMemory.PermanentStorage) + GameMemory.PermanentStorageSize;

            // @TODO:
            // hmh day 018 introduced a fixed framerate (60 or 30 fps), we might wanna do that too?
            LastCounter = SDL_GetPerformanceCounter();

            // this throws an exception to act like a return
            emscripten_set_main_loop(main_loop, GameUpdateHz, 1); /* (fn, fps (0 means auto), simulate_infinite_loop)*/
        } else {
            printf( "Failed to initialize Renderer!\n" );
        }
    } else {
        printf( "Failed to initialize Window!\n" );
    }
    // this is only called if in case of failure
    SDL_Quit();
    return(0);
}
void main_loop(){
    game_controller_input *OldKeyboardController = GetController(OldInput,0);
    game_controller_input *NewKeyboardController = GetController(NewInput,0);
    game_controller_input ZeroController = {};
    *NewKeyboardController = ZeroController;
    for(int ButtonIndex = 0;
        ButtonIndex < ArrayCount(NewKeyboardController->Buttons);
        ++ButtonIndex)
    {
        NewKeyboardController->Buttons[ButtonIndex].EndedDown =
        OldKeyboardController->Buttons[ButtonIndex].EndedDown;
    }
    if(SDLProcessPendingEvents(NewKeyboardController))
    {
        // QUITTING
        // not sure if this is necessary but whatevs
        SDL_Quit();
    return;
    }

    // TODO: Adjust mouse X and Y according to image offset and stretching
    uint32 MouseState = SDL_GetMouseState(&NewInput->MouseX, &NewInput->MouseY);
    NewInput->MouseZ = 0; // TODO(casey): Support mousewheel?
    SDLProcessKeyboardEvent(&NewInput->MouseButtons[0],
            MouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
    SDLProcessKeyboardEvent(&NewInput->MouseButtons[1],
            MouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE));
    SDLProcessKeyboardEvent(&NewInput->MouseButtons[2],
            MouseState & SDL_BUTTON(SDL_BUTTON_RIGHT));
    SDLProcessKeyboardEvent(&NewInput->MouseButtons[3],
            MouseState & SDL_BUTTON(SDL_BUTTON_X1));
    SDLProcessKeyboardEvent(&NewInput->MouseButtons[4],
            MouseState & SDL_BUTTON(SDL_BUTTON_X2));


    game_offscreen_buffer Buffer = {};
    Buffer.Memory = GlobalBackbuffer.Memory;
    Buffer.Width = GlobalBackbuffer.Width;
    Buffer.Height = GlobalBackbuffer.Height;
    Buffer.Pitch = GlobalBackbuffer.Pitch;
    Buffer.BytesPerPixel = GlobalBackbuffer.BytesPerPixel;
    GameUpdateAndRender(&GameMemory, NewInput, &Buffer);

    game_input *Temp = NewInput;
    NewInput = OldInput;
    OldInput = Temp;

    // we could use SDL_Delay() here to enforce a fixed framerate like hmh day 018 does
    // but it's better to use emscripten_set_main_loop for that i guess

    // Get this before SDLUpdateWindow() so that we don't keep missing VBlanks.
    uint64 EndCounter = SDL_GetPerformanceCounter();

    SDLUpdateWindow(Window, Renderer, &GlobalBackbuffer);

    uint64 CounterElapsed = EndCounter - LastCounter;

    real64 MSPerFrame = (((1000.0f * (real64)CounterElapsed) / (real64)PerfCountFrequency));
    real64 FPS = (real64)PerfCountFrequency / (real64)CounterElapsed;

    //printf("%.02fms/f, %.02f/s\n", MSPerFrame, FPS);
    // this really starts eating performance

    LastCounter = EndCounter;
}
