#ifndef __GGO_CABREL_CURVE__
#define __GGO_CABREL_CURVE__

#include <ggo_kernel.h>
#include <ggo_random_interpolator_abc.h>
#include <ggo_marching_squares.h>

class ggo_cabrel_coef_interpolator : public ggo::random_interpolator_abc<float, float>
{
  void get_random_data(float & coef, float & dt) override
  {
    coef = ggo::rand_float(-1.f, 1.f);
    dt = ggo::rand_float(20.f, 50.f);
  }
};

template <int deg_max>
class ggo_cabrel_curve
{
public:

  void update()
  {
    // Interpolate curve coefs.
    GGO_ASSERT_EQ(_coefs.size(), _coefs_interpolators.size());
    for (int i = 0; i < _coefs.size(); ++i)
    {
      _coefs[i] = _coefs_interpolators[i].update(1);
    }
  }

  void paint(ggo::rgb_image_abc & image) const
  {
    // Lambda that evaluate the polynom at a given location.
    auto eval = [&](float x, float y)
    {
      int i = 0;

      float v = _coefs[i++];
      for (int deg = 1; deg <= deg_max; ++deg)
      {
        for (int deg_x = 0; deg_x <= deg; ++deg_x)
        {
          int deg_y = deg - deg_x;
          v += _coefs[i++] * std::pow(x, ggo::to<float>(deg_x)) * std::pow(y, ggo::to<float>(deg_y));
        }
      }

      return v;
    };

    // Retrieve segments with marching squares algorithm.
    const float range = 4.f;
    const int steps = 250;
    const float step_size = range / ggo::to<float>(steps);
    auto cells = ggo::marching_squares(eval, ggo::pos2f(-0.5f * range, -0.5f * range), steps, step_size);

    // Paint segments.
    const ggo::pos2f offset(0.5f * image.get_max_size(), 0.5f * image.get_max_size());
    const float scale = image.get_max_size() / range;
    const float width = 0.002f * image.get_min_size();
    std::vector<ggo::rgb_layer> layers;

    ggo::color c(ggo::color::get_random());

    for (const auto & cell : cells)
    {
      for (const auto & segment : cell._segments)
      {
        auto extended_segment = std::make_shared<ggo::extended_segment_float>(offset + scale * segment.p1(), offset + scale * segment.p2(), width);
        layers.emplace_back(extended_segment, c);
      }
    }

    ggo::paint(image, layers);
  }

private:

  std::array<float, (deg_max + 1) * (deg_max + 2) / 2>                        _coefs;
  std::array<ggo_cabrel_coef_interpolator, (deg_max + 1) * (deg_max + 2) / 2> _coefs_interpolators;
};

#endif
