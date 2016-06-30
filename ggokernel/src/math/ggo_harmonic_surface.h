#include <vector>
#include <ggo_kernel.h>

namespace ggo
{
  template <typename T>
  class harmonic_surface
  {
  public:
    
    void  push_harmonic(const ggo::vec2<T> & dir, T wavelength, T amplitude, T phase);
    T     evaluate(T x, T y) const;
    
  private:
  
    struct wave
    {
      ggo::vec2<T>  _wave_vector;
      T             _phase;
      T             _amplitude;
    };
    
    std::vector<wave> _waves;
  };
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename T>
  void harmonic_surface<T>::push_harmonic(const ggo::vec2<T> & dir, T wavelength, T amplitude, T phase)
  {
    wave new_wave;
    new_wave._wave_vector = dir;
    new_wave._wave_vector.set_length(1 / wavelength);
    new_wave._phase = phase;
    new_wave._amplitude = amplitude;
    _waves.push_back(new_wave);
  }
  
  template <typename T>
  T harmonic_surface<T>:: evaluate(T x, T y) const
  {
    T result = 0;
    
    for (const auto & wave : _waves)
    {
      result += wave._amplitude * std::cos(2 * ggo::PI<float>() * ggo::dot(wave._wave_vector, ggo::vec2<T>(x, y)) + wave._phase);
    }
    
    return result;
  }
}
