#include <SDL.h>
#include <sstream>
#include <thread>
#include <atomic>
#include <ggo_log.h>
#include <ggo_kernel.h>
#include <ggo_signal.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_animation_artist_abc.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

//#define DEBUG_ANIM_MULTI_THREADING 1

// Animation.
int anim_workers_ids = 0;
std::mutex anim_mutex;
std::condition_variable anim_condition_start;
std::condition_variable anim_condition_done;
std::unique_ptr<ggo::animation_artist_abc> anim_artist;

SDL_Window * window = nullptr;
SDL_Surface * screen_surface = nullptr;
const int nominal_frame_duration_ms = 40; // 25 fps
bool quit = false;
bool paused = false;

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
void render_anim_func(const ggo::rect_int & clipping, int id)
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

  // Init animation rendering.
  ggo::rect_int pixel_rect1 = ggo::rect_int::from_width_height(screen_surface->w, screen_surface->h);
  ggo::rect_int pixel_rect2 = ggo::rect_int::from_width_height(screen_surface->w, screen_surface->h);
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
    if (anim_artist->prepare_frame() == true && next == false)
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

      // Wait for frame to be rendered by workers threads.
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
      std::unique_lock<std::mutex> lock(anim_mutex);
#ifdef DEBUG_ANIM_MULTI_THREADING
      std::cout << "main: resetting artist " << anim_workers_ids << "\n";
#endif
      anim_artist.reset(create_animation_artist());
    }

    // Check FPS.
    auto frame_duration_ms = SDL_GetTicks() - frame_start_time_ms;
    Uint32 delay_ms = 0;

    ggo::paint_rect<ggo::bgra_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      0, screen_surface->w, 0, 10, ggo::black_8u());
    ggo::paint_rect<ggo::bgra_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      0, 100, 0, 10, ggo::white_8u());
    ggo::paint_rect<ggo::bgra_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      0, 100 * frame_duration_ms / nominal_frame_duration_ms, 0, 10, ggo::red_8u());

    if (frame_duration_ms < nominal_frame_duration_ms)
    {
      delay_ms = nominal_frame_duration_ms - frame_duration_ms;
      SDL_Delay(delay_ms);
      frame_duration_ms = nominal_frame_duration_ms;
    }

    SDL_UpdateWindowSurface(window);

    std::ostringstream oss;
    oss << "FPS: " << 1000 / frame_duration_ms << ", CPU: " << ggo::round_to<int>(100.f * (1.f - float(delay_ms) / float(nominal_frame_duration_ms))) << "%";
    SDL_SetWindowTitle(window, oss.str().c_str());
  }

  anim_condition_start.notify_all();

  anim_thread1.join();
  anim_thread2.join();
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