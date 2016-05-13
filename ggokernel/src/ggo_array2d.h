#ifndef GGO_ARRAY2D_H_
#define GGO_ARRAY2D_H_

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// ARRAY2D
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class array2d
  {
  public:

                  array2d();
                  array2d(int size_x, int size_y);
                  array2d(int size_x, int size_y, const T & value);
                  array2d(const array2d<T> & rhs);
                  array2d(array2d<T> && rhs);
                 ~array2d() { delete[] _data; }

    T &			      operator()(int x, int y);
    const T &	    operator()(int x, int y) const;

    int			      get_size_x() const { return _size_x; }
    int			      get_size_y() const { return _size_y; }
    void		      set_size(int size_x, int size_y);

    void		      fill(const T & value) { std::fill(_data, _data + _size_x * _size_y, value); }

    T *			      get_pointer() { return _data; }
    const T *	    get_pointer() const { return _data; }

    T *           begin() { return _data; }
    const T *     begin() const { return _data; }
    T *           end() { return _data + _size_x * _size_y; }
    const T *     end() const { return _data + _size_x * _size_y; }

    array2d<T> &  operator=(const array2d<T> & rhs);
    array2d<T> &  operator=(array2d<T> && rhs);

  private:

    T *	_data;
    int _size_x;
    int _size_y;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  array2d<T>::array2d()
  {
    _data	= nullptr;
    _size_x	= 0;
    _size_y	= 0;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  array2d<T>::array2d(int size_x, int size_y)
  {
    _data	= new T[size_x*size_y];
    _size_x	= size_x;
    _size_y	= size_y;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  array2d<T>::array2d(int size_x, int size_y, const T & value)
  :
  array2d(size_x, size_y)
  {
    fill(value);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  array2d<T>::array2d(const array2d<T> & rhs)
  {
    _data = new T[rhs._size_x * rhs._size_y];
    _size_x = rhs._size_x;
    _size_y = rhs._size_y;
  
    std::copy(rhs._data, rhs._data + rhs._size_x * rhs._size_y, _data);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  array2d<T>::array2d(array2d<T> && rhs)
  {
    _data = rhs._data;
    _size_x = rhs._size_x;
    _size_y = rhs._size_y;

    rhs._data = nullptr;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void array2d<T>::set_size(int size_x, int size_y)
  {
    delete[] _data;

    _data	= new T[size_x*size_y];
    _size_x	= size_x;
    _size_y	= size_y;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  array2d<T> & array2d<T>::operator=(const array2d<T> & rhs)
  {
    delete[] _data;

    _data = new T[rhs._size_x * rhs._size_y];
    _size_x = rhs._size_x;
    _size_y = rhs._size_y;

    std::copy(rhs._data, rhs._data + rhs._size_x * rhs._size_y, _data);

    return *this;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  array2d<T> & array2d<T>::operator=(array2d<T> && rhs)
  {
    delete[] _data;

    _data = rhs._data;
    _size_x = rhs._size_x;
    _size_y = rhs._size_y;

    rhs._data = nullptr;

    return *this;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T & array2d<T>::operator()(int x, int y)
  {
    GGO_ASSERT_BTW(x, 0, _size_x - 1);
    GGO_ASSERT_BTW(y, 0, _size_y - 1); 
    return _data[y * _size_x + x];
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  const T & array2d<T>::operator()(int x, int y) const
  {
    GGO_ASSERT_BTW(x, 0, _size_x - 1);
    GGO_ASSERT_BTW(y, 0, _size_y - 1);
    return _data[y * _size_x + x];
  }
}

//////////////////////////////////////////////////////////////
// GRID (WIDTH == HEIGHT)
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class grid2d
  {
  public:

              grid2d() {}
              grid2d(int size) : _array2d(size, size) {}
              grid2d(int size, const T & value) : _array2d(size, size, value) {}

    T &			  operator()(int x, int y) { return _array2d(x, y); }
    const T &	operator()(int x, int y) const { return _array2d(x, y); }

    int			  get_size() const { return _array2d.get_size_x(); }
    void		  set_size(int size) { _array2d.set_size(size, size); }
  
    void		  fill(const T & value) { _array2d.fill(value); }

  private:

    array2d<T>  _array2d;
  };
}

#endif
