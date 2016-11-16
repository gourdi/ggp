#include <SDL.h>
#include <sstream>
#include <thread>
#include <atomic>
#include <ggo_log.h>
#include <ggo_kernel.h>
#include <ggo_signal.h>
#include <ggo_animation_artist_abc.h>
#include <ggo_bitmap_artist_abc.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

enum class rendering_state
{
  animation,
  bitmap
};

SDL_Window * window = nullptr;
SDL_Surface * screen_surface = nullptr;
const int nominal_frame_duration_ms = 40; // 25 fps
std::atomic_bool bitmap_rendered = false;
ggo::signal signal;
std::unique_ptr<uint8_t[]> bitmap_buffer;

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
ggo::bitmap_artist_abc * create_bitmap_artist()
{
  const std::vector<ggo::bitmap_artist_id> ids{
    ggo::bitmap_artist_id::julia,
    ggo::bitmap_artist_id::topodoko,
    ggo::bitmap_artist_id::mandelbrot
  };

  auto index = 2;// ggo::rand<size_t>(0, ids.size() - 1);

  std::cout << "Bitmap artist ID: " << index << std::endl;

  return ggo::bitmap_artist_abc::create(ids[index], screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd);
}

/////////////////////////////////////////////////////////////////////
void main_loop()
{
  std::unique_ptr<ggo::animation_artist_abc> animation_artist(ggo::animation_artist_abc::create(
    ggo::animation_artist_id::neon, screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd));

  int bitmap_counter = 300;
  rendering_state state = rendering_state::animation;
  bool quit = false;

  while (quit == false)
  {
    auto frame_start_time_ms = SDL_GetTicks();

    bool next = false;

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
      case SDL_MOUSEBUTTONUP:
        if (event.button.button == 1)
        {
          next = true;
        }
        break;
      }
    }

    // Update display.
    switch (state)
    {
    case rendering_state::animation:
      if (animation_artist->render_next_frame(screen_surface->pixels) == false || next == true)
      {
        if (bitmap_rendered == true)
        {
          state = rendering_state::bitmap;
          bitmap_counter = 300;

          memcpy(screen_surface->pixels, bitmap_buffer.get(), screen_surface->h * screen_surface->pitch);

          // Start rendering another bitmap.
          bitmap_rendered = false;
          signal.notify();
        }
        else
        {
          animation_artist.reset(ggo::animation_artist_abc::create(
            ggo::animation_artist_id::neon, screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd));
        }
      }
      break;

    case rendering_state::bitmap:
      --bitmap_counter;
      if (bitmap_counter == 0 || next == true)
      {
        state = rendering_state::animation;

        animation_artist.reset(ggo::animation_artist_abc::create(
          ggo::animation_artist_id::neon, screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd));
      }
      break;
    }

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
void render_bitmap_func()
{
  std::unique_ptr<ggo::bitmap_artist_abc> bitmap_artist;

  bitmap_buffer.reset(new uint8_t[screen_surface->h * screen_surface->pitch]);

  while (true)
  {
    // Wait until main() signals a bitmap to be rendered.
    signal.wait();

    std::cout << "Starting rendering bitmap" << std::endl;
    bitmap_artist.reset(create_bitmap_artist());
    bitmap_artist->render_bitmap(bitmap_buffer.get());
    std::cout << "Finished rendering bitmap" << std::endl;

    bitmap_artist.reset();

    bitmap_rendered = true;
  }
}

/////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  try
  {
    // Initialize SDL
    const int screen_width = 320;
    const int screen_height = 240;
    const int screen_bpp = 32;

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

    // Init CommputArt.
    std::thread render_bitmap_thread(render_bitmap_func);
    render_bitmap_thread.detach(); // We don't want bitmap rendering to block exit.

    bitmap_rendered = false;
    signal.notify(); // Start rendering bitmap immediatly.

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
