#ifndef __GGO_ARRAY__
#define __GGO_ARRAY__

#include <ggo_kernel.h>

namespace ggo
{
  template <typename data_t, int n_dims>
  class array final
  {
  public:

    // Default constructor.
    array()
    {
      _buffer = nullptr;
    }

    // Constructor to specify the dimensions, and optionnaly a fill value.
    template <typename... args>
    array(args... a)
    {
      array_builder<n_dims, n_dims>::process_args(_dimensions, &_buffer, a...);
    }

    // Copy constructor.
    array(const array<data_t, n_dims> & rhs)
    {
      int count = rhs.get_count();
      ggo::copy<n_dims>(_dimensions, rhs._dimensions);
      _buffer = new data_t[count];
      std::copy(rhs._buffer, rhs._buffer + count, _buffer);
    }

    // Move constructor.
    array(array<data_t, n_dims> && rhs)
    {
      ggo::copy<n_dims>(_dimensions, rhs._dimensions);
      _buffer = rhs._buffer;

      rhs._buffer = nullptr;
    }

    // Coefs constructors (only 1d and 2d).
    template <typename coefs_t, size_t n>
    array(coefs_t const (&coefs)[n])
    {
      static_assert(n_dims == 1);
      _dimensions[0] = n;
      _buffer = new data_t[n];
      ggo::copy<n>(_buffer, coefs);
    }

    template <typename coefs_t, size_t h, size_t w>
    array(coefs_t const (&coefs)[h][w])
    {
      static_assert(n_dims == 2);
      _dimensions[0] = w;
      _dimensions[1] = h;
      _buffer = new data_t[h * w];
      for (int row = 0; row < h; ++row)
      {
        ggo::copy<w>(_buffer + row * w, coefs[row]);
      }
    }

    // Destructor.
    ~array()
    {
      delete[] _buffer;
    }

    // Copy operator=.
    array<data_t, n_dims> & operator=(const array<data_t, n_dims> & rhs)
    {
      delete[] _buffer;

      int count = rhs.get_count();
      ggo::copy<n_dims>(_dimensions, rhs._dimensions);
      _buffer = new data_t[count];
      std::copy(rhs._buffer, rhs._buffer + count, _buffer);

      return *this;
    }

    // Move operator=.
    array<data_t, n_dims> & operator=(array<data_t, n_dims> && rhs)
    {
      delete _buffer;

      ggo::copy<n_dims>(_dimensions, rhs._dimensions);
      _buffer = rhs._buffer;

      rhs._buffer = nullptr;

      return *this;
    }

    // Returns the size of a dimension given as a template parameter.
    template <int dim>
    int get_size() const
    {
      static_assert(dim >= 0 && dim < n_dims);
      return _dimensions[dim];
    }

    int get_size() const
    {
      static_assert(n_dims == 1);
      return _dimensions[0];
    }

    int get_width() const
    {
      static_assert(n_dims == 2);
      return _dimensions[0];
    }

    int get_height() const
    {
      static_assert(n_dims == 2);
      return _dimensions[1];
    }

    // Returns the full number of elements inside the array.
    int get_count() const
    {
      return ggo::multiply_all<n_dims>(_dimensions);
    }

    // Resizes the current array. Current data is lost.
    template <typename... args>
    void resize(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");

      delete[] _buffer;
      array_builder<n_dims, n_dims>::process_args(_dimensions, &_buffer, a...);
    }

    // Comparison.
    bool operator==(const array<data_t, n_dims> & rhs) const
    {
      if (compare<n_dims>(_dimensions, rhs._dimensions) == false)
      {
        return false;
      }

      return std::equal(_buffer, _buffer + get_count(), rhs._buffer);
    }

    bool operator!=(const array<data_t, n_dims> & rhs) const
    {
      return !this->operator==(rhs);
    }

    // Direct access to the buffer.
    data_t * data() { return _buffer; }
    const data_t * data() const { return _buffer; }

    // Subscripting non-const operator. Must provide as many arguements as array's dimensions.
    template <typename... args>
    data_t & operator()(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute(_dimensions, a...)];
    }

    // Subscripting const operator. Must provide as many arguements as array's dimensions.
    template <typename... args>
    const data_t & operator()(args... a) const
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute(_dimensions, a...)];
    }

    template <typename... args>
    data_t & at(args... a) { return this->operator()(a...); }

    template <typename... args>
    const data_t & at(args... a) const { return this->operator()(a...); }

    // Accessing array's elements, with indexes looping over array's dimension.
    template <typename... args>
    data_t & at_loop(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute_loop(_dimensions, a...)];
    }

    // Accessing array's elements, with indexes looping over array's dimension.
    template <typename... args>
    const data_t & at_loop(args... a) const
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute_loop(_dimensions, a...)];
    }

    // Accessing array's elements, with indexes looping over array's dimension.
    template <typename... args>
    data_t & at_mirror(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute_mirror(_dimensions, a...)];
    }

    // Accessing array's elements, with indexes looping over array's dimension.
    template <typename... args>
    const data_t & at_mirror(args... a) const
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      return _buffer[offset<n_dims, n_dims>::compute_mirror(_dimensions, a...)];
    }

    // Iterators helpers.
    data_t *        begin()       { return _buffer; }
    const data_t *  begin() const { return _buffer; }
    data_t *        end()         { return _buffer + get_count(); }
    const data_t *  end() const   { return _buffer + get_count(); }

    // Fill the array with a given value.
    void fill(const data_t & v)
    {
      std::fill(_buffer, _buffer + ggo::multiply_all<n_dims>(_dimensions), v);
    }

  private:

    // Set-up array data members. The trick is that the specialized version that handles the stop case
    // has 2 methods : one to just just the process once all dimensions parameters have been consuped, 
    // and another one to fill the array.
    template <int dim_remaining, int dim_count>
    struct array_builder
    {
      template <typename... args>
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
      template <typename fill_t>
      static void process_args(int * dimensions, data_t ** buffer, const fill_t & fill_value)
      {
        int count = ggo::multiply_all<dim_count>(dimensions);
        *buffer = new data_t[count];
        std::fill(*buffer, *buffer + count, fill_value);
      }

      static void process_args(int * dimensions, data_t ** buffer)
      {
        int count = ggo::multiply_all<dim_count>(dimensions);
        *buffer = new data_t[count];
      }
    };

    // Compute a pointer offset given each dimension positions.
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

      template <typename... args>
      static int compute_loop(const int * dimensions, int index, args... a)
      {
        static_assert(remaining <= count && remaining > 1, "error");
        index = ggo::loop_index(index, dimensions[count - remaining]);
        return index + dimensions[count - remaining] * offset<remaining - 1, count>::compute_loop(dimensions, a...);
      }

      template <typename... args>
      static int compute_mirror(const int * dimensions, int index, args... a)
      {
        static_assert(remaining <= count && remaining > 1, "error");
        index = ggo::mirror_index_edge_duplicated(index, dimensions[count - remaining]);
        return index + dimensions[count - remaining] * offset<remaining - 1, count>::compute_mirror(dimensions, a...);
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

      static int compute_loop(const int * dimensions, int index)
      {
        index = ggo::loop_index(index, dimensions[count - 1]);
        return index;
      }

      static int compute_mirror(const int * dimensions, int index)
      {
        index = ggo::mirror_index_edge_duplicated(index, dimensions[count - 1]);
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
  template <typename data_t, int n_dims>
  std::ostream & operator<<(std::ostream & os, const ggo::array<data_t, n_dims> & a)
  {
    // Do nothing for now.
    return os;
  }
}

//////////////////////////////////////////////////////////////
// Usefull aliases.
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

