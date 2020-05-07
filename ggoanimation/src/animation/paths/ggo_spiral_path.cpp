#include "ggo_spiral_path.h"
#if 0
//////////////////////////////////////////////////////////////
ggo::pos2_f ggo::spiral_path::get_pos(int counter)
{
	return _radius_speed * counter * ggo::vec2_f::from_angle(_angle_speed * counter);
}
#endif
