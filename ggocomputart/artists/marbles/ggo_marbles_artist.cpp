#include "ggo_marbles_artist.h"
#include <ggo_scene.h>
#include <ggo_object3d.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_point_camera.h>
#include <ggo_linear_fog.h>
#include <ggo_material_abc.h>
#include <ggo_background3d_color.h>
#include <iostream>
#include <algorithm>

namespace
{
  //////////////////////////////////////////////////////////////
	class my_material : public ggo::material_abc
	{
	public:
		
                my_material(float sphere_size);
		
		ggo::color	get_color(const ggo::point3d_float & pos) const override;
		
	private:
		
		ggo::color	_color1;
		ggo::color	_color2;
		float		    _range_large;
		float		    _range_small;
		float		    _amplitude;
		float		    _wavelength;
	};
  
  //////////////////////////////////////////////////////////////
  my_material::my_material(float sphere_size)
  {
    _color1 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.75, 1), ggo::rand_float(0.5, 1));
    _color2 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.75, 1), ggo::rand_float(0.5, 1));
    _range_large = sphere_size * ggo::rand_float(0.2f, 0.4f);
    _range_small = sphere_size * ggo::rand_float(0.01f, 0.08f);
    _amplitude = sphere_size * ggo::rand_float(0.04f, 0.08f);
    _wavelength = sphere_size * ggo::rand_float(10, 20);
  }

  //////////////////////////////////////////////////////////////
  ggo::color my_material::get_color(const ggo::point3d_float & pos) const
  {
    float x = pos.x() + _amplitude * std::cos(_wavelength * pos.z());
    x = std::fmod(x, _range_large);
    if (x < 0)
    {
      x += _range_large;
    }
    float x_inf = _range_large / 2 - _range_small;
    float x_sup = _range_large / 2 + _range_small;

    return ((x > x_inf) && (x < x_sup)) ? _color1 : _color2;	
  }
}

//////////////////////////////////////////////////////////////
ggo_marbles_artist::ggo_marbles_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_marbles_artist::render_bitmap(uint8_t * buffer)
{
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLACK));

  // The fog.
  scene_builder.set_fog(std::make_shared<ggo::linear_fog>(ggo::color(0.5f), 25.f));

	// Setup the camera.
	ggo::multi_sampling_point_camera camera(get_render_width(), get_render_height());
	camera.basis().set_pos(0, 0, 10);
	camera.basis().rotate(ggo::ray3d_float::O_X(), 1.2f);
	camera.set_aperture(0.2f);
	camera.set_depth_of_field_factor(ggo::rand_float(0.10f, 0.15f));
	camera.set_depth_of_field(ggo::rand_float(7, 9));

	// Floor plane.
  auto floor = scene_builder.add_object(std::make_shared<ggo::plane3d_float>(0.f, 0.f, 1.f, 0.f), ggo::color::WHITE, true);
  floor->set_reflection_factor(0.5f);
  floor->set_roughness(0.1f);

	// Create the spheres.
	std::vector<ggo::sphere3d_float> spheres;
  while (spheres.size() < 128)
	{
		float radius = ggo::rand_float(0.2f, 0.8f);
		ggo::sphere3d_float sphere(ggo::point3d_float(ggo::rand_float(-4, 4), ggo::rand_float(-8, 8), radius), radius);
		
		bool ok = true;
		for (const auto & sphere_cur : spheres)
		{
			if (ggo::distance(sphere_cur.center(), sphere.center()) < sphere_cur.radius() + sphere.radius())
			{
				ok = false;
				break;
			}
		}
		
		if (ok == true)
		{
			spheres.push_back(sphere);
		}
	}

  int lights_count = 0;
  
	for (const auto & sphere : spheres)
	{
    ggo::vector3d_float dir_to_center(sphere.center() - camera.basis().pos());
    dir_to_center.normalize();
    
    if ((lights_count < 3) &&
        (ggo::dot(dir_to_center, camera.basis().z()) < -0.98) && // In the axis of the camera
        (ggo::dot(dir_to_center, camera.basis().y()) < 0.1))     // Center of the sphere is low in the output picture.
    {
      scene_builder.add_sphere_light(ggo::color::WHITE, sphere.radius(), sphere.center());
      ++lights_count;
    }
    else
    {
      auto shape = std::make_shared<ggo::centered_sphere3d_float>(sphere.radius());
      auto material = std::make_shared<const my_material>(sphere.radius());
      auto object = scene_builder.add_object(shape, material, false);

      object->set_phong_factor(ggo::rand_float(3, 5));
      object->set_phong_shininess(ggo::rand_float(250, 500));
      object->basis().rotate_x(ggo::rand_float(0, 2 * ggo::PI<float>()));
      object->basis().rotate_y(ggo::rand_float(0, 2 * ggo::PI<float>()));
      object->basis().rotate_z(ggo::rand_float(0, 2 * ggo::PI<float>()));
      object->basis().pos() = sphere.center();
    }
	}

	// Render.
  ggo::raytrace_params raytrace_params;
  raytrace_params._depth = 2;
  ggo::global_sampling_renderer renderer(camera, 128);
	renderer.render(buffer, get_render_width(), get_render_height(), scene_builder, raytrace_params);
}

