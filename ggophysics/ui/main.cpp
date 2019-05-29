#include <SDL.h>
#include <iostream>
#include <string>
#include <kernel/ggo_log.h>
#include <physics/ggo_physics_world.h>

#define GGO_SDL_ERROR(zzz) GGO_LOG_ERROR("\"" << #zzz << "\" failed (error:" << SDL_GetError() << ")"); throw std::runtime_error(SDL_GetError());

#define GGO_SDL_CALL(zzz) if ((zzz) < 0) { GGO_SDL_ERROR(zzz); }

/////////////////////////////////////////////////////////////////////
int SDL_main(int argc, char ** argv)
{
  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;
  constexpr float time_step = 1.0f / 60.0f;
  constexpr Uint32 time_step_ms = ggo::round_to<Uint32>(1000 * time_step);

  try
  {
    // Initialize SDL
    const int screen_width = 1280;
    const int screen_height = 720;
    GGO_SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
    GGO_SDL_CALL(SDL_CreateWindowAndRenderer(screen_width, screen_height, SDL_WINDOW_SHOWN, &window, &renderer));

    // Physics world.
    ggo::physics_world world(100);
    world.set_external_force([](const ggo::rigid_body& body)
    {
      return ggo::vec2_f(0.f, -10.f);
    });

    constexpr ggo::pos2_f center_pos{ 0.5f * screen_width, 0.1f * screen_height };
    constexpr float pixel_size = 0.05f;

    auto floor = std::make_unique<ggo::rigid_body>(50.f, 10.f, 0.f, 1.f);
    floor->_position = { 0.f, -10.f };
    world.add(std::move(floor));

    floor = std::make_unique<ggo::rigid_body>(0.5f, 100.f, 0.f, 1.f);
    floor->_position = { -25.f, 5.f };
    floor->_rotation = 0.6f;
    world.add(std::move(floor));

    floor = std::make_unique<ggo::rigid_body>(0.5f, 100.f, 0.f, 1.f);
    floor->_position = { 25.f, 5.f };
    floor->_rotation = -0.6f;
    world.add(std::move(floor));

    floor = std::make_unique<ggo::rigid_body>(8.0f, 0.5f, 0.f, 1.f);
    floor->_position = { -4.f, 8.f };
    floor->_rotation = -0.5f;
    world.add(std::move(floor));

    floor = std::make_unique<ggo::rigid_body>(8.0f, 0.5f, 0.f, 1.f);
    floor->_position = { 4.f, 18.f };
    floor->_rotation = 0.7f;
    world.add(std::move(floor));

    for (int i = 0; i < 200; ++i)
    {
      auto body = std::make_unique<ggo::rigid_body>(ggo::rand(0.1f, 1.f), ggo::rand(0.1f, 1.f), 200.f, 0.1f);
      body->_position = { ggo::rand(-15.f, 15.f), ggo::rand(10.f, 200.f) };
      world.add(std::move(body));
    }


    // Main loop.
    bool quit = false;
    while (quit == false)
    {
      auto frame_start_time_ms = SDL_GetTicks();

      // Handle queue events.
      SDL_Event event;
      while (SDL_PollEvent(&event) != 0)
      {
        switch (event.type)
        {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode)
          {
          case SDL_SCANCODE_ESCAPE:
            quit = true;
            break;
          }
          break;
        }
      }

      // Paint.
      auto map_pos = [&](ggo::pos2_f p)
      {
        SDL_Point mapped_p;

        mapped_p.x = ggo::round_to<int>(center_pos.x() + p.x() / pixel_size);
        mapped_p.y = screen_height - ggo::round_to<int>(center_pos.y() + p.y() / pixel_size);

        return mapped_p;
      };

      GGO_SDL_CALL(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));
      GGO_SDL_CALL(SDL_RenderClear(renderer));

      GGO_SDL_CALL(SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));
      auto paint_box = [&](const ggo::rigid_body & body)
      {
        auto points = body.box().get_points();
        std::array<SDL_Point, 5> mapped_points;
        for (int i = 0; i < 4; ++i)
        {
          mapped_points[i] = map_pos(points[i]);
        }
        mapped_points[4] = mapped_points[0]; // Close the loop.
        GGO_SDL_CALL(SDL_RenderDrawLines(renderer, mapped_points.data(), 5));
      };
      world.for_each_body(paint_box);

      GGO_SDL_CALL(SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF));
      auto paint_contact = [&](const ggo::contact & contact)
      {
        for (int i = 0; i < contact._points_count; ++i)
        {
          SDL_Point mapped_point = map_pos(contact._points[i]._vertex._pos);
          SDL_Rect rect;
          rect.x = mapped_point.x - 2;
          rect.y = mapped_point.y - 2;
          rect.w = 5;
          rect.h = 5;
          SDL_RenderFillRect(renderer, &rect);
        }
      };
      world.for_each_contact(paint_contact);

      SDL_RenderPresent(renderer); // Returns void, so no need for SDL_CALL.

      // Update physics.
      world.update(time_step);

      // Check FPS.
      auto frame_duration_ms = SDL_GetTicks() - frame_start_time_ms;
      if (frame_duration_ms < time_step_ms)
      {
        Uint32 delay_ms = time_step_ms - frame_duration_ms;
        SDL_Delay(delay_ms);
      }
    }
  }
  catch (...)
  {
    std::cerr << "Exception detected, execution terminated." << std::endl;
  }

  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
