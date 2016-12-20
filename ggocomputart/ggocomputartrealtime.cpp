#include <SDL.h>
#include <sstream>
#include <thread>
#include <atomic>
#include <ggo_log.h>
#include <ggo_kernel.h>
#include <ggo_signal.h>
#include <ggo_buffer_paint.h>
#include <ggo_animation_artist_abc.h>
#include <ggo_bitmap_artist_abc.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

//#define DEBUG_ANIM_MULTI_THREADING 1

enum class rendering_state
{
  animation,
  bitmap
};

// Animation.
int anim_workers_ids = 0;
std::mutex anim_mutex;
std::condition_variable anim_condition_start;
std::condition_variable anim_condition_done;
std::unique_ptr<ggo::animation_artist_abc> anim_artist;

// Bitmap.
bool bitmap_rendering_start = false;
bool bitmap_rendering_done = false;
std::mutex bitmap_mutex;
std::condition_variable bitmap_condition_start;
std::unique_ptr<uint8_t[]> bitmap_buffer;

SDL_Window * window = nullptr;
SDL_Surface * screen_surface = nullptr;
const int nominal_frame_duration_ms = 40; // 25 fps
bool quit = false;
bool paused = false;

/////////////////////////////////////////////////////////////////////
ggo::bitmap_artist_abc * create_bitmap_artist()
{
  const std::vector<ggo::bitmap_artist_id> ids{
    //ggo::bitmap_artist_id::julia,
    //ggo::bitmap_artist_id::topodoko,
    //ggo::bitmap_artist_id::mandelbrot,
    //ggo::bitmap_artist_id::distorsion,
    //ggo::bitmap_artist_id::plastic,
    ggo::bitmap_artist_id::rex
  };

  auto index = ggo::rand<size_t>(0, ids.size() - 1);

  std::cout << "Bitmap artist ID: " << index << std::endl;

  return ggo::bitmap_artist_abc::create(ids[index], screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd);
}

/////////////////////////////////////////////////////////////////////
ggo::animation_artist_abc * create_animation_artist()
{
#ifdef GGO_ANDROID
  const std::vector<ggo::animation_artist_id> ids{
    ggo::animation_artist_id::bozons,
    ggo::animation_artist_id::duffing,
    ggo::animation_artist_id::neon,
    ggo::animation_artist_id::kanji
  };
#else
  const std::vector<ggo::animation_artist_id> ids{
    ggo::animation_artist_id::bozons,
    ggo::animation_artist_id::duffing,
    ggo::animation_artist_id::kanji,
    ggo::animation_artist_id::neon,
    ggo::animation_artist_id::storni
  };
#endif

  auto index = ggo::rand<size_t>(0, ids.size() - 1);

  std::cout << "Animation artist ID: " << index << std::endl;

#ifdef GGO_ANDROID
  return ggo::animation_artist_abc::create(ids[index],
    screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd, ggo::animation_artist_abc::realtime_rendering_android);
#else
  return ggo::animation_artist_abc::create(ids[index],
    screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgra_8u_yd, ggo::animation_artist_abc::realtime_rendering_pc);
#endif
}

/////////////////////////////////////////////////////////////////////
void render_anim_func(const ggo::pixel_rect & clipping, int id)
{
  while (true)
  {
    {
      std::unique_lock<std::mutex> lock(anim_mutex);
#ifdef DEBUG_ANIM_MULTI_THREADING
      std::cout << "render " << std::this_thread::get_id() << ": wait for start " << anim_workers_ids << "\n";
#endif
      anim_condition_start.wait(lock, [&] { return (anim_workers_ids & id) != 0 || quit == true; });
#ifdef DEBUG_ANIM_MULTI_THREADING
      std::cout << "render " << std::this_thread::get_id()  << ": starting " << anim_workers_ids << "\n";
#endif
    }

    if (quit == true)
    {
      break;
    }

    anim_artist->render_frame(screen_surface->pixels, clipping);

    {
      std::unique_lock<std::mutex> lock(anim_mutex);
      anim_workers_ids &= ~id;
#ifdef DEBUG_ANIM_MULTI_THREADING
      std::cout << "render " << std::this_thread::get_id() << ": notify done " << anim_workers_ids << "\n";
#endif
    }

    anim_condition_done.notify_one();
  }
}

/////////////////////////////////////////////////////////////////////
void render_bitmap_func()
{
  std::unique_ptr<ggo::bitmap_artist_abc> bitmap_artist;

  bitmap_buffer.reset(new uint8_t[screen_surface->h * screen_surface->pitch]);

  while (true)
  {
    {
      // Wait until main() signals a bitmap to be rendered.
      std::unique_lock<std::mutex> lock(bitmap_mutex);
      bitmap_condition_start.wait(lock, [&] { return bitmap_rendering_start == true || quit == true; });
      bitmap_rendering_start = false;
    }

    if (quit == true)
    {
      break;
    }

    std::cout << "Starting rendering bitmap" << std::endl;
    bitmap_artist.reset(create_bitmap_artist());
    bitmap_artist->render_bitmap(bitmap_buffer.get(), quit);
    std::cout << "Finished rendering bitmap" << std::endl;

    bitmap_rendering_done = true;
  }
}

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
  anim_artist.reset(create_animation_artist());

  int bitmap_counter = 300;
  rendering_state state = rendering_state::animation;

  // Init bitmap rendering.
  std::thread bitmap_thread(render_bitmap_func);
  bitmap_rendering_done = false;
  bitmap_rendering_start = true;
  bitmap_condition_start.notify_one(); // Start rendering bitmap immediatly.

  // Init animation rendering.
  ggo::pixel_rect pixel_rect1 = ggo::pixel_rect::from_width_height(screen_surface->w, screen_surface->h);
  ggo::pixel_rect pixel_rect2 = ggo::pixel_rect::from_width_height(screen_surface->w, screen_surface->h);
  if (screen_surface->w > screen_surface->h)
  {
    pixel_rect1.right() /= 2;
    pixel_rect2.left() = pixel_rect1.right() + 1;
  }
  else
  {
    pixel_rect1.top() /= 2;
    pixel_rect2.bottom() = pixel_rect1.top() + 1;
  }

  std::thread anim_thread1(render_anim_func, pixel_rect1, 1);
  std::thread anim_thread2(render_anim_func, pixel_rect2, 2);

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
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESTORED)
        {
          screen_surface = SDL_GetWindowSurface(window);
        }
        break;
      }
    }

    // Update display.
    switch (state)
    {
    case rendering_state::animation:
      if (anim_artist->update() == true && next == false)
      {
        // Start current frame rendering.
        {
          std::unique_lock<std::mutex> lock(anim_mutex);
#ifdef DEBUG_ANIM_MULTI_THREADING
          std::cout << "main: notify start " << anim_workers_ids << std::endl;
#endif
          anim_workers_ids = 1 | 2;
        }
        anim_condition_start.notify_all();

        // Wait for frame to be rendering by workers threads.
        {
          std::unique_lock<std::mutex> lock(anim_mutex);
#ifdef DEBUG_ANIM_MULTI_THREADING
          std::cout << "main: waiting for done " << anim_workers_ids << "\n";
#endif
          anim_condition_done.wait(lock, [] { return anim_workers_ids == 0 || quit == true; });
#ifdef DEBUG_ANIM_MULTI_THREADING
          std::cout << "main: done " << anim_workers_ids << "\n";
#endif
        }
      }
      else
      {
        if (bitmap_rendering_done == true)
        {
          state = rendering_state::bitmap;
          bitmap_counter = 300;

          memcpy(screen_surface->pixels, bitmap_buffer.get(), screen_surface->h * screen_surface->pitch);

          // Start rendering another bitmap.
          bitmap_rendering_done = false;
          bitmap_rendering_start = true;
          bitmap_condition_start.notify_one();
        }
        else
        {
          std::unique_lock<std::mutex> lock(anim_mutex);
#ifdef DEBUG_ANIM_MULTI_THREADING
          std::cout << "main: resetting artist " << anim_workers_ids << "\n";
#endif
          anim_artist.reset(create_animation_artist());
        }
      }
      break;

    case rendering_state::bitmap:
      --bitmap_counter;
      if (bitmap_counter == 0 || next == true)
      {
        state = rendering_state::animation;

        std::unique_lock<std::mutex> lock(anim_mutex);
        anim_artist.reset(create_animation_artist());
      }
      break;
    }

    // Check FPS.
    auto frame_duration_ms = SDL_GetTicks() - frame_start_time_ms;
    Uint32 delay_ms = 0;

    ggo::paint_rect_safe<ggo::bgra_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      0, screen_surface->w, 0, 10, ggo::black_8u());
    ggo::paint_rect_safe<ggo::bgra_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      0, 100, 0, 10, ggo::white_8u());
    ggo::paint_rect_safe<ggo::bgra_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      0, 100 * frame_duration_ms / nominal_frame_duration_ms, 0, 10, ggo::red_8u());

    if (frame_duration_ms < nominal_frame_duration_ms)
    {
      delay_ms = nominal_frame_duration_ms - frame_duration_ms;
      SDL_Delay(delay_ms);
      frame_duration_ms = nominal_frame_duration_ms;
    }

    SDL_UpdateWindowSurface(window);

    std::ostringstream oss;
    oss << "FPS: " << 1000 / frame_duration_ms << ", CPU: " << ggo::to<int>(100.f * (1.f - float(delay_ms) / float(nominal_frame_duration_ms))) << "%";
    SDL_SetWindowTitle(window, oss.str().c_str());
  }

  bitmap_condition_start.notify_one();
  anim_condition_start.notify_all();

  anim_thread1.join();
  anim_thread2.join();
  bitmap_thread.join();
}

/////////////////////////////////////////////////////////////////////
int SDL_main(int argc, char ** argv)
{
  try
  {
    // Initialize SDL
    const int screen_width = 640;
    const int screen_height = 480;
    const int screen_bpp = 32;

    GGO_SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
    window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
      GGO_SDL_ERROR("Window could not be created");
    }
    
    screen_surface = SDL_GetWindowSurface(window);
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
