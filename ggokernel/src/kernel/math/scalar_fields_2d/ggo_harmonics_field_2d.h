#ifndef __GGO_HARMONIC_2D__
#define __GGO_HARMONIC_2D__

#include <kernel/math/scalar_fields_2d/ggo_scalar_field_2d_abc.h>

namespace ggo
{
  template <typename data_t>
  class harmonics_field_2d : public scalar_field_2d_abc<data_t>
  {
  public:
    
    void    push_harmonic(const ggo::vec2<data_t> & dir, data_t wavelength, data_t amplitude, data_t phase);
    data_t  sample(data_t x, data_t y) const;
    
  private:
  
    struct wave
    {
      ggo::vec2<data_t>  _wave_vector;
      data_t             _phase;
      data_t             _amplitude;
    };
    
    std::vector<wave> _waves;
  };
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename data_t>
  void harmonics_field_2d<data_t>::push_harmonic(const ggo::vec2<data_t> & dir, data_t wavelength, data_t amplitude, data_t phase)
  {
    wave new_wave;
    new_wave._wave_vector = dir.get_normalized() / wavelength;
    new_wave._phase = phase;
    new_wave._amplitude = amplitude;
    _waves.push_back(new_wave);
  }
  
  template <typename data_t>
  data_t harmonics_field_2d<data_t>::sample(data_t x, data_t y) const
  {
    data_t result = 0;
    
    for (const auto & wave : _waves)
    {
      result += wave._amplitude * std::cos(2 * ggo::pi<data_t>() * ggo::dot(wave._wave_vector, ggo::vec2<data_t>(x, y)) + wave._phase);
    }
    
    return result;
  }
}

#endif
