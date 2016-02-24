#include <SDL.h>
#include <iostream>
#include <string>
#include <ggo_physics.h>

#define SDL_CALL(zzz) if ((zzz) < 0) { \
                        std::cerr << __FILE__ << ":" << __LINE__ << "\"" << #zzz << "\" failed (" << SDL_GetError() << ")" << std::endl; \
                        throw std::runtime_error(SDL_GetError()); }

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

ggo::point2d_float origin_pixel_pos{ 0.5f * SCREEN_WIDTH, 0.5f * SCREEN_HEIGHT };
float              meters_per_pixel = 0.1f; // 10 cm per pixel.

/////////////////////////////////////////////////////////////////////
void render_physics(const std::vector<ggo::body> & bodies, SDL_Renderer * renderer)
{
  SDL_CALL(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF));
  SDL_CALL(SDL_RenderClear(renderer));

  SDL_CALL(SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));
  for (const auto & body : bodies)
  {
    auto points = body._shape->get_draw_points();

    std::vector<SDL_Point> mapped_points;
    for (const auto & point : points)
    {
      ggo::point2d_float mapped_point = origin_pixel_pos + point / meters_per_pixel;

      mapped_points.push_back({ ggo::to<int>(mapped_point.x()), SCREEN_HEIGHT - ggo::to<int>(mapped_point.y()) });
    }

    // Close the loop.
    if (mapped_points.empty() == false)
    {
      mapped_points.push_back(mapped_points[0]);
    }

    SDL_CALL(SDL_RenderDrawLines(renderer, &mapped_points[0], static_cast<int>(mapped_points.size())));
  }

  SDL_RenderPresent(renderer); // Returns void, so no need for SDL_CALL.
}

/////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  try
  {
    // Initialize SDL
    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    SDL_CALL(SDL_Init(SDL_INIT_VIDEO));
    SDL_CALL(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer));

    std::vector<ggo::body> bodies;
    ggo::body disc;
    disc._shape = std::make_shared<ggo::oriented_box_float>(ggo::point2d_float(0, 0), ggo::vector2d_float(1, 0), 1.f, 0.1f);
    disc._mass = 1.f; // 1kg
    bodies.push_back(disc);

    // Main loop.
    int previous_frame_duration = 0;
    bool quit = false;
    while (quit == false)
    {
      auto frame_start_time = SDL_GetTicks();

      // Handle events on queue.
      SDL_Event event;
      while (SDL_PollEvent(&event) != 0)
      {
        //User requests quit
        if (event.type == SDL_QUIT)
        {
          quit = true;
        }
      }

      // Update physics.
      update_physics(bodies, previous_frame_duration / 1000.f); // Duration of the previous frame (in s).

      // Render data.
      render_physics(bodies, renderer);

      // Frame rate control.
      const int MIN_FRAME_DURATION = 15;
      auto current_frame_duration = SDL_GetTicks() - frame_start_time;
      if (current_frame_duration < MIN_FRAME_DURATION)
      {
        SDL_Delay(MIN_FRAME_DURATION - current_frame_duration);
      }

      previous_frame_duration = SDL_GetTicks() - frame_start_time;
      std::string title = std::string("ggophyscis - ") + std::to_string(previous_frame_duration) + " fps";
      SDL_SetWindowTitle(window, title.c_str());
    }

    // Clean up.
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
  catch (...)
  {
    std::cerr << "Exception detected, execution terminated." << std::endl;
  }

  return 0;
}
