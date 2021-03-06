#include <SDL.h>
#include <sstream>
#include <thread>
#include <atomic>
#include <kernel/ggo_log.h>
#include <kernel/ggo_kernel.h>
#include <kernel/threading/ggo_signal.h>
#include <2d/ggo_image.h>
#include <2d/fill/ggo_fill.h>
#include <ggo_realtime_artist.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

//#define DEBUG_ANIM_MULTI_THREADING 1

// Rendering.
int anim_workers_ids = 0;
std::mutex anim_mutex;
bool display_cpu_usage = true;
std::condition_variable anim_condition_start;
std::condition_variable anim_condition_done;
std::unique_ptr<ggo::realtime_artist> artist;

SDL_Window * window = nullptr;
SDL_Surface * screen_surface = nullptr;
constexpr uint32_t time_step_ms = 40; // 25 fps
constexpr ggo::ratio fps{ 1000, time_step_ms }; // 25 fps
bool quit = false;

/////////////////////////////////////////////////////////////////////
ggo::realtime_artist * create_artist()
{
#ifdef GGO_ANDROID
  const std::vector<ggo::realtime_artist_id> ids{
    ggo::realtime_artist_id::bozons,
    ggo::realtime_artist_id::duffing,
    ggo::realtime_artist_id::neon,
    ggo::realtime_artist_id::kanji
  };
#else
  const std::vector<ggo::realtime_artist_id> ids{
    //ggo::realtime_artist_id::bozons,
    //ggo::realtime_artist_id::duffing,
    //ggo::realtime_artist_id::kanji,
    //ggo::realtime_artist_id::neon,
    //ggo::realtime_artist_id::storni,
    //ggo::realtime_artist_id::lagaude,
    //ggo::realtime_artist_id::demeco,
    //ggo::realtime_artist_id::wakenda,
    //ggo::realtime_artist_id::poupette,
    ggo::realtime_artist_id::sonson,
    //ggo::realtime_artist_id::badaboum
  };
#endif

  int index = ggo::rand<int>(0, int(ids.size() - 1));

  std::cout << "Artist ID: " << index << std::endl;

  return ggo::realtime_artist::create(ids[index], screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::pixel_type::bgrx_8u, ggo::lines_order::down, fps);
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

    artist->render_tile(screen_surface->pixels, clipping);

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
  artist.reset(create_artist());

  // Init rendering.
  const std::vector<ggo::rect_int> tiles{
    ggo::rect_int::from_left_right_bottom_top(0, screen_surface->w / 2 - 1, 0, screen_surface->h / 2 - 1),
    ggo::rect_int::from_left_right_bottom_top(screen_surface->w / 2, screen_surface->w - 1, 0, screen_surface->h / 2 - 1),
    ggo::rect_int::from_left_right_bottom_top(0, screen_surface->w / 2 - 1, screen_surface->h / 2, screen_surface->h - 1),
    ggo::rect_int::from_left_right_bottom_top(screen_surface->w / 2, screen_surface->w - 1, screen_surface->h / 2, screen_surface->h - 1) };

  std::vector<std::thread> worker_threads;
  int threads_mask = 0;
  int thread_id = 1;
  for (const auto & tile : tiles)
  {
    threads_mask |= thread_id;
    worker_threads.emplace_back(render_anim_func, tile, thread_id);
    thread_id <<= 1;
  }

  ggo::pos2_i cursor_pos;
  SDL_GetMouseState(&cursor_pos.x(), &cursor_pos.y());
  bool fullscreen = false;

  while (quit == false)
  {
    auto frame_start_time_ms = SDL_GetTicks();

    bool next = false;
    uint32_t cursor_flags = 0;

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
        case SDL_SCANCODE_SPACE:
          next = true;
          break;
        case SDL_SCANCODE_F:
          SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
          fullscreen = !fullscreen;
          screen_surface = SDL_GetWindowSurface(window);
          break;
        }
        break;
      case SDL_MOUSEMOTION:
        cursor_pos.x() = event.motion.x;
        cursor_pos.y() = event.motion.y;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == 1)
        {
          cursor_flags |= ggo::cursor_event_down;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if (event.button.button == 1)
        {
          cursor_flags |= ggo::cursor_event_up;
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
    artist->preprocess_frame(screen_surface->pixels, cursor_flags, { cursor_pos.x(), screen_surface->h - cursor_pos.y() - 1 });
    {
      // Start current frame rendering.
      {
        std::unique_lock<std::mutex> lock(anim_mutex);
#ifdef DEBUG_ANIM_MULTI_THREADING
        std::cout << "main: notify start " << anim_workers_ids << std::endl;
#endif
        anim_workers_ids = threads_mask;
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

    if (artist->finished() == true || next == true)
    {
      std::unique_lock<std::mutex> lock(anim_mutex);
#ifdef DEBUG_ANIM_MULTI_THREADING
      std::cout << "main: resetting artist " << anim_workers_ids << "\n";
#endif
      artist.reset(create_artist());
    }

    // Check FPS.
    auto frame_duration_ms = SDL_GetTicks() - frame_start_time_ms;
    Uint32 delay_ms = 0;

    if (display_cpu_usage == true)
    {
      ggo::image_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down> img(screen_surface->pixels, { screen_surface->w, screen_surface->h }, screen_surface->pitch);

      ggo::fill_solid(img, ggo::black_8u(), ggo::rect_int::from_left_right_bottom_top(0, screen_surface->w, 0, 10));
      ggo::fill_solid(img, ggo::white_8u(), ggo::rect_int::from_left_right_bottom_top(0, 100, 0, 10));
      ggo::fill_solid(img, ggo::red_8u(), ggo::rect_int::from_left_right_bottom_top(0, 100 * frame_duration_ms / time_step_ms, 0, 10));
    }

    if (frame_duration_ms < time_step_ms)
    {
      delay_ms = time_step_ms- frame_duration_ms;
      SDL_Delay(delay_ms);
      frame_duration_ms = time_step_ms;
    }

    SDL_UpdateWindowSurface(window);

    std::ostringstream oss;
    oss << "Gourdi.net - FPS: " << 1000 / frame_duration_ms << ", CPU: " << ggo::round_to<int>(100.f * (1.f - float(delay_ms) / float(time_step_ms))) << "%";
    SDL_SetWindowTitle(window, oss.str().c_str());
  }

  anim_condition_start.notify_all();

  for (auto & worker_thread : worker_threads)
  {
    worker_thread.join();
  }
}

/////////////////////////////////////////////////////////////////////
int SDL_main(int argc, char ** argv)
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
