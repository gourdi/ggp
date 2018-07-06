#include "ggo_cumbia_bitmap_artist.h"
#include "ggo_cumbia_artist.h"
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>

//////////////////////////////////////////////////////////////
ggo::cumbia_bitmap_artist::cumbia_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::basis3d_float camera_basis;
  float aperture = 0.f;
  float depth_of_field = 0.f;
  float depth_of_field_factor = 0.f;

	ggo::cumbia_artist artist;
	artist.init(camera_basis, aperture, depth_of_field, depth_of_field_factor, 1 << 17);

  ggo::multi_sampling_point_camera camera(width(), height(), camera_basis, aperture, depth_of_field, depth_of_field_factor);

  ggo::global_sampling_renderer renderer(camera, 56);
	artist.render_bitmap(buffer, width(), height(), line_step(), format(), renderer);
}

