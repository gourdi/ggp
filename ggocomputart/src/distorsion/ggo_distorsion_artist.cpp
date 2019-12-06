#include "ggo_distorsion_artist.h"

//////////////////////////////////////////////////////////////
ggo::distorsion_artist::distorsion_artist()
{
  // Stripes.
  float hue = ggo::rand<float>();

  std::vector<std::pair<float, float>> stripes{ {0.f, 1.f} };
  std::vector<std::pair<float, float>> new_stripes{ {0.f, 1.f} };

  for (int i = 0; i < 5; ++i)
  {
    new_stripes.clear();
    for (const auto& stripe : stripes)
    {
      float x_split = stripe.first + ggo::rand(0.3f, 0.7f) * (stripe.second - stripe.first);
      new_stripes.push_back(std::make_pair(stripe.first, x_split));
      new_stripes.push_back(std::make_pair(x_split, stripe.second));
    }
    stripes = new_stripes;
  }

  for (const auto& stripe : stripes)
  {
    rgb_32f color = ggo::from_hsv<ggo::rgb_32f>(hue, ggo::rand<float>(), ggo::rand<float>());

    _stripes.emplace_back(color, stripe.second);
  }

  // Transforms.
  for (int i = 0; i < 32; ++i)
  {
    transform t;

    t._center_start.x() = ggo::rand(-0.25f, 1.25f);
    t._center_start.y() = ggo::rand(-0.25f, 1.25f);
    t._center_end.x() = ggo::rand(-0.25f, 1.25f);
    t._center_end.y() = ggo::rand(-0.25f, 1.25f);
    t._variance = 0.05f;
    t._disp = ggo::rand(0.f, 1.f);

    _transforms.push_back(t);
  }
}
