#ifndef __GGO_VELOCITY_PATH__
#define __GGO_VELOCITY_PATH__

#include <animation/ggo_path_abc.h>

namespace ggo
{
  class velocity_path : public path_abc
  {
  public:

                velocity_path(float delta, float angle);
                velocity_path(const ggo::vec2_f & velocity) : _velocity(velocity) {};

    ggo::pos2_f  get_pos(int counter) override;

  private:

    ggo::vec2_f _velocity;
  };
}

#endif
