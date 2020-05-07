#include <SDL.h>
#include <iostream>
#include <kernel/ggo_ratio.h>
#include <kernel/ggo_log.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill_solid.h>
#include <animation/ggo_synfig_io.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

SDL_Window* window = nullptr;
constexpr uint32_t time_step_ms = 20; // 25 fps
constexpr ggo::ratio fps{ 1000, time_step_ms }; // 25 fps
bool quit = false;

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
void main_loop()
{
  bool fullscreen = false;

  auto animation = ggo::load_synfig_animation();

  auto run_start_time_ms = SDL_GetTicks();

  SDL_Surface * screen_surface = SDL_GetWindowSurface(window);


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
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_ESCAPE:
          if (fullscreen)
          {
            SDL_SetWindowFullscreen(window, 0);
            fullscreen = false;
            screen_surface = SDL_GetWindowSurface(window);
          }
          else
          {
            quit = true;
          }
          break;
        case SDL_SCANCODE_F:
          SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
          fullscreen = !fullscreen;
          screen_surface = SDL_GetWindowSurface(window);
          break;
        }
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESTORED)
        {
          screen_surface = SDL_GetWindowSurface(window);
        }
        break;
      }
    }

    float elpased_time = (SDL_GetTicks() - run_start_time_ms) / 1000.f;

    ggo::image_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down> image(screen_surface->pixels, { screen_surface->w, screen_surface->h }, screen_surface->pitch);

    ggo::fill_solid(image, ggo::convert_color_to<ggo::rgb_8u>(animation._bkgd_color));

    auto scene = ggo::canvas_at_time(animation, elpased_time, { screen_surface->w, screen_surface->h });
    ggo::paint<ggo::sampling_2x2>(image, scene);

    auto frame_duration_ms = SDL_GetTicks() - frame_start_time_ms;
    Uint32 delay_ms = 0;

    if (frame_duration_ms < time_step_ms)
    {
      delay_ms = time_step_ms - frame_duration_ms;
      SDL_Delay(delay_ms);
      frame_duration_ms = time_step_ms;
    }

    SDL_UpdateWindowSurface(window);

    std::ostringstream oss;
    oss << "Gourdi.net - FPS: " << 1000 / frame_duration_ms << ", CPU: " << ggo::round_to<int>(100.f * (1.f - float(delay_ms) / float(time_step_ms))) << "%";
    SDL_SetWindowTitle(window, oss.str().c_str());
  }
}

/////////////////////////////////////////////////////////////////////
int SDL_main(int argc, char** argv)
{
  try
  {
    // Initialize SDL
    const int screen_width = 1280;
    const int screen_height = 720;

    GGO_SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
    window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
      GGO_SDL_ERROR("Window could not be created");
    }

    SDL_Surface * screen_surface = SDL_GetWindowSurface(window);
    if (screen_surface == nullptr)
    {
      GGO_SDL_ERROR("Failed retrieving window's surface");
    }

    main_loop();
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
