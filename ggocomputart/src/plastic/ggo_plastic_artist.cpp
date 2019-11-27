#include "ggo_plastic_artist.h"
#include <kernel/ggo_ease.h>

//////////////////////////////////////////////////////////////
ggo::plastic_artist::plastic_artist()
{
  for (int i = 0; i < 50; ++i)
  {
    plastic_artist::params params;

    params._center = { ggo::rand(-1.5f, 1.5f), ggo::rand(-1.5f, 1.5f) };
    params._radius = ggo::rand(0.1f, 0.5f);
    params._angle_start = ggo::rand(0.f, 2 * ggo::pi<float>());
    params._angle_end = params._angle_start + ggo::rand(4 * ggo::pi<float>(), 6 * ggo::pi<float>());
    params._den = ggo::rand(0.5f, 2.f);
    params._mult = ggo::rand(0.5f, 2.f);
    params._power = ggo::rand(2.f, 3.f);

    _params.push_back(params);
  }

  _color = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(), 1);
  _altitude_factor = ggo::rand<float>(0.04f, 0.08f);
}

