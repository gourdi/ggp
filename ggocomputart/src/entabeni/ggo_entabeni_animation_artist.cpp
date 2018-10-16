#include "ggo_entabeni_animation_artist.h"
#include "ggo_entabeni.h"

//////////////////////////////////////////////////////////////
ggo::entabeni_animation_artist::entabeni_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format, 300)
{
  _z = 0.f;
  _grid_start = ggo::entabeni::create_grid(true, false);
  _grid_end = ggo::entabeni::create_grid(true, false);
  _color_map = ggo::entabeni::create_color_map();
}

//////////////////////////////////////////////////////////////
void ggo::entabeni_animation_artist::render_frame(void * buffer, int frame_index)
{
  // Update displacement.
  _angle = _angle_interpolator.update(1);
  _z -= 0.25f;

  // Interpolate grid.
  ggo::array<float, 2> grid(_grid_start.width(), _grid_start.height());
  float t = static_cast<float>(frame_index) / static_cast<float>(frames_count());

  for (int y = 0; y < grid.height(); ++y)
  {
    for (int x = 0; x < grid.width(); ++x)
    {
      grid(x, y) = (1.f - t) * _grid_start(x, y) + t * _grid_end(x, y);
    }
  }

  // Paint the grid.
  ggo::entabeni::render_bitmap(buffer, width(), height(), line_step(), format(), grid, _color_map, _z, _angle);
}

