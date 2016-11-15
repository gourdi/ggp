#include "ggo_metaballs_artist.h"
#include <iostream>
#include <algorithm>
#include <ggo_object3d.h>
#include <ggo_background3d_color.h>

//#define GGO_METABALLS_DEBUG

//////////////////////////////////////////////////////////////
ggo::metaballs_artist::params::params()
{
	float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
	
	_threshold = ggo::rand<float>(0.8f, 0.9f);
	_phong_factor = ggo::rand<float>(2, 4);
	_phong_shininess = ggo::rand<float>(10, 100);
	_light1 = ggo::pos3f(0.f, 0.f, 1000.f);
	_light2 = ggo::pos3f(1000 * std::cos(angle), 1000 * std::sin(angle), 1000.f);
	_background_color = ggo::color_32f(ggo::rand<float>(0.2f, 0.8f));
}

//////////////////////////////////////////////////////////////
ggo::metaballs_artist::metaballs_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
artist(width, height, line_step, pbf)
{
}	

//////////////////////////////////////////////////////////////
void ggo::metaballs_artist::render_bitmap(void * buffer, ggo::renderer_abc & renderer, const ggo::metaballs_artist::params & params)
{
#ifdef GGO_METABALLS_DEBUG
	std::cout << "Threshold: " << metaball._threshold << std::endl;
#endif

  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(params._background_color));

  auto metaball = std::make_shared<ggo::metaball<float>>(params._threshold);
  for (const auto & center : params._centers)
  {
    std::shared_ptr<ggo::influence_shape3d_abc<float>> shape(new ggo::sphere3d_float(center, 0.7f));
    metaball->add_influence_data(shape, 1.0f);
  }
  auto object = scene_builder.add_object(metaball, params._background_color, true);
  object->set_phong_factor(params._phong_factor);
  object->set_phong_shininess(params._phong_shininess);

	scene_builder.add_sphere_light(ggo::color_32f(0.8f), 0.1f, params._light1);
  scene_builder.add_sphere_light(ggo::color_32f(0.8f), 0.1f, params._light2);

  renderer.render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), scene_builder);
}

