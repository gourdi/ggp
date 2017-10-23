#ifndef __GGO_BUFFER__
#define __GGO_BUFFER__

#include <ggo_kernel.h>
#include <cstring>

namespace ggo
{
  class buffer final
  {
  public:

    buffer(size_t size)
    {
      _buffer = new uint8_t[size];
      _size = size;
    }

    buffer(size_t size, uint8_t fill_value)
    {
      _buffer = new uint8_t[size];
      _size = size;
      std::fill(_buffer, _buffer + size, fill_value);
    }

    ~buffer()
    {
      delete[] _buffer;
    }

    // Move.
    buffer(buffer && b)
    {
      _buffer = b._buffer;
      _size = b._size;

      b._buffer = nullptr;
    }

    void operator=(buffer && b)
    {
      delete[] _buffer;

      _buffer = b._buffer;
      _size = b._size;

      b._buffer = nullptr;
    }

    // No copy.
    buffer(const buffer & b) = delete;
    void operator=(const buffer & b) = delete;

    // Size.
    size_t  get_size() const { return _size; }

    // Raw buffer access.
    void *        data()        { return _buffer; }
    const void *  data() const  { return _buffer; }

    // Comparison.
    bool operator==(const buffer & other) const { return std::equal(_buffer, _buffer + _size, other._buffer, other._buffer + other._size); };
    bool operator!=(const buffer & other) const { return !operator==(other); };

  private:

    uint8_t * _buffer;
    size_t    _size;
  };
}

#endif
