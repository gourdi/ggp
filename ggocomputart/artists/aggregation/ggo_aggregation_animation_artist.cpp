#include "ggo_aggregation_animation_artist.h"
#include <ggo_ease.h> 
#include <ggo_fill.h>

namespace
{
  const int FRAMES_COUNT = 300;
}

////////////////////////////////////////////////////////
ggo_aggregation_animation_artist::ggo_aggregation_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_aggregation_animation_artist::init_sub()
{
  _background_gray = ggo::rand_float(0.5f, 0.9f);
  
  float attraction_factor = ggo::rand_float(0.04f, 0.08f);
  float rotation_factor = ggo::rand_float(0.1f, 0.2f);
  ggo::vec3f rotation_vector(ggo::rand_float(-1, 1), ggo::rand_float(-1, 1), ggo::rand_float(-1, 1));
  
  _points = ggo::aggregation_artist::compute_points(attraction_factor, rotation_vector, rotation_factor);
}

//////////////////////////////////////////////////////////////
bool ggo_aggregation_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (frame_index >= FRAMES_COUNT)
  {
    return false;
  }

  int count = ggo::to<int>(ggo::ease_inout(frame_index, FRAMES_COUNT, 0.f, static_cast<float>(_points.size())));
  std::vector<ggo::pos3f> points(_points.begin(), _points.begin() + count);
  
  float angle = ggo::ease_inout(frame_index, FRAMES_COUNT, 0.f, 3.f * ggo::PI<float>());
  
  ggo::aggregation_artist::render(_background_gray, points, angle, buffer, get_render_width(), get_render_height());

	return true;
}

