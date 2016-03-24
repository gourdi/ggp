#include <SDL.h>
#include <iostream>
#include <string>
#include <ggo_physics.h>

#define SDL_CALL(zzz) if ((zzz) < 0) { \
                        std::cerr << __FILE__ << ":" << __LINE__ << "\"" << #zzz << "\" failed (" << SDL_GetError() << ")" << std::endl; \
                        throw std::runtime_error(SDL_GetError()); }

const int screen_width = 640;
const int screen_height = 480;
const float attenuation = 0.999f;

ggo::point2d_float origin_pixel_pos{ 0.5f * screen_width, 0.5f * screen_height };
float              meters_per_pixel = 0.001f;

std::vector<std::vector<ggo::oriented_box_body>> history;

/////////////////////////////////////////////////////////////////////
void render_physics(const std::vector<ggo::oriented_box_body> & bodies, const std::vector<ggo::half_plane<float>> & half_planes, SDL_Renderer * renderer)
{
  SDL_CALL(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF));
  SDL_CALL(SDL_RenderClear(renderer));

  SDL_CALL(SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));

  auto map_point = [&](const ggo::point2d_float & p) {
    ggo::point2d_float mapped_point = origin_pixel_pos + p / meters_per_pixel;
    SDL_Point sdl_point;
    sdl_point.x = ggo::to<int>(mapped_point.x());
    sdl_point.y = screen_height - ggo::to<int>(mapped_point.y());
    return sdl_point;
  };

  // Paint box.
  for (const auto & body : bodies)
  {
    std::vector<SDL_Point> mapped_points;

    auto points = body._box.get_points();
    for (const auto & point : points)
    {
      mapped_points.push_back(map_point(point));
    }
    mapped_points.push_back(mapped_points[0]); // Close the loop.

    SDL_CALL(SDL_RenderDrawLines(renderer, &mapped_points[0], static_cast<int>(mapped_points.size())));
  }

  // Paint half-plane.
  for (const auto & half_plane : half_planes)
  {
    std::vector<SDL_Point> mapped_points;
    ggo::vector2d_float ortho_normal(-half_plane.normal().y(), half_plane.normal().x());
    mapped_points.push_back(map_point(half_plane.normal() * half_plane.dist_to_origin() + 1000.f * ortho_normal));
    mapped_points.push_back(map_point(half_plane.normal() * half_plane.dist_to_origin() - 1000.f * ortho_normal));

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
    SDL_CALL(SDL_CreateWindowAndRenderer(screen_width, screen_height, SDL_WINDOW_SHOWN, &window, &renderer));

    std::vector<ggo::half_plane_float> half_planes{
      { { 0.f, 1.f }, -0.2f },
      { { 0.f, -1.f }, -0.2f },
      { { 1.f, 0.f }, -0.25f },
      { { -1.f, 0.f }, -0.25f } };

    ggo::oriented_box_body body1(ggo::oriented_box_float({ 0.0f, 0.1f }, { 5.f, 1.f }, 0.1f, 0.02f));
    body1._mass = 1.f;
    body1._linear_velocity = { 0.15f, -0.1f };

    ggo::oriented_box_body body2(ggo::oriented_box_float({ 0.2f, 0.0f }, { 0.f, 1.f }, 0.1f, 0.02f));
    body2._mass = 1.f;
    body2._linear_velocity = { 0.0f, 0.f };

    std::vector<ggo::oriented_box_body> bodies{ body1, body2 };

    // Main loop.
    int previous_frame_duration = 0;
    bool quit = false;
    bool paused = false;
    while (quit == false)
    {
      auto frame_start_time = SDL_GetTicks();

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
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode)
          {
          case SDL_SCANCODE_SPACE:
            paused = !paused;
            break;
          case SDL_SCANCODE_RIGHT:
            if (paused == true)
            {
              update_physics(bodies, half_planes, 0.01f, attenuation);
              render_physics(bodies, half_planes, renderer);
              history.push_back(bodies);
            }
            break;
          case SDL_SCANCODE_LEFT:
            if (paused == true)
            {
              history.pop_back();
              bodies = history.back();
              render_physics(bodies, half_planes, renderer);
            }
            break;
          }
          break;
        }
      }

      // Update physics.
      if (paused == false)
      {
        update_physics(bodies, half_planes, 0.01f, attenuation);// previous_frame_duration / 1000.f); // Duration of the previous frame (in s).
        history.push_back(bodies);
      }

      // Render data.
      render_physics(bodies, half_planes, renderer);

      // Frame rate control.
      const int MIN_FRAME_DURATION = 15;
      auto current_frame_duration = SDL_GetTicks() - frame_start_time;
      if (current_frame_duration < MIN_FRAME_DURATION)
      {
        SDL_Delay(MIN_FRAME_DURATION - current_frame_duration);
      }



      //SDL_Delay(250);



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
