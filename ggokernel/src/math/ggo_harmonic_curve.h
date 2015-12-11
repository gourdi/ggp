#ifndef __GGO_HARMONIC_CURVE__
#define __GGO_HARMONIC_CURVE__

#include <map>

namespace ggo
{
  template <typename T>
  class harmonic_curve
  {
  public:

    void	reset() { _harmonics.clear(); }
    void	set_harmonic(int harmonic_index, T amplitude, T offset);

    T		  evaluate(T angle) const;

  private:

    struct harmonic
    {
      harmonic() {};
      harmonic(T amplitude, T offset) : _amplitude(amplitude), _offset(offset) {}
      
      T _amplitude;
      T _offset;
    };

    std::map<int, harmonic> _harmonics;
  };
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void harmonic_curve<T>::set_harmonic(int harmonic_index, T amplitude, T offset)
  {
    _harmonics[harmonic_index] = harmonic(amplitude, offset);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T harmonic_curve<T>::evaluate(T angle) const
  {
    T value = T(0);

    for (auto it = _harmonics.begin(); it != _harmonics.end(); it++)
    {
      const int & harmonic_index = it->first;
      const harmonic & harmonic_data = it->second;
      
      value += harmonic_data._amplitude * std::cos(harmonic_index * (harmonic_data._offset + angle));
    }
    
    return value;
  }
}

#endif