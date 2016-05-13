#ifndef __GGO_ARRAY__
#define __GGO_ARRAY__

#include <ggo_kernel.h>

namespace ggo
{
  template<typename T>
  class array
  {
  public:

                      array();
                      array(int size);
                      array(int size, T value);
                      array(const T * buffer, int size);
                      array(const array & a);
                      array(array && a);
                     ~array();

    array &           operator=(const array & a);
    array &           operator=(array && a);

    T &					      front() { return _buffer[0]; };
    const T &			    front() const { return _buffer[0]; };
    T &					      back() { return _buffer[_size-1]; };
    const T &			    back() const { return _buffer[_size-1]; };

                      operator T * () { return _buffer; };
                      operator const T * () const { return _buffer; };

    const char *		  to_char()	const { return (const    char *)(_buffer); };
    const uint8_t *		to_uint8()	const { return (const uint8_t *)(_buffer); };
    const uint16_t *	to_uint16()	const { return (const uint16_t*)(_buffer); };
    const uint32_t *	to_uint32()	const { return (const uint32_t*)(_buffer); };
    const int8_t *		to_int8()	const { return (const  int8_t *)(_buffer); };
    const int16_t *		to_int16()	const { return (const  int16_t*)(_buffer); };
    const int32_t *		to_int32()	const { return (const  int32_t*)(_buffer); };

    char *				    to_char()	{ return (   char *)(_buffer); };
    uint8_t *			    to_uint8()	{ return (uint8_t *)(_buffer); };
    uint16_t *			  to_uint16()	{ return (uint16_t*)(_buffer); };
    uint32_t *			  to_uint32()	{ return (uint32_t*)(_buffer); };
    int8_t *			    to_int8()	{ return ( int8_t *)(_buffer); };
    int16_t *			    to_int16()	{ return ( int16_t*)(_buffer); };
    int32_t *			    to_int32()	{ return ( int32_t*)(_buffer); };

    int					      get_size() const { return _size; };
    void				      set_size(int size); // Clears data.

    T *					      data() { return _buffer; };
    const T *		      data() const { return _buffer; };

    void				      fill(T value);
    void				      zero();
    void				      copy(const T * buffer);

    T *               begin() { return _buffer; }
    const T *         begin() const { return _buffer; }
    T *               end() { return _buffer + _size; }
    const T *         end() const { return _buffer + _size; }

  private:

    T *	_buffer;
    int	_size;
  };
}

namespace ggo
{
  using array_char    = array<char>;
  using array_int     = array<int>;
  using array_uint8   = array<uint8_t>;
  using array_uint16  = array<uint16_t>;
  using array_uint32  = array<uint32_t>;
  using array_uint64  = array<uint64_t>;
  using array_int8    = array<int8_t>;
  using array_int16   = array<int16_t>;
  using array_int32   = array<int32_t>;
  using array_int64   = array<int64_t>;
  using array_float   = array<float>;
  using array_double  = array<double>;
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T>::array()
  {
    _size	= 0;
    _buffer	= nullptr;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T>::array(int size)
  {
    _size	= size;
    _buffer	= new T[size];
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T>::array(int size, T value)
  {
    _size	= size;
    _buffer	= new T[size];

    fill(value);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T>::array(const T * buffer, int size)
  {
    _buffer	= new T[size];
    _size	= size;
    std::copy(buffer, buffer + size, _buffer);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T>::array(const array & a)
  {
    _size = a._size;

    if (a._buffer == nullptr)
    {
      _buffer = nullptr;
    }
    else
    {
      _buffer = new T[a._size];
      std::copy(a._buffer, a._buffer + _size, _buffer);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T>::array(array && a)
  {
    _buffer = a._buffer;
    _size = a._size;

    a._buffer = nullptr;
    a._size = 0;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T>::~array()
  {
    delete[] _buffer;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void array<T>::set_size(int size)
  {
    delete[] _buffer;

    _buffer	= new T[size];
    _size	= size;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T> & array<T>::operator=(const array & a)
  {
    _size = a._size;

    delete[] _buffer;

    if (a._buffer == nullptr)
    {
      _buffer = nullptr;
    }
    else
    {
      _buffer	= new T[a._size];
      std::copy(a._buffer, a._buffer + _size, _buffer);
    }

    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  array<T> & array<T>::operator=(array && a)
  {
    delete[] _buffer;

    _buffer = a._buffer;
    _size = a._size;

    a._buffer = nullptr;
    a._size = 0;

    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void array<T>::fill(T value)
  {
    if (_buffer != nullptr)
    {
      std::fill(_buffer, _buffer + _size, value);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void array<T>::zero()
  {
    fill(0);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void array<T>::copy(const T * buffer)
  {
    if (_buffer != nullptr)
    {
      std::copy(buffer, buffer + _size, _buffer);
    }	
  }
}

#endif

