#include "ggo_linear_path.h"

//////////////////////////////////////////////////////////////
ggo::linear_path::linear_path(float delta, float angle)
{
	_velocity = ggo::from_polar(angle, delta);
}

//////////////////////////////////////////////////////////////
ggo::pos2f ggo::linear_path::get_pos(int counter)
{
	return float(counter) * _velocity;
}
