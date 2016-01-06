#include "ggo_rubiks_cube_artist.h"
#include <ggo_paint.h>
#include <ggo_color_conv.h>
#include <ggo_object3d.h>
using namespace std;

const int	GGO_CUBES_COUNT		= 5;
const float	GGO_UNIVERSE_SIZE	= 10;

//////////////////////////////////////////////////////////////
ggo_rubiks_cube_artist::ggo_rubiks_cube_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode)
:
ggo_raytracing_artist_abc(render_width, render_height, rendering_mode)
{
	ggo_color		color;
	ggo_ray3d_float	ray;

	// Color.
	color.randomize();

	// The camera.
	_camera = ggo_camera3d();
	_camera.get_node().set_pos(0, GGO_UNIVERSE_SIZE, GGO_UNIVERSE_SIZE);
	_camera.get_node().rotate_x(-GGO_PI/4);
	ray.set_pos(0, 0, 0);
	ray.set_dir(0, 0, 1);
	_camera.get_node().rotate(ray, ggo::rand_float(0, 2*GGO_PI));

	// Lights.
	_raytracer.clear_lights();

	int light1	= _raytracer.create_light();
	_raytracer.get_light(light1)->_color	= ggo_color::WHITE;
	_raytracer.get_light(light1)->_pos.x()	= _camera.get_node().pos().x() + ggo::rand_float(-0.5*GGO_UNIVERSE_SIZE, 0.5*GGO_UNIVERSE_SIZE);
	_raytracer.get_light(light1)->_pos.y()	= _camera.get_node().pos().y() + ggo::rand_float(-0.5*GGO_UNIVERSE_SIZE, 0.5*GGO_UNIVERSE_SIZE);
	_raytracer.get_light(light1)->_pos.z()	= _camera.get_node().pos().z() + ggo::rand_float(-0.5*GGO_UNIVERSE_SIZE, 0.5*GGO_UNIVERSE_SIZE);

	int light2	= _raytracer.create_light();
	_raytracer.get_light(light2)->_color	= ggo_color::WHITE;
	_raytracer.get_light(light2)->_pos.x()	= _camera.get_node().pos().x() + ggo::rand_float(-0.5*GGO_UNIVERSE_SIZE, 0.5*GGO_UNIVERSE_SIZE);
	_raytracer.get_light(light2)->_pos.y()	= _camera.get_node().pos().y() + ggo::rand_float(-0.5*GGO_UNIVERSE_SIZE, 0.5*GGO_UNIVERSE_SIZE);
	_raytracer.get_light(light2)->_pos.z()	= _camera.get_node().pos().z() + ggo::rand_float(-0.5*GGO_UNIVERSE_SIZE, 0.5*GGO_UNIVERSE_SIZE);

	// Create cubes.
	create_cubes();
	
	_raytrace_params._anti_alias	= true;
	_raytrace_params._shadows		= true;
}

//////////////////////////////////////////////////////////////
void ggo_rubiks_cube_artist::create_cubes()
{
	int						x, y, z, rotate_x1 , rotate_y1, rotate_z1, rotate_x2 , rotate_y2, rotate_z2;
	float					pos_x, pos_y, pos_z, angle1, angle2, scale;
	ggo_color				base_color, inv_color;
	const ggo_ray3d_float	axis_x(ggo_point3d_float(0, 0, 0), ggo_vector3d_float(1, 0, 0));
	const ggo_ray3d_float	axis_y(ggo_point3d_float(0, 0, 0), ggo_vector3d_float(0, 1, 0));
	const ggo_ray3d_float	axis_z(ggo_point3d_float(0, 0, 0), ggo_vector3d_float(0, 0, 1));

	_raytracer.clear_objects3d();

	// Rotate some row.
	rotate_x1 = rotate_y1 = rotate_z1 = -1;
	rotate_x2 = rotate_y2 = rotate_z2 = -1;
	switch ( ggo::rand_int( 0, 2 ) )
	{
	case 0:
		rotate_x1 = ggo::rand_int( 0, GGO_CUBES_COUNT-1 );
		rotate_x2 = ggo::rand_int( 0, GGO_CUBES_COUNT-1 );
		break;
	case 1:
		rotate_y1 = ggo::rand_int( 0, GGO_CUBES_COUNT-1 );
		rotate_y2 = ggo::rand_int( 0, GGO_CUBES_COUNT-1 );
		break;
	case 2:
		rotate_z1 = ggo::rand_int( 0, GGO_CUBES_COUNT-1 );
		rotate_z2 = ggo::rand_int( 0, GGO_CUBES_COUNT-1 );
		break;
	}
	angle1		= ggo::rand_float( 0, GGO_PI );
	angle2		= ggo::rand_float( 0, GGO_PI );

	// Scale.
	scale		= ggo::rand_float( 0.3, 0.5 );

	// The colors.
	base_color	= ggo_color::from_hsv(ggo::rand_float(), ggo::rand_float(), 1);
	inv_color	= ggo_color(1 - base_color.r(), 1 - base_color.g(), 1 - base_color.b());

	// Create the cube at the right place.
	for (x = 0; x < GGO_CUBES_COUNT; ++x)
	{
		for (y = 0; y < GGO_CUBES_COUNT; ++y)
		{
			for (z = 0; z < GGO_CUBES_COUNT; ++z)
			{
				pos_x		= 
				pos_y		= 0.5*GGO_UNIVERSE_SIZE * ( static_cast<float>(y)/(GGO_CUBES_COUNT-1) - 0.5 );
				pos_z		= 0.5*GGO_UNIVERSE_SIZE * ( static_cast<float>(z)/(GGO_CUBES_COUNT-1) - 0.5 );

				// Add some random color cubes.
				ggo_polygon3d_float	cube;
				cube.set_axis_aligned_box(scale, scale, scale);

				ggo_color	cube_color	= ggo::rand_float() < 1/15.0 ? inv_color : base_color;
				int			obj			= _raytracer.create_object3d();
				_raytracer.get_object3d(obj)->set_shape(cube);
				_raytracer.get_object3d(obj)->set_color(cube_color);
				_raytracer.get_object3d(obj)->get_node().pos().x() = 0.5*GGO_UNIVERSE_SIZE * (static_cast<float>(x)/(GGO_CUBES_COUNT-1) - 0.5);
				_raytracer.get_object3d(obj)->get_node().pos().y() = 0.5*GGO_UNIVERSE_SIZE * (static_cast<float>(y)/(GGO_CUBES_COUNT-1) - 0.5);
				_raytracer.get_object3d(obj)->get_node().pos().z() = 0.5*GGO_UNIVERSE_SIZE * (static_cast<float>(z)/(GGO_CUBES_COUNT-1) - 0.5);

				// Apply the rotation.
				if (x == rotate_x1)
				{
					_raytracer.get_object3d(obj)->get_node().rotate(axis_x, angle1);
				}
				if (y == rotate_y1)
				{
					_raytracer.get_object3d(obj)->get_node().rotate(axis_y, angle1);
				}
				if (z == rotate_z1)
				{
					_raytracer.get_object3d(obj)->get_node().rotate(axis_z, angle1);
				}
				if (x == rotate_x2)
				{
					_raytracer.get_object3d(obj)->get_node().rotate(axis_x, angle2);
				}
				if (y == rotate_y2)
				{
					_raytracer.get_object3d(obj)->get_node().rotate(axis_y, angle2);
				}
				if (z == rotate_z2)
				{
					_raytracer.get_object3d(obj)->get_node().rotate(axis_z, angle2);
				}
			}
		}
	}
}
