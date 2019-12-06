#ifndef __GGO_DISTORSION_ARTIST__
#define __GGO_DISTORSION_ARTIST__

#include <kernel/ggo_vec2.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class distorsion_artist
  {
  public:

    distorsion_artist();

    struct stripe
    {
      stripe(ggo::rgb_32f color, float x_sup) : _color(color), _x_sup(x_sup) {}

      ggo::rgb_32f  _color;
      float         _x_sup;
    };

    struct transform
    {
      ggo::pos2_f _center_start;
      ggo::pos2_f _center_end;
      float       _disp;
      float       _variance;
    };

    template <typename Func>
    void for_each_stripe(Func&& f) const
    {
      for (const auto& stripe : _stripes)
      {
        f(stripe);
      }
    }

    template <typename Func>
    void for_each_transform(float progress, Func&& f) const
    {
      for (const auto& transform : _transforms)
      {
        ggo::pos2_f center = ggo::linerp(transform._center_start, transform._center_end, 1 - progress);
        float disp = ggo::linerp(0.f, transform._disp, 1 - progress);

        f(center, disp, transform._variance);
      }
    }

  private:

    std::vector<stripe>     _stripes;
    std::vector<transform>  _transforms;
  };
}

#endif