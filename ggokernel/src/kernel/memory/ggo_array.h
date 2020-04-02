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
  class array final
  {
  public:

    using data_t = data_t_;

    template <typename... args_t>
    array(args_t... args)
    {
      static_assert(sizeof...(args) == n_dims || sizeof...(args) == n_dims + 1);

      ggo::assign<n_dims>(_dims, args...);
      _buffer = new data_t[this->count()];

      if constexpr (sizeof...(args) == n_dims + 1)
      {
        this->fill(std::get<n_dims>(std::make_tuple(args...)));
      }
    }

    array(const array<data_t, n_dims> & a)
    {
      ggo::assign(_dims, a._dims);
      _buffer = new data_t[this->count()];
      std::copy(a._buffer, a._buffer + count(), _buffer);
    }

    array(array<data_t, n_dims> && a)
    {
      ggo::assign(_dims, a._dims);
      _buffer = a._buffer;
      a._buffer = nullptr;
    }

    ~array()
    {
      delete[] _buffer;
    }

    int count() const { return ggo::mul(_dims); }
    const data_t * data() const { return _buffer; }
    data_t * data() { return _buffer; }

    const data_t* begin() const { return _buffer; }
    data_t* begin() { return _buffer; }
    const data_t* end() const { return _buffer + count(); }
    data_t* end() { return _buffer + count(); }

    int dim(int dim) const { return _dims[dim]; }

    void operator=(const array<data_t, n_dims> & a)
    {
      _dims = a._dims;
      std::copy(a._buffer, a._buffer + count(), _buffer);
    }

    void operator=(array<data_t, n_dims> && a)
    {
      delete[] _buffer;
      ggo::assign(_dims, a._dims);
      _buffer = a._buffer;
      a._buffer = nullptr;
    }

    bool operator==(const array<data_t, n_dims> & a) const
    {
      return std::equal(_dims, _dims + n_dims, a._dims) && std::equal(_buffer, _buffer + count(), a._buffer);
    }
    bool operator!=(const array<data_t, n_dims>& a) const
    {
      return !operator==(a);
    }

    template <ggo::border_mode mode, typename... indices_t>
    size_t offset(indices_t... indices) const
    {
      return offset_aux<sizeof...(indices) - 1, mode>(std::make_tuple(indices...));
    }

    template <typename... indices_t>
    const data_t& operator()(indices_t... indices) const
    {
      return _buffer[offset<ggo::border_mode::in_memory>(indices...)];
    }

    template <typename... indices_t>
    data_t & operator()(indices_t... indices)
    {
      return _buffer[offset<ggo::border_mode::in_memory>(indices...)];
    }

    template <ggo::border_mode mode, typename... indices_t>
    const data_t & at(indices_t... indices) const
    {
      return _buffer[offset<mode>(indices...)];
    }

    void fill(const data_t& v) { std::fill(_buffer, _buffer + count(), v); }

    // 1D API
    template <size_t n>
    array(data_t const (&coefs)[n])
    {
      static_assert(n_dims == 1);
      _dims[0] = n;
      _buffer = new data_t[n];
      std::copy(coefs, coefs + n, _buffer);
    }

    int size() const { static_assert(n_dims == 1); return _dims[0]; }
    const data_t & operator[](int index) const { static_assert(n_dims == 1); return _buffer[index]; }
    data_t & operator[](int index) { static_assert(n_dims == 1); return _buffer[index]; }

    // 2D API
    template <size_t h, size_t w>
    array(data_t const (&coefs)[h][w])
    {
      static_assert(n_dims == 2);
      _dims[0] = h;
      _dims[1] = w;
      _buffer = new data_t[h * w];

      for (int row = 0; row < h; ++row)
      {
        std::copy(coefs[row], coefs[row] + w, _buffer + row * w);
      }
    }

    int width() const { static_assert(n_dims == 2); return _dims[1]; }
    int height() const { static_assert(n_dims == 2); return _dims[0]; }
    int rows() const { static_assert(n_dims == 2); return _dims[0]; }
    int cols() const { static_assert(n_dims == 2); return _dims[1]; }

  private:

    template <int dim, border_mode mode, typename tuple_t>
    size_t offset_aux(const tuple_t & indices) const
    {
      if constexpr (dim == 0)
      {
        return ggo::index<mode>(std::get<0>(indices), _dims[dim]);
      }
      else
      {
        return offset_aux<dim - 1, mode>(indices) * _dims[dim] + ggo::index<mode>(std::get<dim>(indices), _dims[dim]);
      }
    }

    int _dims[n_dims];
    data_t* _buffer;
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
  using array1 = array<data_t, 1>;

  using array_c   = array1<char>;
  using array_i   = array1<int>;
  using array_8u  = array1<uint8_t>;
  using array_32f = array1<float>;
  using array_f   = array1<float>;
}

namespace ggo
{
  template <typename data_t>
  using array2 = array<data_t, 2>;

  using array2_8u   = array2<uint8_t>;
  using array2_32s  = array2<int32_t>;
  using array2_i    = array2<int>;
  using array2_32f  = array2<float>;
  using array2_f    = array2<float>;
}

#endif

