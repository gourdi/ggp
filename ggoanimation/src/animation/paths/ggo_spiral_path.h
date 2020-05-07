#ifndef __GGO_SPIRAL_PATH__
#define __GGO_SPIRAL_PATH__
#if 0

#include <animation/ggo_path_abc.h>

namespace ggo
{
  class spiral_path : public path_abc
  {
  public:

                spiral_path(float angle_speed, float radius_speed) : _angle_speed(angle_speed), _radius_speed(radius_speed) {};

    ggo::pos2_f  get_pos(int counter) override;

  private:

    float _angle_speed;
    float _radius_speed;
  };
}

#endif
#endif
