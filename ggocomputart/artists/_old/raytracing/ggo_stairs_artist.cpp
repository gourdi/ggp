#include "ggo_stairs_artist.h"
#include <ggo_dct.h>
#include <ggo_object3d.h>
using namespace std;

#define	GGO_CUBES_HALF_COUNT	25
#define	GGO_UNIVERSE_SIZE		175
#define GGO_Z_MAPPINGS_COUNT	5

//////////////////////////////////////////////////////////////
ggo_stairs_artist::ggo_stairs_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode)
:
ggo_raytracing_artist_abc(render_width, render_height, rendering_mode)
{
	ggo_ray3d_float	ray;

	float hue = ggo::rand_float();

	// Color.
	_raytrace_params._background_color = ggo_color::from_hsv(hue, 1, 1);
	
	// The fog.
	_raytrace_params._fog_near	= GGO_UNIVERSE_SIZE;
	_raytrace_params._fog_far	= 3*GGO_UNIVERSE_SIZE;

	// The camera.
	float camera_angle = 1.1;
	_camera = ggo_camera3d();
	_camera.get_node().set_pos(0, GGO_UNIVERSE_SIZE*sinf(camera_angle), GGO_UNIVERSE_SIZE*cosf(camera_angle));
	_camera.get_node().rotate_x(-camera_angle);
	_camera.set_aperture(0.02);
	ray.set_pos(0, 0, 0);
	ray.set_dir(0, 0, 1);
	_camera.get_node().rotate(ray, ggo::rand_float(0, 2*GGO_PI));

	// Create cubes.
	create_lights(hue); 
	create_objects(hue);
	
	_raytrace_params._anti_alias	= true;
	_raytrace_params._shadows		= true;
}

//////////////////////////////////////////////////////////////
void ggo_stairs_artist::create_lights(float hue)
{
	_raytracer.clear_lights();

	ggo_color color1 = ggo_color::from_hsv(hue, 1, 1);
	color1.r() *= 0.75;
	color1.g() *= 0.75;
	color1.b() *= 0.75;
	int light1 = _raytracer.create_light();
	_raytracer.get_light(light1)->_color	= color1;
	_raytracer.get_light(light1)->_pos		= _camera.get_node().pos();

	ggo_color color2 = ggo_color::from_hsv(hue, 1, 1);
	color2.r() *= 1.2;
	color2.g() *= 1.2;
	color2.b() *= 1.2;
	int light2 = _raytracer.create_light();
	_raytracer.get_light(light2)->_color	= color2;
	_raytracer.get_light(light2)->_pos		= ggo_point3d_float(0, 0, ggo::rand_float(25, 50));
}

//////////////////////////////////////////////////////////////
void ggo_stairs_artist::create_objects(float hue)
{
	_raytracer.clear_objects3d();
	
	// Cubes positions mappings.
	ggo_mapping mappings[GGO_Z_MAPPINGS_COUNT];
	for (int i = 0; i < GGO_Z_MAPPINGS_COUNT; ++i) 
	{
		mappings[i].set_first_point_y(ggo::rand_float());
		mappings[i].set_last_point_y(ggo::rand_float());
		mappings[i].push_point(ggo::rand_float(0.1, 0.5), ggo::rand_float());
		mappings[i].push_point(ggo::rand_float(0.5, 0.9), ggo::rand_float());
	}

	// Create the cubes.
	for (int x = -GGO_CUBES_HALF_COUNT; x <= GGO_CUBES_HALF_COUNT; ++x)
	{
		ggo_array_float	freq(2*GGO_CUBES_HALF_COUNT+1);
		ggo_array_float	spat(2*GGO_CUBES_HALF_COUNT+1);
		float k = (x+GGO_CUBES_HALF_COUNT)/(2.f*GGO_CUBES_HALF_COUNT);		
		
		freq.fill(0);
		for (int i = 0; i < GGO_Z_MAPPINGS_COUNT; ++i) 
		{
			freq[i+1] = ggo_map<float>(mappings[i].get_value(k, GGO_INTERPOLATION_CUBIC), 0, 1, -5, 5);
		}
		dct(freq, spat, 2*GGO_CUBES_HALF_COUNT+1);
		
		for (int y = -GGO_CUBES_HALF_COUNT; y <= GGO_CUBES_HALF_COUNT; ++y)
		{		
			float pos_x	= 2*x;
			float pos_y	= 2*y;
			float pos_z	= -5 + 4*spat[y+GGO_CUBES_HALF_COUNT];
			
			int obj = _raytracer.create_object3d();

			ggo_polygon3d_float box;
			box.set_axis_aligned_box(1, 1, 10);

			_raytracer.get_object3d(obj)->set_shape(box);
			_raytracer.get_object3d(obj)->get_node().set_pos(pos_x, pos_y, pos_z);
			_raytracer.get_object3d(obj)->set_color(ggo_color::WHITE);
			
			if (ggo::rand_int(0, 25) == 0) 
			{
				_raytracer.get_object3d(obj)->get_node().move(0, 0, ggo::rand_float(5, 10));
			}
		}
	}
}
