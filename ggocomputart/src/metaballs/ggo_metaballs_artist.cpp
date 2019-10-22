#include "ggo_metaballs_artist.h"
#include <iostream>
#include <algorithm>
#include <raytracer/ggo_scene.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/renderers/ggo_renderer_abc.h>

//#define GGO_METABALLS_DEBUG

//////////////////////////////////////////////////////////////
ggo::metaballs_artist::params::params()
{
	float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
	
	_threshold = ggo::rand<float>(0.8f, 0.9f);
	_phong_factor = ggo::rand<float>(2, 4);
	_phong_shininess = ggo::rand<float>(10, 100);
	_light1 = ggo::pos3_f(0.f, 0.f, 1000.f);
	_light2 = ggo::pos3_f(1000 * std::cos(angle), 1000 * std::sin(angle), 1000.f);
	_background_color = ggo::rgb_32f(ggo::rand<float>(0.2f, 0.8f));
}

//////////////////////////////////////////////////////////////
void ggo::metaballs_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step,
                                          ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
                                          ggo::renderer_abc & renderer, const ggo::metaballs_artist::params & params)
{
#ifdef GGO_METABALLS_DEBUG
	std::cout << "Threshold: " << metaball._threshold << std::endl;
#endif

  ggo::scene scene(std::make_shared<ggo::background3d_color>(params._background_color));

  ggo::metaball<float> metaball(params._threshold);
  for (const auto & center : params._centers)
  {
    std::shared_ptr<ggo::influence_shape3d_abc<float>> shape(new ggo::sphere3d_f(center, 0.7f));
    metaball.add_influence_data(shape, 1.0f);
  }
  constexpr uint32_t flags = ggo::discard_reflection | ggo::discard_basis | ggo::discard_roughness;
  auto & object = scene.add_diffuse_object<flags>(metaball, ggo::solid_color_material(params._background_color));
  object.set_phong(params._phong_factor, params._phong_shininess);

	scene.add_sphere_light(ggo::rgb_32f(0.8f), params._light1, 0.1f);
  scene.add_sphere_light(ggo::rgb_32f(0.8f), params._light2, 0.1f);

  renderer.render(ggo::image(buffer, { width, height }, pixel_type, memory_lines_order, line_byte_step), scene);
}

