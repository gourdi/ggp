#include "ggo_entabeni_animation_artist.h"
#include "ggo_entabeni.h"

namespace
{
  const int frames_count = 300;
}

//////////////////////////////////////////////////////////////
ggo::entabeni_animation_artist::entabeni_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
}

//////////////////////////////////////////////////////////////
void ggo::entabeni_animation_artist::init()
{
  _frame_index = -1;
  _z = 0.f;
  _grid_start = ggo::entabeni::create_grid(true, false);
  _grid_end = ggo::entabeni::create_grid(true, false);
  _color_map = ggo::entabeni::create_color_map();
}

//////////////////////////////////////////////////////////////
bool ggo::entabeni_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index > frames_count)
  {
    return false;
  }

  // Update displacement.
  _angle = _angle_interpolator.update(1);
  _z -= 0.25f;

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::entabeni_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping)
{
  // Interpolate grid.
  ggo::array<float, 2> grid(_grid_start.get_size<0>(), _grid_start.get_size<1>());
  float t = static_cast<float>(_frame_index) / static_cast<float>(frames_count);
  auto interpolate = [&](int x, int y)
  {
    grid(x, y) = (1.f - t) * _grid_start(x, y) + t * _grid_end(x, y);
  };
  ggo::for_each(grid, interpolate);

  // Paint the grid.
  ggo::entabeni::render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), grid, _color_map, _z, _angle);
}

