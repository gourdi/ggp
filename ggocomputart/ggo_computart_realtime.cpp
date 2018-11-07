#include <SDL.h>
#include <sstream>
#include <thread>
#include <atomic>
#include <kernel/ggo_log.h>
#include <kernel/ggo_kernel.h>
#include <kernel/threading/ggo_signal.h>
#include <2d/fill/ggo_fill.h>
#include <ggo_realtime_artist_abc.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

//#define DEBUG_ANIM_MULTI_THREADING 1

// Rendering.
int anim_workers_ids = 0;
std::mutex anim_mutex;
std::condition_variable anim_condition_start;
std::condition_variable anim_condition_done;
std::unique_ptr<ggo::realtime_artist_abc> artist;

SDL_Window * window = nullptr;
SDL_Surface * screen_surface = nullptr;
const int nominal_frame_duration_ms = 40; // 25 fps
bool quit = false;
bool paused = false;

/////////////////////////////////////////////////////////////////////
ggo::realtime_artist_abc * create_artist()
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
    ggo::realtime_artist_id::poupette
  };
#endif

  auto index = ggo::rand<size_t>(0, ids.size() - 1);

  std::cout << "Artist ID: " << index << std::endl;

 return ggo::realtime_artist_abc::create(ids[index], screen_surface->w, screen_surface->h, screen_surface->pitch, ggo::bgrx_8u_yd);
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
    artist->preprocess_frame();
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

    ggo::fill_solid<ggo::bgrx_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      ggo::black_8u(), ggo::rect_int::from_left_right_bottom_top(0, screen_surface->w, 0, 10));
    ggo::fill_solid<ggo::bgrx_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      ggo::white_8u(), ggo::rect_int::from_left_right_bottom_top(0, 100, 0, 10));
    ggo::fill_solid<ggo::bgrx_8u_yd>(screen_surface->pixels, screen_surface->w, screen_surface->h, screen_surface->pitch,
      ggo::red_8u(), ggo::rect_int::from_left_right_bottom_top(0, 100 * frame_duration_ms / nominal_frame_duration_ms, 0, 10));

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
