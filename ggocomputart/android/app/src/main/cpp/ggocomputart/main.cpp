#include <SDL.h>
#include <iostream>
#include <SDL_video.h>

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { /*GGO_SDL_ERROR(zzz);*/ }

int SDL_main(int argc, char *argv[])
{
    //Get device display mode
//    if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 1 )
//    {
//        return 1;
//    }

    GGO_SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window * window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                           0, 0, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        return 1;
    }

    SDL_Surface * screen_surface = SDL_GetWindowSurface(window);
    if (screen_surface == nullptr)
    {
        return 1;
    }

    while (true)
    {
        uint8_t * ptr = static_cast<uint8_t *>(screen_surface->pixels);
        for (int i = 0; i < 2048; ++i) {
            ptr[4 * i + 0] = 0xff;
            ptr[4 * i + 1] = 0x00;
            ptr[4 * i + 2] = 0x00;
            ptr[4 * i + 3] = 0xff;
        }

      SDL_UpdateWindowSurface(window);
    }

    return 0;
}
