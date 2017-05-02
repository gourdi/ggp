#ifndef __GGO_BUFFER__
#define __GGO_BUFFER__

#include <ggo_kernel.h>
#include <iterator>

namespace ggo
{
  template <typename data_t>
  class buffer final
  {
  public:

    buffer(size_t size)
    {
      _buffer = new data_t[size];
      _end = _buffer + size;
    }

    buffer(size_t size, const data_t & fill_value)
    {
      _buffer = new data_t[size];
      _end = _buffer + size;
      std::fill(_buffer, _end, fill_value);
    }

    buffer(std::initializer_list<data_t> data)
    {
      _buffer = new data_t[data.size()];
      _end = _buffer + data.size();

      auto * ptr = _buffer;
      for (const auto & v : data)
      {
        *ptr++ = v;
      }
    }

    buffer(buffer && b)
    {
      _buffer = b._buffer;
      _end = b._end;

      b._buffer = nullptr;
    }

    ~buffer()
    {
      delete[] _buffer;
    }

    // Remove copy and move.
    buffer(const buffer & b) = delete;
    void operator=(const buffer & b) = delete;
    void operator=(buffer && b) = delete;

    // Size.
    size_t  get_size() const { return std::distance(_buffer, _end); }

    // Raw buffer access.
    data_t *        data()        { return _buffer; }
    const data_t *  data() const  { return _buffer; }

    // Random access.
    const data_t &  operator[](int i) const { return _buffer[i]; }
    data_t &        operator[](int i)       { return _buffer[i]; }

    // Iterators helpers.
    data_t *        begin()       { return _buffer; }
    const data_t *  begin() const { return _buffer; }
    data_t *        end()         { return _end; }
    const data_t *  end() const   { return _end; }

    // Comparison.
    bool operator==(const ggo::buffer<data_t> & rhs) const
    {
      if (rhs.get_size() != get_size())
      {
        return false;
      }

      const data_t * ptr_rhs = rhs._buffer;
      for (const data_t * ptr = _buffer; ptr != _end; ++ptr, ++ptr_rhs)
      {
        if (*ptr != *ptr_rhs)
        {
          return false;
        }
      }

      return true;
    }

    bool operator!=(const ggo::buffer<data_t> & rhs) const
    {
      return !operator==(rhs);
    }

    // Fill.
    void fill(const data_t & v) { std::fill(_buffer, _end, v); }

  private:

    data_t *  _buffer;
    data_t *  _end;
  };
}

#endif
