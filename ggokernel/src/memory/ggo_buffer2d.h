#ifndef __GGO_BUFFER2D__
#define __GGO_BUFFER2D__

#include <ggo_kernel.h>
#include <cstring>

namespace ggo
{
  template <typename data_t = uint8_t>
  class buffer2d_t final
  {
  public:

    buffer2d_t(size_t width, size_t height)
    {
      _buffer = new data_t[width * height];
      _width = width;
      _height = height;
    }

    buffer2d_t(size_t width, size_t height, data_t fill_value)
    {
      _buffer = new uint8_t[line_byte_step * height];
      _width = width;
      _height = height;

      std::fill(_buffer, _buffer + width * height, fill_value);
    }

    buffer2d_t(const std::initializer_list<std::initializer_list<data_t>> & lines)
    {
      _height = lines.size();
      _width = lines.begin()->size();
      _buffer = new data_t[_width * _height];

      data_t * it = _buffer;
      for (const auto& line : lines)
      {
        if (line.size() != _width)
        {
          throw std::runtime_error("invalid line size");
        }

        for (const auto& v : line)
        {
          *it++ = v;
        }
      }
    }

    ~buffer2d_t()
    {
      delete[] _buffer;
    }

    // Move.
    buffer2d_t(buffer2d_t && b)
    {
      _buffer = b._buffer;
      _width = b._width;
      _height = b._height;

      b._buffer = nullptr;
    }

    void operator=(buffer2d_t && b)
    {
      delete[] _buffer;

      _buffer = b._buffer;
      _width = b._width;
      _height = b._height;

      b._buffer = nullptr;
    }

    // No copy.
    buffer2d_t(const buffer2d_t & b) = delete;
    void operator=(const buffer2d_t & b) = delete;

    // Size.
    size_t  width() const { return _width; }
    size_t  height() const { return _height; }
    size_t  line_byte_step() const { return _width * sizeof(data_t); }

    // Raw buffer access.
    data_t *        data() { return _buffer; }
    const data_t *  data() const { return _buffer; }

    // Comparison.
    bool operator==(const buffer2d_t & other) const
    {
      return _width == other._width && _height == other._height &&
        std::equal(_buffer, _buffer + _width * _height, other._buffer);
    };
    bool operator!=(const buffer2d_t & other) const { return !operator==(other); };

    template <typename predicate> bool equal(const buffer2d_t & other, predicate p) const
    { 
      return _width == other._width && _height == other._height &&
      std::equal(_buffer, _buffer + _width * _height, other._buffer, p);
    };

    // Iteration.
    const data_t * begin() const { return _buffer; };
    const data_t * end() const { return _buffer + _width * _height; };
    data_t * begin() { return _buffer; };
    data_t * end() { return _buffer + _width * _height; };

    // Random access.
    const data_t &  operator()(int x, int y) const { return _buffer[y * _width + x]; }
    data_t &        operator()(int x, int y) { return _buffer[y * _width + x]; }

  private:

    data_t *  _buffer;
    size_t    _width;
    size_t    _height;
  };

  using buffer2d = buffer2d_t<uint8_t>;
  using buffer2d_8u = buffer2d_t<uint8_t>;
  using buffer2d_16u = buffer2d_t<uint16_t>;
  using buffer2d_32s = buffer2d_t<int32_t>;
  using buffer2d_32f = buffer2d_t<float>;
}

#endif
