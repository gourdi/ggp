#ifndef __GGO_RLE_IMAGE__
#define __GGO_RLE_IMAGE__

#include <ggo_image_abc.h>
#include <ggo_rle.h>
#include <ggo_array.h>

namespace ggo
{
  template <typename color_type>
  class rle_image : public image_abc<color_type>
  {
  public:

                              rle_image(int width, int height, const color_type & fill_value = color_type(0), int cache_size = 8);

    void                      write(int x, int y, const color_type & value) override;
    color_type                read(int x, int y) const override;

  private:

    ggo::array<color_type> &  get_cache_line(int y) const;

  private:

    mutable ggo::array<std::vector<std::pair<color_type, int>>> _lines;
    mutable std::vector<std::pair<int, ggo::array<color_type>>> _cache_lines;
            const int                                           _cache_size;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  rle_image<color_type>::rle_image(int width, int height, const color_type & fill_value, int cache_size)
  :
  image_abc<color_type>(width, height),
  _lines(height),
  _cache_size(cache_size)
  {
    for (int i = 0; i < height; ++i)
    {
      _lines[i].push_back(std::make_pair(fill_value, width));
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void rle_image<color_type>::write(int x, int y, const color_type & value)
  {
    auto & cache_line = get_cache_line(y);

    cache_line[x] = value;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  color_type rle_image<color_type>::read(int x, int y) const
  {
    const auto & cache_line = get_cache_line(y);

    return cache_line[x];
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  ggo::array<color_type> & rle_image<color_type>::get_cache_line(int y) const
  {
    GGO_ASSERT_BTW(y, 0, _height - 1);

    // Check if the requiredd line already is in the cache.
    auto find_func = [&](const std::pair<int, ggo::array<color_type>> & cache_line) { return cache_line.first == y; };
    auto it = std::find_if(_cache_lines.begin(), _cache_lines.end(), find_func);

    if (it != _cache_lines.end())
    {
      return it->second;
    }

    // Encode the first cache line if the cache is full.
    if (_cache_lines.size() == _cache_size)
    {
      _lines[_cache_lines.front().first] = rle_encode(_cache_lines.front().second);
      _cache_lines.erase(_cache_lines.begin());
    }

    // Decode the requested line.
    _cache_lines.emplace_back(std::make_pair(y, ggo::array<color_type>(_width)));

    rle_decode(_lines[y], _cache_lines.back().second);

    return _cache_lines.back().second;
  }
}

#endif
