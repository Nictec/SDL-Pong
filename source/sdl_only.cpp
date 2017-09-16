/*
 * Platform file for pure sdl
 * SDL Pong by deltragon.
 * Inspired by Handmade Hero
 *
 *
 *
 */
#include "sdl_common.cpp"

int main( int argc, char* args[] ) {
    SDL_Init( SDL_INIT_VIDEO );
    uint64 PerfCountFrequency = SDL_GetPerformanceFrequency();
    SDL_Window *Window = SDL_CreateWindow("SDL pong",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_RESIZABLE);
    //Start up SDL and create window
    if( Window ){
        // Create a "Renderer" for our window.
        SDL_Renderer *Renderer = SDL_CreateRenderer(Window,
                                                    -1,
                                                    SDL_RENDERER_PRESENTVSYNC);
        if (Renderer) {
            bool Running = true;
            sdl_window_dimension Dimension = SDLGetWindowDimension(Window);
            SDLResizeTexture(&GlobalBackbuffer, Renderer, Dimension.Width, Dimension.Height);
            int XOffset = 0;
            int YOffset = 0;
            game_input Input[2] = {};
            game_input *NewInput = &Input[0];
            game_input *OldInput = &Input[1];
#if HANDMADE_INTERNAL
            // TODO: This will fail gently on 32-bit at the moment, but we should probably fix it.
            void *BaseAddress = (void *)Terabytes(2);
#else
            void *BaseAddress = (void *)(0);
#endif

            game_memory GameMemory = {};
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

            uint64 LastCounter = SDL_GetPerformanceCounter();
            while(Running){
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
                    Running = false;
                }
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


                uint64 EndCounter = SDL_GetPerformanceCounter();

                SDLUpdateWindow(Window, Renderer, &GlobalBackbuffer);

                uint64 CounterElapsed = EndCounter - LastCounter;

                real64 MSPerFrame = (((1000.0f * (real64)CounterElapsed) / (real64)PerfCountFrequency));
                real64 FPS = (real64)PerfCountFrequency / (real64)CounterElapsed;

                printf("%.02fms/f, %.02f/s\n", MSPerFrame, FPS);

                LastCounter = EndCounter;

            }
        } else {
            printf( "Failed to initialize Renderer!\n" );
        }
    } else {
        printf( "Failed to initialize Window!\n" );
    }
    SDL_Quit();
    return(0);

}
