#include "ggo_entabeni_animation_artist.h"
#include "ggo_entabeni.h"

//////////////////////////////////////////////////////////////
ggo_entabeni_animation_artist::ggo_entabeni_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_entabeni_animation_artist::init_sub()
{
  _z = 0.f;
  _grid_start = ggo_entabeni::create_grid(true, false);
  _grid_end = ggo_entabeni::create_grid(true, false);
  _color_map = ggo_entabeni::create_color_map();
}

//////////////////////////////////////////////////////////////
bool ggo_entabeni_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  const int frames_count = 300;
  if (frame_index > frames_count)
  {
    return false;
  }

  // Update displacement.
  float angle = _angle.update(1);
  float z = _z;
  _z -= 0.1f;

  // Interpolate grid.
  ggo::array<float, 2> grid(_grid_start.get_size<0>(), _grid_start.get_size<1>());
  float t = static_cast<float>(frame_index) / static_cast<float>(frames_count);
  auto interpolate = [&](int x, int y)
  {
    grid(x, y) = (1.f - t) * _grid_start(x, y) + t * _grid_end(x, y);
  };
  ggo::for_each(grid, interpolate);

  // Paint the grid.
  auto image = make_image_buffer(buffer);
  ggo_entabeni::render_bitmap(image, grid, _color_map, z, angle);

  return true;
}

