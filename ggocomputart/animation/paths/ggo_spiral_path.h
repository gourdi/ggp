#ifndef __GGO_SPIRAL_PATH__
#define __GGO_SPIRAL_PATH__

#include <ggo_path_abc.h>

class ggo_spiral_path : public ggo_path_abc
{
public:
	
                      ggo_spiral_path(float angle_speed, float radius_speed) : _angle_speed(angle_speed), _radius_speed(radius_speed) {};
			
	ggo::point2d_float	get_pos(int counter) override;
	
private:
	
	float _angle_speed;
	float _radius_speed;
};

#endif
