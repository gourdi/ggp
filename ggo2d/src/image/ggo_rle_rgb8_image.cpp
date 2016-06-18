#include "ggo_rle_rgb8_image.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  rle_rgb8_image::rle_rgb8_image(int width, int height, const ggo::color & fill_value, int cache_size)
  :
  image_abc<ggo::color>(width, height),
  _rle_lines(height),
  _cache_size(cache_size)
  {
    auto fill_tuple = std::make_tuple(fill_value.r8(), fill_value.g8(), fill_value.b8());

    for (int i = 0; i < height; ++i)
    {
      _rle_lines(i).push_back(std::make_pair(fill_tuple, width));
    }
  }

  /////////////////////////////////////////////////////////////////////
  void rle_rgb8_image::set(int x, int y, const ggo::color & value)
  {
    auto & cache_line = get_cache_line(y);

    uint8_t * ptr = cache_line.data() + 3 * x;
    ptr[0] = value.r8();
    ptr[1] = value.g8();
    ptr[2] = value.b8();
  }

  /////////////////////////////////////////////////////////////////////
  ggo::color rle_rgb8_image::get(int x, int y) const
  {
    const auto & cache_line = get_cache_line(y);

    const uint8_t * ptr = cache_line.data() + 3 * x;

    return ggo::color(ptr[0] / 255.f, ptr[1] / 255.f, ptr[2] / 255.f);
  }

  /////////////////////////////////////////////////////////////////////
  ggo::array<uint8_t, 1> & rle_rgb8_image::rle_rgb8_image::get_cache_line(int y) const
  {
    GGO_ASSERT_BTW(y, 0, _height - 1);

    // Check if the required line already is in the cache.
    auto find_func = [&](const std::pair<int, ggo::array<uint8_t, 1>> & cache_line) { return cache_line.first == y; };
    auto it = std::find_if(_cache_lines.begin(), _cache_lines.end(), find_func);

    if (it != _cache_lines.end())
    {
      return it->second;
    }

    // Encode the first cache line if the cache is full and remove it from the cache.
    if (_cache_lines.size() == _cache_size)
    {
      const uint8_t * ptr = _cache_lines.front().second.data();
      const uint8_t * end = _cache_lines.front().second.data() + 3 * get_width();

      auto read_func = [&]()
      {
        auto current_pixel = std::make_tuple(ptr[0], ptr[1], ptr[2]);
        ptr += 3;
        return current_pixel;
      };

      auto done_func = [&]()
      {
        return ptr == end;
      };

      _rle_lines(_cache_lines.front().first) = rle_encode<std::tuple<uint8_t, uint8_t, uint8_t>>(read_func, done_func);

      _cache_lines.erase(_cache_lines.begin());
    }

    // Decode the requested line.
    _cache_lines.emplace_back(std::make_pair(y, ggo::array<uint8_t, 1>(3 * this->_width)));

    uint8_t * ptr = _cache_lines.back().second.data();
    auto write_func = [&](const std::tuple<uint8_t, uint8_t, uint8_t> & pixel, int count)
    {
      while (count > 0)
      {
        ptr[0] = std::get<0>(pixel);
        ptr[1] = std::get<1>(pixel);
        ptr[2] = std::get<2>(pixel);
        ptr += 3;
        --count;
      }
    };

    rle_decode(_rle_lines(y), write_func);

    return _cache_lines.back().second;
  }
}
