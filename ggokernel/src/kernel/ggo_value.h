#ifndef __GGO_VALUE__
#define __GGO_VALUE__

#include <variant>
#include <kernel/ggo_vec.h>

namespace ggo
{
  class value final
  {
  public:

    // Single value interface.
    template <typename data_t>
    value(data_t v) : _variant(v) {}

    template <typename data_t>
    const data_t & to() const { return std::get<data_t>(_variant); }

    // Container interface.
    template <typename iterator_t>
    value(iterator_t begin, iterator_t end)
    {
      using data_t = std::iterator_traits<iterator_t>::value_type;
      const size_t size = static_cast<size_t>(std::distance(begin, end));

      buffer<data_t> buf{ new data_t[size], size };

      std::copy(begin, end, buf._buffer);

      _variant = buf;
      buf._buffer = nullptr; // Not to delete the buffer when the temporary is destroyed.
    }

    template <typename data_t>
    struct range
    {
      const data_t *  _begin;
      const data_t *  _end;

      const data_t *  begin() const { return _begin; }
      const data_t *  end() const { return _begin; }
    };

    template <typename data_t>
    const range<data_t> to_range_of() const
    { 
      range<data_t> r;
      r._begin = std::get<value::buffer<data_t>>(_variant)._buffer;
      r._end = std::get<value::buffer<data_t>>(_variant)._buffer + std::get<value::buffer<data_t>>(_variant)._size;

      return r;
    }

  private:

    template <typename data_t>
    struct buffer
    {
      data_t * _buffer = nullptr;
      size_t _size = 0;

      buffer(data_t * b, size_t s) : _buffer(b), _size(s) { }

      ~buffer() { delete[] _buffer; }
    };

    std::variant<int, float, pos2f, buffer<int>, buffer<float>, buffer<pos2f>> _variant;
  };
}

#endif
