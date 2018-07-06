#include "ggo_demeco_bitmap_artist.h"
#include <memory>
#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <raytracer/ggo_scene.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/cameras/ggo_point_camera.h>

//////////////////////////////////////////////////////////////
ggo::demeco_bitmap_artist::demeco_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::demeco_bitmap_artist::render_bitmap(void * buffer) const
{
  //ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::black_32f()));

  //auto cylinder = std::make_shared<ggo::cylinder3d<float>>(ggo::pos3f(0.f, 0.f, 0.f), ggo::pos3f(1.f, 0.f, 0.f), 0.1f);
  //auto objet = scene_builder.add_object(cylinder, ggo::red_32f(), false);
  //objet->set_shading(shading::simple_color);

  //ggo::mono_sampling_point_camera camera(width(), height(), ggo::basis3d_float({ 0.f, 0.f, 20.f }), 0.1f);
  //ggo::mono_sampling_renderer renderer(camera);
  //renderer.render(buffer, width(), height(), line_step(), format(), scene_builder);
}
