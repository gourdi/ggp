#include "ggo_velocity_path.h"
#if 0

//////////////////////////////////////////////////////////////
ggo::velocity_path::velocity_path(float delta, float angle)
{
	_velocity = delta * ggo::vec2_f::from_angle(angle);
}

//////////////////////////////////////////////////////////////
ggo::pos2_f ggo::velocity_path::get_pos(int counter)
{
	return float(counter) * _velocity;
}
#endif
