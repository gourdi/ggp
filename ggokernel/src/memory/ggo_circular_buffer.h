#ifndef __GGO_CIRCULAR_BUFFER__
#define __GGO_CIRCULAR_BUFFER__

#include <vector>
#include <ggo_kernel.h>

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
#ifndef NDEBUG
      ++_size;
      GGO_ASSERT(_size <= _buffer.size());
#endif
    }

    data_t pop()
    {
#ifndef NDEBUG
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

  private:

    std::vector<data_t> _buffer;
    int                 _push_index = 0;
    int                 _pop_index = 0;
#ifndef NDEBUG
    int                 _size = 0;
#endif
  };
}

#endif