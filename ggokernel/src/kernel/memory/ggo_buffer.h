#ifndef __GGO_BUFFER__
#define __GGO_BUFFER__

#include <kernel/ggo_kernel.h>
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

    buffer_t(const std::initializer_list<data_t> & list)
    {
      _buffer = new data_t[list.size()];
      _size = list.size();
      std::copy(list.begin(), list.end(), _buffer);
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
    size_t  size() const { return _size; }

    // Raw buffer access.
    data_t *        data()        { return _buffer; }
    const data_t *  data() const  { return _buffer; }

    // Comparison.
    bool operator==(const buffer_t & other) const { return std::equal(_buffer, _buffer + _size, other._buffer, other._buffer + other._size); };
    bool operator!=(const buffer_t & other) const { return !operator==(other); };

    // Iteration.
    const data_t * begin() const { return _buffer; };
    const data_t * end() const { return _buffer + _size; };
    data_t * begin() { return _buffer; };
    data_t * end() { return _buffer + _size; };

    // Operator[]
    data_t &  operator[](int i) { return _buffer[i]; }
    const data_t &  operator[](int i) const { return _buffer[i]; }

  private:

    data_t *  _buffer;
    size_t    _size;
  };

  using buffer = buffer_t<uint8_t>;
  using buffer8u = buffer_t<uint8_t>;
  using buffer16u = buffer_t<uint16_t>;
}

#endif
