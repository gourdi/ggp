#include "ggo_marbles_artist.h"
#include <raytracer/ggo_scene.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/objects3d/volumetric_objects/ggo_linear_fog.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <iostream>
#include <algorithm>

namespace
{
  //////////////////////////////////////////////////////////////
	class my_material : public ggo::material_abc
	{
	public:
		
                    my_material(float sphere_size);
		
		ggo::rgb_32f    get_color(const ggo::pos3_f & pos) const override;
		
	private:
		
		ggo::rgb_32f	  _color1;
		ggo::rgb_32f	  _color2;
		float		        _range_large;
		float		        _range_small;
		float		        _amplitude;
		float		        _wavelength;
	};
  
  //////////////////////////////////////////////////////////////
  my_material::my_material(float sphere_size)
  {
    _color1 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.75, 1), ggo::rand<float>(0.5, 1));
    _color2 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.75, 1), ggo::rand<float>(0.5, 1));
    _range_large = sphere_size * ggo::rand<float>(0.2f, 0.4f);
    _range_small = sphere_size * ggo::rand<float>(0.01f, 0.08f);
    _amplitude = sphere_size * ggo::rand<float>(0.04f, 0.08f);
    _wavelength = sphere_size * ggo::rand<float>(10, 20);
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f my_material::get_color(const ggo::pos3_f & pos) const
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
ggo::marbles_artist::marbles_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
}

//////////////////////////////////////////////////////////////
void ggo::marbles_artist::render_bitmap(void * buffer) const
{
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::black<ggo::rgb_32f>()));

  // The fog.
  scene.emplace_volumetric_object<ggo::linear_fog>(ggo::rgb_32f(0.5f), 0.f, 25.f);

	// Setup the camera.
  ggo::basis3d_f camera_basis({ 0.f, 0.f, 10.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 1.2f);
	ggo::multi_sampling_point_camera camera(width(), height(), camera_basis, 0.2f, ggo::rand<float>(7, 9), ggo::rand<float>(0.10f, 0.15f));

	// Floor plane.
  constexpr uint32_t flags = ggo::discard_basis | ggo::discard_phong;
  auto & floor = scene.add_diffuse_object<flags>(ggo::const_plane3d<float, 0, 0, 1, 0>(), ggo::white_material()); // z = 0
  floor.set_reflection_factor(0.5f);
  floor.set_roughness(0.1f);

	// Create the spheres.
	std::vector<ggo::sphere3d_f> spheres;
  while (spheres.size() < 128)
	{
		float radius = ggo::rand<float>(0.2f, 0.8f);
		ggo::sphere3d_f sphere(ggo::pos3_f(ggo::rand<float>(-4, 4), ggo::rand<float>(-8, 8), radius), radius);
		
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
    ggo::vec3_f dir_to_center(normalize(sphere.center() - camera.basis().pos()));
    
    if ((lights_count < 3) &&
        (ggo::dot(dir_to_center, camera.basis().z()) < -0.98) && // In the axis of the camera
        (ggo::dot(dir_to_center, camera.basis().y()) < 0.1))     // Center of the sphere is low in the output picture.
    {
      scene.add_sphere_light(ggo::white_32f(), sphere.center(), sphere.radius());
      ++lights_count;
    }
    else
    {
      const uint32_t flags = ggo::discard_reflection | ggo::discard_roughness;
      auto & object = scene.add_diffuse_object<flags>(ggo::centered_sphere3d_f(sphere.radius()), my_material(sphere.radius()));

      object.set_phong(ggo::rand<float>(3, 5), ggo::rand<float>(250, 500));
      object.basis().rotate_x(ggo::rand<float>(0, 2 * ggo::pi<float>()));
      object.basis().rotate_y(ggo::rand<float>(0, 2 * ggo::pi<float>()));
      object.basis().rotate_z(ggo::rand<float>(0, 2 * ggo::pi<float>()));
      object.basis().pos() = sphere.center();
    }
	}

	// Render.
  ggo::raytrace_params raytrace_params;
  raytrace_params._depth = 2;
  ggo::global_sampling_renderer renderer(camera, 128);
	renderer.render(image(buffer, size(), pixel_type(), memory_lines_order(), line_byte_step()), scene, raytrace_params);
}

