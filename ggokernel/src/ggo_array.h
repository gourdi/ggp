#ifndef __GGO_ARRAY__
#define __GGO_ARRAY__

#include <ggo_kernel.h>
//
//namespace ggo
//{
//  template<typename T>
//  class array
//  {
//  public:
//
//                      array();
//                      array(int size);
//                      array(int size, T value);
//                      array(const T * buffer, int size);
//                      array(const array & a);
//                      array(array && a);
//                     ~array();
//
//    array &           operator=(const array & a);
//    array &           operator=(array && a);
//
//    T &					      front() { return _buffer[0]; };
//    const T &			    front() const { return _buffer[0]; };
//    T &					      back() { return _buffer[_size-1]; };
//    const T &			    back() const { return _buffer[_size-1]; };
//
//                      operator T * () { return _buffer; };
//                      operator const T * () const { return _buffer; };
//
//    const char *		  to_char()	const { return (const    char *)(_buffer); };
//    const uint8_t *		to_uint8()	const { return (const uint8_t *)(_buffer); };
//    const uint16_t *	to_uint16()	const { return (const uint16_t*)(_buffer); };
//    const uint32_t *	to_uint32()	const { return (const uint32_t*)(_buffer); };
//    const int8_t *		to_int8()	const { return (const  int8_t *)(_buffer); };
//    const int16_t *		to_int16()	const { return (const  int16_t*)(_buffer); };
//    const int32_t *		to_int32()	const { return (const  int32_t*)(_buffer); };
//
//    char *				    to_char()	{ return (   char *)(_buffer); };
//    uint8_t *			    to_uint8()	{ return (uint8_t *)(_buffer); };
//    uint16_t *			  to_uint16()	{ return (uint16_t*)(_buffer); };
//    uint32_t *			  to_uint32()	{ return (uint32_t*)(_buffer); };
//    int8_t *			    to_int8()	{ return ( int8_t *)(_buffer); };
//    int16_t *			    to_int16()	{ return ( int16_t*)(_buffer); };
//    int32_t *			    to_int32()	{ return ( int32_t*)(_buffer); };
//
//    int					      get_size() const { return _size; };
//    void				      set_size(int size); // Clears data.
//
//    T *					      data() { return _buffer; };
//    const T *		      data() const { return _buffer; };
//
//    void				      fill(T value);
//    void				      zero();
//    void				      copy(const T * buffer);
//
//    T *               begin() { return _buffer; }
//    const T *         begin() const { return _buffer; }
//    T *               end() { return _buffer + _size; }
//    const T *         end() const { return _buffer + _size; }
//
//  private:
//
//    T *	_buffer;
//    int	_size;
//  };
//}
//
//
//
//namespace ggo
//{
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T>::array()
//  {
//    _size	= 0;
//    _buffer	= nullptr;
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T>::array(int size)
//  {
//    _size	= size;
//    _buffer	= new T[size];
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T>::array(int size, T value)
//  {
//    _size	= size;
//    _buffer	= new T[size];
//
//    fill(value);
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T>::array(const T * buffer, int size)
//  {
//    _buffer	= new T[size];
//    _size	= size;
//    std::copy(buffer, buffer + size, _buffer);
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T>::array(const array & a)
//  {
//    _size = a._size;
//
//    if (a._buffer == nullptr)
//    {
//      _buffer = nullptr;
//    }
//    else
//    {
//      _buffer = new T[a._size];
//      std::copy(a._buffer, a._buffer + _size, _buffer);
//    }
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T>::array(array && a)
//  {
//    _buffer = a._buffer;
//    _size = a._size;
//
//    a._buffer = nullptr;
//    a._size = 0;
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T>::~array()
//  {
//    delete[] _buffer;
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  void array<T>::set_size(int size)
//  {
//    delete[] _buffer;
//
//    _buffer	= new T[size];
//    _size	= size;
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T> & array<T>::operator=(const array & a)
//  {
//    _size = a._size;
//
//    delete[] _buffer;
//
//    if (a._buffer == nullptr)
//    {
//      _buffer = nullptr;
//    }
//    else
//    {
//      _buffer	= new T[a._size];
//      std::copy(a._buffer, a._buffer + _size, _buffer);
//    }
//
//    return *this;
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  array<T> & array<T>::operator=(array && a)
//  {
//    delete[] _buffer;
//
//    _buffer = a._buffer;
//    _size = a._size;
//
//    a._buffer = nullptr;
//    a._size = 0;
//
//    return *this;
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  void array<T>::fill(T value)
//  {
//    if (_buffer != nullptr)
//    {
//      std::fill(_buffer, _buffer + _size, value);
//    }
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  void array<T>::zero()
//  {
//    fill(0);
//  }
//
//  /////////////////////////////////////////////////////////////////////
//  template <typename T>
//  void array<T>::copy(const T * buffer)
//  {
//    if (_buffer != nullptr)
//    {
//      std::copy(buffer, buffer + _size, _buffer);
//    }	
//  }
//}




namespace ggo
{
  template <typename data_t, int n_dims>
  class array
  {
  public:

    array()
    {
      _buffer = nullptr;
    }

    template <typename... args>
    array(args... a)
    {
      array_builder<n_dims, n_dims>::process_args(_dimensions, &_buffer, a...);
    }

    array(const array<data_t, n_dims> & rhs)
    {
      int count = rhs.get_count();
      copy_array<n_dims>::copy(_dimensions, rhs._dimensions);
      _buffer = new data_t[count];
      std::copy(rhs._buffer, rhs._buffer + count, _buffer);
    }

    array(array<data_t, n_dims> && rhs)
    {
      copy_array<n_dims>::copy(_dimensions, rhs._dimensions);
      _buffer = rhs._buffer;

      rhs._buffer = nullptr;
    }

    ~array()
    {
      delete[] _buffer;
    }

    array<data_t, n_dims> & operator=(const array<data_t, n_dims> & rhs) = delete;


    array<data_t, n_dims> & operator=(array<data_t, n_dims> && rhs)
    {
      delete _buffer;

      copy_array<n_dims>::copy(_dimensions, rhs._dimensions);
      _buffer = rhs._buffer;

      rhs._buffer = nullptr;

      return *this;
    }

    template <int dim>
    int get_size() const
    {
      static_assert(dim >= 0 && dim < n_dims, "invalid dimension");
      return _dimensions[dim];
    }

    int get_count() const
    {
      return multiply_ptr<n_dims, n_dims>::multiply(_dimensions);
    }

    template <typename... args>
    void resize(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");

      delete[] _buffer;
      array_builder<n_dims, n_dims>::process_args(_dimensions, &_buffer, a...);
    }

    data_t * data() { return _buffer; }
    const data_t * data() const { return _buffer; }

    template <typename... args>
    data_t & operator()(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute(_dimensions, a...)];
    }

    template <typename... args>
    const data_t & operator()(args... a) const
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute(_dimensions, a...)];
    }

    data_t *  begin() { return _buffer; }
    const data_t *  begin() const { return _buffer; }
    data_t *  end() { return _buffer + get_count(); }
    const data_t *  end() const { return _buffer + get_count(); }

    void fill(const data_t & v)
    {
      std::fill(_buffer, _buffer + multiply_ptr<n_dims, n_dims>::multiply(_dimensions), v);
    }

  private:

    template <int remaining>
    struct copy_array
    {
      template <typename data_t, int size>
      static void copy(data_t(&dst)[size], const data_t(&src)[size])
      {
        static_assert(size > 0, "error");
        dst[size - remaining] = src[size - remaining];
        copy_array<remaining - 1>::copy(dst, src);
      }
    };

    template <>
    struct copy_array<0>
    {
      template <typename data_t, int size>
      static void copy(data_t(&dst)[size], const data_t(&src)[size])
      {
      }
    };

    template <int remaining, int count>
    struct multiply_ptr
    {
      template <typename data_t>
      static data_t multiply(const data_t * ptr)
      {
        static_assert(count >= 2, "error");
        return ptr[count - remaining] * multiply_ptr<remaining - 1, count>::multiply(ptr);
      }
    };

    template <int count>
    struct multiply_ptr<1, count>
    {
      template <typename data_t>
      static data_t multiply(const data_t * ptr)
      {
        return ptr[count - 1];
      }
    };


    template <int dim_remaining, int dim_count>
    struct array_builder
    {
      template <typename data_t, typename... args>
      static void process_args(int * dimensions, data_t ** buffer, int dim, args... a)
      {
        static_assert(dim_remaining <= dim_count, "error");
        dimensions[dim_count - dim_remaining] = dim;
        array_builder<dim_remaining - 1, dim_count>::process_args(dimensions, buffer, a...);
      }
    };

    template <int dim_count>
    struct array_builder<0, dim_count>
    {
      template <typename data_t, typename fill_t, typename... args>
      static void process_args(int * dimensions, data_t ** buffer, const fill_t & fill_value)
      {
        int count = multiply_ptr<dim_count, dim_count>::multiply(dimensions);
        *buffer = new data_t[count];
        std::fill(*buffer, *buffer + count, fill_value);
      }

      template <typename data_t, typename... args>
      static void process_args(int * dimensions, data_t ** buffer)
      {
        int count = multiply_ptr<dim_count, dim_count>::multiply(dimensions);
        *buffer = new data_t[count];
      }
    };

    template<int remaining, int count>
    struct offset
    {
      template <typename... args>
      static int compute(const int * dimensions, int index, args... a)
      {
        static_assert(remaining <= count && remaining > 1, "error");
        GGO_ASSERT(index >= 0 && index < dimensions[count - remaining]);
        return index + dimensions[count - remaining] * offset<remaining - 1, count>::compute(dimensions, a...);
      }
    };

    template<int count>
    struct offset<1, count>
    {
      static int compute(const int * dimensions, int index)
      {
        GGO_ASSERT(index >= 0 && index < dimensions[count - 1]);
        return index;
      }
    };


  private:

    int       _dimensions[n_dims];
    data_t *  _buffer;
  };
}


namespace ggo
{
  using array_char    = array<char, 1>;
  using array_int     = array<int, 1>;
  using array_uint8   = array<uint8_t, 1>;
  using array_uint16  = array<uint16_t, 1>;
  using array_uint32  = array<uint32_t, 1>;
  using array_uint64  = array<uint64_t, 1>;
  using array_int8    = array<int8_t, 1>;
  using array_int16   = array<int16_t, 1>;
  using array_int32   = array<int32_t, 1>;
  using array_int64   = array<int64_t, 1>;
  using array_float   = array<float, 1>;
  using array_double  = array<double, 1>;
}

#endif

