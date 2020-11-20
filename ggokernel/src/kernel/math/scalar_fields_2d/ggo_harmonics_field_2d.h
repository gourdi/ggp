#pragma once

#include <kernel/ggo_vec2.h>
#include <kernel/ggo_kernel.h>
#include <kernel/math/scalar_fields_2d/ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <typename scalar_t>
  class harmonics_field_2d : public scalar_field_2d_abc<scalar_t>
  {
  public:
    
    void    push_harmonic(const ggo::vec2<scalar_t> & dir, scalar_t wavelength, scalar_t amplitude, scalar_t phase);
    scalar_t  sample(scalar_t x, scalar_t y) const;
    
  private:
  
    struct wave
    {
      ggo::vec2<scalar_t>  _wave_vector;
      scalar_t             _phase;
      scalar_t             _amplitude;
    };
    
    std::vector<wave> _waves;
  };
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename scalar_t>
  void harmonics_field_2d<scalar_t>::push_harmonic(const ggo::vec2<scalar_t> & dir, scalar_t wavelength, scalar_t amplitude, scalar_t phase)
  {
    wave new_wave;
    new_wave._wave_vector = normalize(dir) / wavelength;
    new_wave._phase = phase;
    new_wave._amplitude = amplitude;
    _waves.push_back(new_wave);
  }
  
  template <typename scalar_t>
  scalar_t harmonics_field_2d<scalar_t>::sample(scalar_t x, scalar_t y) const
  {
    scalar_t result = 0;
    
    for (const auto & wave : _waves)
    {
      result += wave._amplitude * std::cos(2 * ggo::pi<scalar_t>() * ggo::dot(wave._wave_vector, ggo::vec2<scalar_t>(x, y)) + wave._phase);
    }
    
    return result;
  }
}
