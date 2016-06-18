#ifndef __GGO_RLE_IMAGE__
#define __GGO_RLE_IMAGE__

#include <ggo_image_abc.h>
#include <ggo_rle.h>
#include <ggo_array.h>

namespace ggo
{
  template <typename color_t>
  class rle_image : public image_abc<color_t>
  {
  public:

                              rle_image(int width, int height, const color_t & fill_value = color_t(0), int cache_size = 8);

    void                      set(int x, int y, const color_t & value) override;
    color_t                   get(int x, int y) const override;

  private:

    ggo::array<color_t, 1> &  get_cache_line(int y) const;

  private:

    mutable ggo::array<std::vector<std::pair<color_t, int>>, 1> _rle_lines;
    mutable std::vector<std::pair<int, ggo::array<color_t, 1>>> _cache_lines;
            const int                                           _cache_size;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  rle_image<color_t>::rle_image(int width, int height, const color_t & fill_value, int cache_size)
  :
  image_abc<color_t>(width, height),
  _rle_lines(height),
  _cache_size(cache_size)
  {
    for (int i = 0; i < height; ++i)
    {
      _rle_lines(i).push_back(std::make_pair(fill_value, width));
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  void rle_image<color_t>::set(int x, int y, const color_t & value)
  {
    auto & cache_line = get_cache_line(y);

    cache_line(x) = value;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  color_t rle_image<color_t>::get(int x, int y) const
  {
    const auto & cache_line = get_cache_line(y);

    return cache_line(x);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  ggo::array<color_t, 1> & rle_image<color_t>::get_cache_line(int y) const
  {
    GGO_ASSERT_BTW(y, 0, _height - 1);

    // Check if the requiredd line already is in the cache.
    auto find_func = [&](const std::pair<int, ggo::array<color_t, 1>> & cache_line) { return cache_line.first == y; };
    auto it = std::find_if(_cache_lines.begin(), _cache_lines.end(), find_func);

    if (it != _cache_lines.end())
    {
      return it->second;
    }

    // Encode the first cache line if the cache is full.
    if (_cache_lines.size() == _cache_size)
    {
      _rle_lines(_cache_lines.front().first) = rle_encode(_cache_lines.front().second);
      _cache_lines.erase(_cache_lines.begin());
    }

    // Decode the requested line.
    _cache_lines.emplace_back(std::make_pair(y, ggo::array<color_t, 1>(this->_width)));

    rle_decode(_rle_lines(y), _cache_lines.back().second);

    return _cache_lines.back().second;
  }
}

#endif
