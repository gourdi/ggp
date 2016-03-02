#ifndef __GGO_LINEAR_PATH__
#define __GGO_LINEAR_PATH__

#include <ggo_path_abc.h>

class ggo_linear_path : public ggo_path_abc
{
public:
	
                      ggo_linear_path(float delta, float angle);
                      ggo_linear_path(const ggo::vector2d_float & velocity) : _velocity(velocity) {};
	
	ggo::point2d_float	get_pos(int counter) override;
	
private:
	
	ggo::vector2d_float _velocity;
};

#endif
