#ifndef __GGO_DUFFING__
#define __GGO_DUFFING__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_vec.h>

namespace ggo
{
  class duffing
  {
  public:

    duffing()
    :
    _a(ggo::rand<float>(0.245f, 0.255f)),
    _b(ggo::rand<float>(0.295f, 0.305f)),
    _pos(ggo::rand<float>(-1, 1), ggo::rand<float>(-1, 1))
    {

    }

    ggo::pos2f update(const float dt)
    {
      float	x = _pos.x();
      float	y = _pos.y();
      float	dx = y;
      float	dy = x - x * x * x - _a * y + _b * std::cos(_t);

      _t += dt;
      _pos.x() += dx * dt;
      _pos.y() += dy * dt;

      return _pos;
    }

    const float	_a;
    const float	_b;
    float _t = 0.f;
    ggo::pos2f _pos;
  };
}

#endif
