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

                rle_image(int width, int height, const color_type & fill_value = color_type(0));

    void        write(int x, int y, const color_type & value) override;
    color_type  read(int x, int y) const override;

  private:

    void        update_cache(int y) const;

  private:

    mutable ggo::array<std::vector<std::pair<color_type, int>>> _lines;
    mutable ggo::array<color_type>                              _cache_line;
    mutable int                                                 _cache_index = -1;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  rle_image<color_type>::rle_image(int width, int height, const color_type & fill_value)
  :
  image_abc(width, height),
  _lines(height),
  _cache_line(width)
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
    update_cache(y);

    _cache_line[x] = value;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  color_type rle_image<color_type>::read(int x, int y) const
  {
    update_cache(y);

    return _cache_line[x];
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void rle_image<color_type>::update_cache(int y) const
  {
    GGO_ASSERT_BTW(y, 0, _height - 1);

    // Check if the current cache line is the right one.
    if (_cache_index != y)
    {
      // If not, the current cache must be RLE encoded.
      if (_cache_index >= 0)
      {
        _lines[_cache_index] = rle_encode(_cache_line);
      }

      // Then the requested line must be RLE decoded.
      rle_decode(_lines[y], _cache_line);

      _cache_index = y;
    }
  }
}

#endif
