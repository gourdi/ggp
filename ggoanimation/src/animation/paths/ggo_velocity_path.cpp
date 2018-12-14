#include "ggo_velocity_path.h"

//////////////////////////////////////////////////////////////
ggo::velocity_path::velocity_path(float delta, float angle)
{
	_velocity = delta * ggo::vec2f::from_angle(angle);
}

//////////////////////////////////////////////////////////////
ggo::pos2f ggo::velocity_path::get_pos(int counter)
{
	return float(counter) * _velocity;
}
