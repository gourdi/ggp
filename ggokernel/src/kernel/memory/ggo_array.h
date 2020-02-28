#ifndef __GGO_ARRAY__
#define __GGO_ARRAY__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_assign.h>
#include <kernel/ggo_compare.h>
#include <kernel/ggo_reduce.h>
#include <kernel/ggo_borders.h>
#include <kernel/ggo_size.h>

namespace ggo
{
  template <typename data_t_, int n_dims>
  class array
  {
  public:

    using data_t = data_t_;

    // Default constructor.
    array()
    {
      _buffer = nullptr;
    }

    // Constructor to specify the dimensions, and optionnaly a fill value.
    template <typename... args>
    array(int dim1, args... a)
    {
      array_builder<n_dims, n_dims>::process_args(_dimensions, &_buffer, dim1, a...);
    }

    // Copy constructor.
    array(const array<data_t, n_dims> & rhs)
    {
      int count = rhs.count();
      ggo::assign(_dimensions, rhs._dimensions);
      _buffer = new data_t[count];
      std::copy(rhs._buffer, rhs._buffer + count, _buffer);
    }

    // Move constructor.
    array(array<data_t, n_dims> && rhs)
    {
      ggo::assign(_dimensions, rhs._dimensions);
      _buffer = rhs._buffer;

      rhs._buffer = nullptr;
    }

    // Destructor.
    ~array()
    {
      delete[] _buffer;
    }

    // Copy operator=.
    array<data_t, n_dims> & operator=(const array<data_t, n_dims> & rhs)
    {
      if (this != &rhs)
      {
        delete[] _buffer;

        int count = rhs.count();
        ggo::assign(_dimensions, rhs._dimensions);
        _buffer = new data_t[count];
        std::copy(rhs._buffer, rhs._buffer + count, _buffer);
      }
      return *this;
    }

    // Move operator=.
    array<data_t, n_dims> & operator=(array<data_t, n_dims> && rhs)
    {
      if (this != &rhs)
      {
        delete[] _buffer;

        ggo::assign(_dimensions, rhs._dimensions);
        _buffer = rhs._buffer;

        rhs._buffer = nullptr;
      }

      return *this;
    }

    // Returns the size of a dimension given as a template parameter.
    template <int dim>
    int size() const
    {
      static_assert(dim < n_dims);
      static_assert(dim > 2); // Use size(), width() or height() instead.
      return _dimensions[dim];
    }

    // Returns the full number of elements inside the array.
    int count() const
    {
      return ggo::mul(_dimensions);
    }

    // Comparison.
    bool operator==(const array<data_t, n_dims> & rhs) const
    {
      if (compare(_dimensions, rhs._dimensions) == false)
      {
        return false;
      }

      return std::equal(_buffer, _buffer + count(), rhs._buffer);
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
    data_t *        end()         { return _buffer + count(); }
    const data_t *  end() const   { return _buffer + count(); }

    // Fill the array with a given value.
    void fill(const data_t & v)
    {
      std::fill(_buffer, _buffer + ggo::mul(_dimensions), v);
    }

    // Operator[] (1D only)
    data_t &        operator[](int i)       { static_assert(n_dims == 1); return _buffer[i]; }
    const data_t &  operator[](int i) const { static_assert(n_dims == 1); return _buffer[i]; }

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
        static_assert(dim_remaining <= dim_count);
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
        int count = ggo::mul<dim_count>(dimensions);
        *buffer = new data_t[count];
        std::fill(*buffer, *buffer + count, fill_value);
      }

      static void process_args(int * dimensions, data_t ** buffer)
      {
        int count = ggo::mul<dim_count>(dimensions);
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
        static_assert(remaining <= count && remaining > 1);
        GGO_ASSERT(index >= 0 && index < dimensions[count - remaining]);
        return index + dimensions[count - remaining] * offset<remaining - 1, count>::compute(dimensions, a...);
      }

      template <typename... args>
      static int compute_loop(const int * dimensions, int index, args... a)
      {
        static_assert(remaining <= count && remaining > 1);
        index = ggo::loop_index(index, dimensions[count - remaining]);
        return index + dimensions[count - remaining] * offset<remaining - 1, count>::compute_loop(dimensions, a...);
      }

      template <typename... args>
      static int compute_mirror(const int * dimensions, int index, args... a)
      {
        static_assert(remaining <= count && remaining > 1);
        index = ggo::mirror_index(index, dimensions[count - remaining]);
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
        index = ggo::mirror_index(index, dimensions[count - 1]);
        return index;
      }
    };

  protected:

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

namespace ggo
{
  template <typename data_t>
  class array1 final : public array<data_t, 1>
  {
  public:
    
    array1(int size) : array<data_t, 1>(size) {}
    array1(int size, const data_t & fill_value) : array<data_t, 1>(size, fill_value ) {}
    
    template <size_t n>
    array1(data_t const (&coefs)[n])
    {
      this->_dimensions[0] = n;
      this->_buffer = new data_t[n];
      std::copy(coefs, coefs + n, this->_buffer);
    }
    
    int size() const { return this->_dimensions[0]; }
  };

  using array_c       = array1<char>;
  using array_i       = array1<int>;
  using array_8u      = array1<uint8_t>;
  using array_32f     = array1<float>;
  using array_f       = array1<float>;
}

namespace ggo
{
  template <typename data_t>
  class array2 final : public array<data_t, 2>
  {
  public:
    
    array2(int width, int height) : array<data_t, 2>(width, height) {}
    array2(int width, int height, const data_t & fill_value) : array<data_t, 2>(width, height, fill_value ) {}
    
    template <size_t h, size_t w>
    array2(data_t const (&coefs)[h][w])
    {
      this->_dimensions[0] = w;
      this->_dimensions[1] = h;
      this->_buffer = new data_t[h * w];

      for (int row = 0; row < h; ++row)
      {
        std::copy(coefs[row], coefs[row] + w, this->_buffer + row * w);
      }
    }

    int width() const
    {
      return this->_dimensions[0];
    }

    int height() const
    {
      return this->_dimensions[1];
    }
    
    ggo::size size() const
    {
      return { this->_dimensions[0], this->_dimensions[1] };
    }
  };

  using array2_8u    = array2<uint8_t>;
  using array2_32s   = array2<int32_t>;
  using array2_i     = array2<int>;
  using array2_32f   = array2<float>;
  using array2_f     = array2<float>;
}

#endif

