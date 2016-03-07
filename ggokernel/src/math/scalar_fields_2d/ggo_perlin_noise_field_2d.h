#ifndef __GGO_PERLIN_NOISE_FIELD_2D__
#define __GGO_PERLIN_NOISE_FIELD_2D__

#include <ggo_scalar_field_2d_abc.h>
#include <ggo_interpolation2d.h>

namespace ggo
{
  template <typename T = float>
  class perlin_noise_field2d : public scalar_field_2d_abc<T>
  {
  public:

    void  add_octave(T scale, T amplitude, T angle = 0, int noise_size = 16);
    
    T     evaluate(T x, T y) const override;
    
  private:

    T     get(int x, int y) const;
    
  private:

    class octave
    {
    public:
    
        octave(T scale, T amplitude, T angle, int noise_size);
      
      T evaluate(T x, T y) const;
      
      T get(int x, int y) const;

    private:
      
      T                 _scale;
      T                 _angle;
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
  template <typename T>
  perlin_noise_field2d<T>::octave::octave(T scale, T amplitude, T angle, int noise_size)
  :
  _scale(scale),
  _angle(angle),
  _noise_size(noise_size),
  _noise(noise_size * noise_size)
  {
    for (auto & noise_value : _noise)
    {
      noise_value = ggo::rand_real<T>(0, amplitude);
    }
  }

  template <typename T>
  T perlin_noise_field2d<T>::octave::evaluate(T x, T y) const
  {
    x /= _scale;
    y /= _scale;
    
    T x_tmp = x * std::cos(_angle) - y * std::sin(_angle);
    T y_tmp = x * std::sin(_angle) + y * std::cos(_angle);
    
    x = x_tmp;
    y = y_tmp;

    return ggo::bilinear_interpolation2d<T, T, T, T, ggo::fetch_data_duplicated_edge_mirror2d_const<T>>(_noise, _noise_size, _noise_size, x, y);
  }
      
  template <typename T>
  T perlin_noise_field2d<T>::octave::get(int x, int y) const
  {
    x = ggo::pos_mod(x, _noise_size);
    y = ggo::pos_mod(y, _noise_size);
    
    return _noise[y * _noise_size + x];
  }

  template <typename T>
  void perlin_noise_field2d<T>::add_octave(T scale, T amplitude, T angle, int noise_size)
  {
    _octaves.emplace_back(scale, amplitude, angle, noise_size);
  }

  template <typename T>
  T perlin_noise_field2d<T>::evaluate(T x, T y) const
  {
    T result(0);
    for (const auto & octave : _octaves)
    {
      result += octave.evaluate(x, y);
    }
    return result;
  }
}

#endif
