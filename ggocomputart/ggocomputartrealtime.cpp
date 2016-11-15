#include <SDL.h>
#include <sstream>
#include <ggo_log.h>
#include <ggo_kernel.h>
#include <ggo_animation_artist_abc.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

const int screen_width = 640;
const int screen_height = 480;
const int screen_bpp = 32;
SDL_Window * window = nullptr;
const int nominal_frame_duration_ms = 40; // 25 fps
std::unique_ptr<ggo::animation_artist_abc> artist;

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
    auto frame_start_time_ms = SDL_GetTicks();

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
    artist->render_next_frame(screen_surface->pixels);

    SDL_UpdateWindowSurface(window);

    // Check FPS.
    auto frame_duration_ms = SDL_GetTicks() - frame_start_time_ms;

    if (frame_duration_ms < nominal_frame_duration_ms)
    {
      SDL_Delay(nominal_frame_duration_ms - frame_duration_ms);
      frame_duration_ms = nominal_frame_duration_ms;
    }

    std::ostringstream oss;
    oss << "FPS: " << 1000 / frame_duration_ms;
    SDL_SetWindowTitle(window, oss.str().c_str());
  }
}

/////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  SDL_Surface * screen_surface = nullptr;

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

    artist = std::unique_ptr<ggo::animation_artist_abc>(ggo::animation_artist_abc::create(
      ggo::animation_artist_id::neon, screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd));

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
