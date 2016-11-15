#include "ggo_entabeni_animation_artist.h"
#include "ggo_entabeni.h"

//////////////////////////////////////////////////////////////
ggo::entabeni_animation_artist::entabeni_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
animation_artist_abc(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::entabeni_animation_artist::init_sub()
{
  _z = 0.f;
  _grid_start = ggo::entabeni::create_grid(true, false);
  _grid_end = ggo::entabeni::create_grid(true, false);
  _color_map = ggo::entabeni::create_color_map();
}

//////////////////////////////////////////////////////////////
bool ggo::entabeni_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 300;
  if (frame_index > frames_count)
  {
    return false;
  }

  // Update displacement.
  float angle = _angle.update(1);
  float z = _z;
  _z -= 0.25f;

  // Interpolate grid.
  ggo::array<float, 2> grid(_grid_start.get_size<0>(), _grid_start.get_size<1>());
  float t = static_cast<float>(frame_index) / static_cast<float>(frames_count);
  auto interpolate = [&](int x, int y)
  {
    grid(x, y) = (1.f - t) * _grid_start(x, y) + t * _grid_end(x, y);
  };
  ggo::for_each(grid, interpolate);

  // Paint the grid.
  ggo::entabeni::render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), grid, _color_map, z, angle);

  return true;
}

