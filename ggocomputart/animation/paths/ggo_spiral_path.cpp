#include "ggo_spiral_path.h"

//////////////////////////////////////////////////////////////
ggo::point2d_float ggo_spiral_path::get_pos(int counter)
{
	return ggo::point2d_float::from_polar(_angle_speed * counter, _radius_speed * counter);
}
