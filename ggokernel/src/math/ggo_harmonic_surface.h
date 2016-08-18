#include <vector>
#include <ggo_kernel.h>

namespace ggo
{
  template <typename data_t>
  class harmonic_surface
  {
  public:
    
    void    push_harmonic(const ggo::vec2<data_t> & dir, data_t wavelength, data_t amplitude, data_t phase);
    data_t  evaluate(data_t x, data_t y) const;
    
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
  void harmonic_surface<data_t>::push_harmonic(const ggo::vec2<data_t> & dir, data_t wavelength, data_t amplitude, data_t phase)
  {
    wave new_wave;
    new_wave._wave_vector = dir;
    new_wave._wave_vector.set_length(1 / wavelength);
    new_wave._phase = phase;
    new_wave._amplitude = amplitude;
    _waves.push_back(new_wave);
  }
  
  template <typename data_t>
  data_t harmonic_surface<data_t>::evaluate(data_t x, data_t y) const
  {
    data_t result = 0;
    
    for (const auto & wave : _waves)
    {
      result += wave._amplitude * std::cos(2 * ggo::pi<data_t>() * ggo::dot(wave._wave_vector, ggo::vec2<data_t>(x, y)) + wave._phase);
    }
    
    return result;
  }
}
