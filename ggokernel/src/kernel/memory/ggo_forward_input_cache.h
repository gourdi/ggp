#ifndef __GGO_FORWARD_INPUT_CACHE__
#define __GGO_FORWARD_INPUT_CACHE__

#include <kernel/ggo_convert.h>
#include <kernel/ggo_borders.h>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  template <typename data_t, ggo::border_mode border, typename buffer_t = data_t, typename convert_t = passthrough_conversion<data_t>>
  class forward_input_cache
  {
  public:

    static_assert(std::is_same_v<buffer_t, convert_t::input_t>);
    static_assert(std::is_same_v<data_t, convert_t::output_t>);

    forward_input_cache(int samples_count, int kernel_half_size)
      :
      _samples_count(samples_count),
      _kernel_half_size(kernel_half_size),
      _cache(samples_count + 2 * (kernel_half_size - 1))
    {
    }

    template <typename read_func>
    void setup(read_func read)
    {
      // Fill the cache.
      buffer_t * cache_ptr = _cache.data();
      int begin = -_kernel_half_size + 1;
      int end = _samples_count + _kernel_half_size - 1;
      int i = begin;
      for (; i < 0; ++i)
      {
        int ii = ggo::index<border>(i, _samples_count);
        *cache_ptr++ = read(ii);
      }
      for (; i < _samples_count; ++i)
      {
        *cache_ptr++ = read(i);
      }
      for (; i < end; ++i)
      {
        int ii = ggo::index<border>(i, _samples_count);
        *cache_ptr++ = read(ii);
      }
      GGO_ASSERT(cache_ptr == _cache.end());

      // Set the pointer.
      _input_ptr = _cache.data() + _kernel_half_size - 1;
    }

    data_t read(int offset) const
    {
      GGO_ASSERT(offset > -_kernel_half_size && offset < _kernel_half_size);
      GGO_ASSERT(_input_ptr + offset >= _cache.begin() && _input_ptr + offset < _cache.end());
      return convert_t::convert(_input_ptr[offset]);
    }

    void move_to_next()
    {
      _input_ptr += 1;
    }

  private:

    const int _samples_count;
    const int _kernel_half_size;
    ggo::array<buffer_t, 1> _cache;
    const buffer_t * _input_ptr = nullptr;
  };
}

#endif
