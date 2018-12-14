#include "ggo_spiral_path.h"

//////////////////////////////////////////////////////////////
ggo::pos2f ggo::spiral_path::get_pos(int counter)
{
	return _radius_speed * counter * ggo::vec2f::from_angle(_angle_speed * counter);
}
