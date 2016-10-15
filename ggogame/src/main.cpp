#include <SDL.h>
#include <ggo_log.h>
#include <ggo_kernel.h>
#include <ggo_fill.h>
#include <ggo_hard_paint.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

const int screen_width = 640;
const int screen_height = 480;
const int screen_bpp = 32;
SDL_Window* window = nullptr;

/////////////////////////////////////////////////////////////////////
void finalize_sdl()
{
  if (window != nullptr)
  {
    SDL_DestroyWindow(window);
  }
  SDL_Quit();
}

/////////////////////////////////////////////////////////////////////
void main_loop(SDL_Surface* screen_surface)
{
  bool quit = false;
  while (quit == false)
  {
    // Handle events on queue.
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
      // User requests quit.
      switch (event.type)
      {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
          quit = true;
        }
        break;
      }
    }

    // Update display.
    ggo::fill_solid_bgra_8u(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::color::RED.color_8u());
    ggo::paint_rect_bgra_8u_yd_fast(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch, 0, 10, 0, 10, ggo::color::BLUE.color_8u());
    ggo::paint_line_bgra_8u_yd(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      0.f, 0.f, screen_surface->w - 1.f, screen_surface->h - 1.f, ggo::color::GREEN.color_8u());
    SDL_UpdateWindowSurface(window);
    SDL_Delay(200);
  }
}

/////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  SDL_Surface* screen_surface = nullptr;

  try
  {
    // Initialize SDL
    GGO_SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
      GGO_SDL_ERROR("Window could not be created");
    }
    
    screen_surface = SDL_GetWindowSurface(window);
    if (screen_surface == nullptr)
    {
      GGO_SDL_ERROR("Failed retrieving window's surface");
    }

    main_loop(screen_surface);
  }
  catch (...)
  {
    std::cerr << "Exception detected, execution terminated." << std::endl;
    finalize_sdl();
    return 1;
  }

  finalize_sdl();
  return 0;
}