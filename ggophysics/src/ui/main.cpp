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
float              meters_per_pixel = 0.001f;

/////////////////////////////////////////////////////////////////////
void render_physics(const ggo::oriented_box_body & body, const ggo::half_plane<float> & half_plane, SDL_Renderer * renderer)
{
  SDL_CALL(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF));
  SDL_CALL(SDL_RenderClear(renderer));

  SDL_CALL(SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));

  auto map_point = [&](const ggo::point2d_float & p) {
    ggo::point2d_float mapped_point = origin_pixel_pos + p / meters_per_pixel;
    SDL_Point sdl_point;
    sdl_point.x = ggo::to<int>(mapped_point.x());
    sdl_point.y = SCREEN_HEIGHT - ggo::to<int>(mapped_point.y());
    return sdl_point;
  };

  // Paint box.
  auto points = body._box.get_draw_points();

  std::vector<SDL_Point> mapped_points;
  for (const auto & point : points)
  {
    mapped_points.push_back(map_point(point));
  }
  mapped_points.push_back(mapped_points[0]); // Close the loop.

  SDL_CALL(SDL_RenderDrawLines(renderer, &mapped_points[0], static_cast<int>(mapped_points.size())));

  // Paint half-plane.
  mapped_points.clear();
  ggo::vector2d_float ortho_normal(-half_plane.normal().y(), half_plane.normal().x());
  mapped_points.push_back(map_point(half_plane.normal() * half_plane.dist_to_origin() + 1000.f * ortho_normal));
  mapped_points.push_back(map_point(half_plane.normal() * half_plane.dist_to_origin() - 1000.f * ortho_normal));

  SDL_CALL(SDL_RenderDrawLines(renderer, &mapped_points[0], static_cast<int>(mapped_points.size())));

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

    ggo::half_plane_float half_plane({ 0.f, 1.f }, 0.f);

    ggo::oriented_box_body body(ggo::oriented_box_float({ 0.f, 0.2f }, { 5.f, 1.f }, 0.1f, 0.05f));
    body._mass = 1.f;

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
        // User requests quit.
        if (event.type == SDL_QUIT)
        {
          quit = true;
        }
      }

      // Update physics.
      update_physics(body, half_plane, 0.01f);// previous_frame_duration / 1000.f); // Duration of the previous frame (in s).

      // Render data.
      render_physics(body, half_plane, renderer);

      // Frame rate control.
      const int MIN_FRAME_DURATION = 15;
      auto current_frame_duration = SDL_GetTicks() - frame_start_time;
      if (current_frame_duration < MIN_FRAME_DURATION)
      {
        SDL_Delay(MIN_FRAME_DURATION - current_frame_duration);
      }



      SDL_Delay(1000);



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
