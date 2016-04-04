#ifndef __GGO_PERLIN_NOISE_FIELD_2D__
#define __GGO_PERLIN_NOISE_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>
#include <ggo_interpolation2d.h>

namespace ggo
{
  template <typename data_type = float>
  class perlin_noise_field2d : public scalar_field_2d_abc<data_type>
  {
  public:

    void      add_octave(data_type scale, data_type amplitude, data_type angle = 0, int noise_size = 16);
    
    data_type evaluate(data_type x, data_type y) const override;
    
  private:

    data_type get(int x, int y) const;
    
  private:

    class octave
    {
    public:
    
                  octave(data_type scale, data_type amplitude, data_type angle, int noise_size);
      
        data_type evaluate(data_type x, data_type y) const;
      
        data_type get(int x, int y) const;

    private:
      
      data_type         _scale;
      data_type         _angle;
      int               _noise_size;
      ggo::array_float  _noise;
    };
    
    std::vector<octave> _octaves;
  };
}

//////////////////////////////////////////////////////////////
// Implementation
namespace ggo
{
  template <typename data_type>
  perlin_noise_field2d<data_type>::octave::octave(data_type scale, data_type amplitude, data_type angle, int noise_size)
  :
  _scale(scale),
  _angle(angle),
  _noise_size(noise_size),
  _noise(noise_size * noise_size)
  {
    for (auto & noise_value : _noise)
    {
      noise_value = ggo::rand_real<data_type>(0, amplitude);
    }
  }

  template <typename data_type>
  data_type perlin_noise_field2d<data_type>::octave::evaluate(data_type x, data_type y) const
  {
    x /= _scale;
    y /= _scale;
    
    data_type x_tmp = x * std::cos(_angle) - y * std::sin(_angle);
    data_type y_tmp = x * std::sin(_angle) + y * std::cos(_angle);
    
    x = x_tmp;
    y = y_tmp;

    return ggo::bilinear_interpolation2d_mirror(_noise.get_pointer(), _noise_size, _noise_size, x, y, 1);
  }
      
  template <typename data_type>
  data_type perlin_noise_field2d<data_type>::octave::get(int x, int y) const
  {
    x = ggo::pos_mod(x, _noise_size);
    y = ggo::pos_mod(y, _noise_size);
    
    return _noise[y * _noise_size + x];
  }

  template <typename data_type>
  void perlin_noise_field2d<data_type>::add_octave(data_type scale, data_type amplitude, data_type angle, int noise_size)
  {
    _octaves.emplace_back(scale, amplitude, angle, noise_size);
  }

  template <typename data_type>
  data_type perlin_noise_field2d<data_type>::evaluate(data_type x, data_type y) const
  {
    data_type result(0);
    for (const auto & octave : _octaves)
    {
      result += octave.evaluate(x, y);
    }
    return result;
  }
}

#endif
