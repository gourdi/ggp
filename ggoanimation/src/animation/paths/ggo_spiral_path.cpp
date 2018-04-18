#include "ggo_spiral_path.h"

//////////////////////////////////////////////////////////////
ggo::pos2f ggo::spiral_path::get_pos(int counter)
{
	return ggo::from_polar(_angle_speed * counter, _radius_speed * counter);
}
