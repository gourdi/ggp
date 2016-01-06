#include "ggo_zero_gravity_artist.h"
#include <ggo_object3d.h>
using namespace std;

#define	GGO_RADIUS_MIN	0.5
#define	GGO_RADIUS_MAX	5
#define	GGO_MARGIN		1

//////////////////////////////////////////////////////////////
ggo_zero_gravity_artist::ggo_zero_gravity_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode)
:
ggo_raytracing_artist_abc(render_width, render_height, rendering_mode)
{
	float hue = ggo::rand_float();

	// Raytracer parameters.
	_raytrace_params._background_color		= ggo_color::from_hsv(hue, ggo::rand_float(), ggo::rand_float());
	_raytrace_params._depth_of_field_factor = ggo::rand_float(2, 5);

	// The camera.
	_camera = ggo_camera3d();
	_camera.set_aperture(0.1);
	_camera.get_node().set_pos(0, 0, 250);
	_camera.get_node().rotate(ggo_ray3d_float(ggo_point3d_float(0, 0, 0), ggo_vector3d_float(-1, 0, 0)), 1.2);
	_camera.get_node().move(0, 10, 25);
	
	// Lights
	_raytracer.clear_lights();

	int light1 = _raytracer.create_light();
	_raytracer.get_light(light1)->_color	= ggo_color::from_hsv(ggo::rand_float(), 0.8, 0.8);
	_raytracer.get_light(light1)->_pos		= ggo_point3d_float(ggo::rand_float(-5000, 5000), ggo::rand_float(-5000, 5000), 10000);

	int light2 = _raytracer.create_light();
	_raytracer.get_light(light2)->_color	= ggo_color(0.8, 0.8, 0.8);
	_raytracer.get_light(light2)->_pos		= ggo_point3d_float(0, 0, 10000);
	
	// Spheres.
	_raytracer.clear_objects3d();

	vector<ggo_sphere3d_float> spheres;

	// The seed.
	float				radius = ggo::rand_float(GGO_RADIUS_MIN, GGO_RADIUS_MAX);
	ggo_point3d_float	center(0, 0, radius);
	spheres.push_back(ggo_sphere3d_float(center, radius));

	// The leaves.
	while (spheres.size() < 100)
	{
		int seed_index = ggo::rand_int(0, spheres.size()-1);

		// Create new sphere.
		const ggo_sphere3d_float & seed = spheres[seed_index];
		float				radius	= ggo::rand_float(GGO_RADIUS_MIN, GGO_RADIUS_MAX);
		ggo_vector3d_float	dir		= ggo_vector3d_float(ggo::rand_float(-1, 1), ggo::rand_float(-1, 1), ggo::rand_float());
		dir.normalize();
		ggo_point3d_float	center	= seed.center() + dir * (radius + seed.radius() + GGO_MARGIN);
		
		// Check if new sphere intersects old ones.
		size_t i = 0;
		for (i = 0; i < spheres.size(); ++i)
		{
			if (i == seed_index)
			{
				continue;
			}
			
			if (ggo_distance(spheres[i].center(), center) < spheres[i].radius() + radius + GGO_MARGIN)
			{
				break;
			}
		}
		
		if (i == spheres.size())
		{
			spheres.push_back(ggo_sphere3d_float(center, radius));
		}
	}
	
	for (vector<ggo_sphere3d_float>::const_iterator it = spheres.begin(); it != spheres.end(); ++it)
	{
		ggo_sphere3d_float 	sphere(ggo_point3d_float(0, 0, 0), it->radius());
		
		int obj = _raytracer.create_object3d();
		_raytracer.get_object3d(obj)->set_shape(sphere);
		_raytracer.get_object3d(obj)->get_node().pos() = it->center();
		_raytracer.get_object3d(obj)->set_phong(2, 100);
		_raytracer.get_object3d(obj)->set_color(ggo_color::from_hsv(hue, ggo::rand_float(), ggo::rand_float()));
		_raytracer.get_object3d(obj)->set_reflexion_factor(0.1);
	}

	// Floor.
	vector<ggo_point3d_float> points;
	points.push_back(ggo_point3d_float(-100, -100, 0));
	points.push_back(ggo_point3d_float(-100,  100, 0));
	points.push_back(ggo_point3d_float( 100, -100, 0));
	points.push_back(ggo_point3d_float( 100,  100, 0));

	vector<ggo_polygon3d_float::ggo_face> faces;
	faces.push_back(ggo_polygon3d_float::ggo_face(0, 2, 1));
	faces.push_back(ggo_polygon3d_float::ggo_face(1, 2, 3));

	int obj = _raytracer.create_object3d();
	_raytracer.get_object3d(obj)->set_shape(ggo_polygon3d_float(points, faces));
	_raytracer.get_object3d(obj)->get_node().set_pos(0, 0, 0);
	_raytracer.get_object3d(obj)->set_color(ggo_color::WHITE);
	_raytracer.get_object3d(obj)->set_reflexion_factor(0.5);
	_raytracer.get_object3d(obj)->set_reflexion_factor(0.2);
	
	_raytrace_params._shadows			= true;
	_raytrace_params._fog_near			= 250;
	_raytrace_params._fog_far			= 350;
	_raytrace_params._depth_of_field	= 250;
}
