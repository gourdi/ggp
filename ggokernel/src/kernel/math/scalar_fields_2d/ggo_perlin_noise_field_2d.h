#ifndef __GGO_PERLIN_NOISE_FIELD_2D__
#define __GGO_PERLIN_NOISE_FIELD_2D__

#include <kernel/math/scalar_fields_2d/ggo_scalar_field_2d_abc.h>
#include <kernel/math/interpolation/ggo_interpolation2d.h>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  template <typename data_t = float, bool positive_only = true>
  class perlin_noise_field2d : public scalar_field_2d_abc<data_t>
  {
  public:

    void   add_octave(data_t scale, data_t amplitude, data_t angle = 0, int noise_size = 16);
    
    data_t sample(data_t x, data_t y) const override;

  private:

    class octave
    {
    public:
    
              octave(data_t scale, data_t amplitude, data_t angle, int noise_size);
      
      data_t  sample(data_t x, data_t y) const;
      
      data_t  get(int x, int y) const;

    private:
      
      data_t                _scale;
      data_t                _angle;
      int                   _noise_size;
      ggo::array<data_t, 1> _noise;
    };
    
    std::vector<octave> _octaves;
  };
}

//////////////////////////////////////////////////////////////
// Implementation
namespace ggo
{
  template <typename data_t, bool positive_only>
  perlin_noise_field2d<data_t, positive_only>::octave::octave(data_t scale, data_t amplitude, data_t angle, int noise_size)
  :
  _scale(scale),
  _angle(angle),
  _noise_size(noise_size),
  _noise(noise_size * noise_size)
  {
    for (auto & noise_value : _noise)
    {
      noise_value = ggo::rand<data_t>(positive_only ? data_t(0) : -amplitude, amplitude);
    }
  }

  template <typename data_t, bool positive_only>
  data_t perlin_noise_field2d<data_t, positive_only>::octave::sample(data_t x, data_t y) const
  {
    x /= _scale;
    y /= _scale;
    
    data_t x_tmp = x * std::cos(_angle) - y * std::sin(_angle);
    data_t y_tmp = x * std::sin(_angle) + y * std::cos(_angle);
    
    x = x_tmp;
    y = y_tmp;

    return ggo::linear_interpolation2d_mirror<data_t, ggo::lines_order::up>( // We don't care about the direction
      _noise.data(), _noise_size, _noise_size, x, y);
  }
      
  template <typename data_t, bool positive_only>
  data_t perlin_noise_field2d<data_t, positive_only>::octave::get(int x, int y) const
  {
    x = ggo::pos_mod(x, _noise_size);
    y = ggo::pos_mod(y, _noise_size);
    
    return _noise(y * _noise_size + x);
  }

  template <typename data_t, bool positive_only>
  void perlin_noise_field2d<data_t, positive_only>::add_octave(data_t scale, data_t amplitude, data_t angle, int noise_size)
  {
    _octaves.emplace_back(scale, amplitude, angle, noise_size);
  }

  template <typename data_t, bool positive_only>
  data_t perlin_noise_field2d<data_t, positive_only>::sample(data_t x, data_t y) const
  {
    data_t result(0);
    for (const auto & octave : _octaves)
    {
      result += octave.sample(x, y);
    }
    return result;
  }
}

#endif
