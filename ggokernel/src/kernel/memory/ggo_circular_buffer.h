#ifndef __GGO_CIRCULAR_BUFFER__
#define __GGO_CIRCULAR_BUFFER__

#include <vector>
#include <sstream>
#include <kernel/ggo_kernel.h>

namespace ggo
{
  template <typename data_t>
  class circular_buffer
  {
  public:

    circular_buffer(int size) : _buffer(size) {}

    void push(const data_t & v)
    {
      _buffer[_push_index] = v;
      ++_push_index;
      if (_push_index == _buffer.size())
      {
        _push_index = 0;
      }
#ifdef GGO_DEBUG
      ++_size;
      GGO_ASSERT(_size <= _buffer.size());
#endif
    }

    data_t pop()
    {
#ifdef GGO_DEBUG
      --_size;
      GGO_ASSERT(_size >= 0);
#endif
      auto v = _buffer[_pop_index];
      ++_pop_index;
      if (_pop_index == _buffer.size())
      {
        _pop_index = 0;
      }
      return v;
    }

    const data_t * data() const { return _buffer.data(); }

    std::string to_string() const
    {
      std::ostringstream oss;
#ifdef GGO_DEBUG
      oss << '(';

      if (_size > 0)
      {
        oss << _buffer[_pop_index];
        int i = _pop_index + 1;
        while (true)
        {
          if (i >= _buffer.size())
          {
            i = 0;
          }
          if (i == _push_index)
          {
            break;
          }

          oss << ", " << _buffer[i];
          i++;
        }
      }

      oss << ')';
#endif
      return oss.str();
    }

  private:

    std::vector<data_t> _buffer;
    int                 _push_index = 0;
    int                 _pop_index = 0;
#ifdef GGO_DEBUG
    int                 _size = 0;
#endif
  };
}

namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const ggo::circular_buffer<data_t> & cb)
  {
    os << cb.to_string();
    return os;
  }
}

#endif