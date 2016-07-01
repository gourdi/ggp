#ifndef __GGO_SPARCE_MATRIX__
#define __GGO_SPARCE_MATRIX__

#include <ggo_kernel.h>
#include <map>
#include <iostream>

namespace ggo
{
  template <typename T>
  class sparse_matrix
  {
  public:

          sparse_matrix(int size);
    
    void	set(int y, int x, T v);
    T		  get(int y, int x) const;
    T		  operator()(int y, int x) const { return get(y, x); }
    
    int		get_size() const { return _size; };

    void	apply(const T * v, T * result) const;
    
    void	dump(std::ostream & s) const;
    void	dump_csv(std::ostream & s, char separator) const;

  private:
    
    int					      _size;
    std::map<int, T>	_values;
  };
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  sparse_matrix<T>::sparse_matrix(int size)
  :
  _size(size)
  {
    
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void sparse_matrix<T>::set(int y, int x, T v)
  {
    int index = y * _size + x;
    _values[index] = v;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T sparse_matrix<T>::get(int y, int x) const
  {	
    int index = y * _size + x;
    auto it = _values.find(index);
      
    return it == _values.end() ? T(0) : it->second;
  }

  /////////////////////////////////////////////////////////////////////
  // Optimized code.
  template <typename T>
  void sparse_matrix<T>::apply(const T * v, T * result) const
  {
    std::fill(result, result + _size, T(0));
    
    for (const auto & value : _values)
    {
      int index = value.first;
      int y = index / _size;
      int x = index % _size;
      
      result[y] += value.second * v[x];
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void sparse_matrix<T>::dump(std::ostream & s) const
  {
    for (auto it = _values.cbegin(); it != _values.cend(); ++it)
    {
      s << '(' << it->first << "; " << it->second << ") ";
    }
    s << std::endl;
    
    for (int y = 0; y < _size; ++y)
    {
      for (int x = 0; x < _size; ++x)
      {
        s << "\t" << get(y, x);
      }
      s << std::endl;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void sparse_matrix<T>::dump_csv(std::ostream & s, char separator) const
  {
    for (int y = 0; y < _size; ++y)
    {
      for (int x = 0; x < _size - 1; ++x)
      {
        s << get(y, x) << separator; 
      }
      s << get(y, _size - 1) << std::endl;
    }
  }
}

#endif
