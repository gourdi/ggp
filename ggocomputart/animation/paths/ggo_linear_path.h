#ifndef __GGO_LINEAR_PATH__
#define __GGO_LINEAR_PATH__

#include <ggo_path_abc.h>

namespace ggo
{
  class linear_path : public path_abc
  {
  public:

                linear_path(float delta, float angle);
                linear_path(const ggo::vec2f & velocity) : _velocity(velocity) {};

    ggo::pos2f  get_pos(int counter) override;

  private:

    ggo::vec2f _velocity;
  };
}

#endif
