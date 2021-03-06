#pragma once

#include <kernel/rle/ggo_rle.h>

namespace ggo
{
  template <typename color_t_>
  class rle_image
  {
  public:

    using color_t = color_t_;

                              rle_image(int width, int height, const color_t & fill_value = color_t(0), int cache_size = 8);

    void                      write_pixel(int x, int y, const color_t & value);
    color_t                   read_pixel(int x, int y) const;

    int                       width() const { return _width; }
    int                       height() const { return static_cast<int>(_rle_lines.size()); }
    ggo::size                 size() const { return { width(), height() }; }

    // No copy.
                              rle_image(const rle_image<color_t> & rhs) = delete;
    void                      operator=(const rle_image<color_t> & rhs) = delete;

  private:

    ggo::array<color_t, 1> &  get_cache_line(int y) const;

  private:

    mutable std::vector<std::vector<std::pair<color_t, int>>>   _rle_lines;
    mutable std::vector<std::pair<int, ggo::array<color_t, 1>>> _cache_lines;
            const int                                           _cache_size;
            const int                                           _width;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  rle_image<color_t>::rle_image(int width, int height, const color_t & fill_value, int cache_size)
  :
  _rle_lines(height),
  _cache_size(cache_size),
  _width(width)
  {
    for (int i = 0; i < height; ++i)
    {
      _rle_lines[i].push_back(std::make_pair(fill_value, width));
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  void rle_image<color_t>::write_pixel(int x, int y, const color_t & value)
  {
    auto & cache_line = get_cache_line(y);

    cache_line(x) = value;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  color_t rle_image<color_t>::read_pixel(int x, int y) const
  {
    const auto & cache_line = get_cache_line(y);

    return cache_line(x);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  ggo::array<color_t, 1> & rle_image<color_t>::get_cache_line(int y) const
  {
    GGO_ASSERT_BTW(y, 0, height() - 1);

    // Check if the required line already is in the cache.
    auto find_func = [&](const std::pair<int, ggo::array<color_t, 1>> & cache_line) { return cache_line.first == y; };
    auto it = std::find_if(_cache_lines.begin(), _cache_lines.end(), find_func);

    if (it != _cache_lines.end())
    {
      return it->second;
    }

    // Encode the first cache line if the cache is full.
    if (_cache_lines.size() == _cache_size)
    {
      _rle_lines[_cache_lines.front().first] = rle_encode(_cache_lines.front().second);
      _cache_lines.erase(_cache_lines.begin());
    }

    // Decode the requested line.
    _cache_lines.emplace_back(std::make_pair(y, ggo::array<color_t, 1>(this->_width)));

    rle_decode(_rle_lines[y], _cache_lines.back().second);

    return _cache_lines.back().second;
  }
}

