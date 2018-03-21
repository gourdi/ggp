#ifndef __GGO_BUFFER__
#define __GGO_BUFFER__

#include <ggo_kernel.h>
#include <cstring>

namespace ggo
{
  template <typename data_t = uint8_t>
  class buffer_t final
  {
  public:

    buffer_t(size_t size)
    {
      _buffer = new data_t[size];
      _size = size;
    }

    buffer_t(size_t size, data_t fill_value)
    {
      _buffer = new data_t[size];
      _size = size;
      std::fill(_buffer, _buffer + size, fill_value);
    }

    ~buffer_t()
    {
      delete[] _buffer;
    }

    // Move.
    buffer_t(buffer_t && b)
    {
      _buffer = b._buffer;
      _size = b._size;

      b._buffer = nullptr;
    }

    void operator=(buffer_t && b)
    {
      delete[] _buffer;

      _buffer = b._buffer;
      _size = b._size;

      b._buffer = nullptr;
    }

    // No copy.
    buffer_t(const buffer_t & b) = delete;
    void operator=(const buffer_t & b) = delete;

    // Size.
    size_t  get_size() const { return _size; }

    // Raw buffer access.
    data_t *        data()        { return _buffer; }
    const data_t *  data() const  { return _buffer; }

    // Comparison.
    bool operator==(const buffer_t & other) const { return std::equal(_buffer, _buffer + _size, other._buffer, other._buffer + other._size); };
    bool operator!=(const buffer_t & other) const { return !operator==(other); };

  private:

    data_t *  _buffer;
    size_t    _size;
  };

  using buffer = buffer_t<uint8_t>;
  using buffer8u = buffer_t<uint8_t>;
  using buffer16u = buffer_t<uint16_t>;
}

#endif
