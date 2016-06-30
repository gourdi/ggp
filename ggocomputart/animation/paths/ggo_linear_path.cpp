#include "ggo_linear_path.h"

//////////////////////////////////////////////////////////////
ggo_linear_path::ggo_linear_path(float delta, float angle)
{
	_velocity = ggo::from_polar(angle, delta);
}

//////////////////////////////////////////////////////////////
ggo::pos2f ggo_linear_path::get_pos(int counter)
{
	return float(counter) * _velocity;
}
