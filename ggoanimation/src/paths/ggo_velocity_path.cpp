#include "ggo_velocity_path.h"

//////////////////////////////////////////////////////////////
ggo::velocity_path::velocity_path(float delta, float angle)
{
	_velocity = ggo::from_polar(angle, delta);
}

//////////////////////////////////////////////////////////////
ggo::pos2f ggo::velocity_path::get_pos(int counter)
{
	return float(counter) * _velocity;
}
