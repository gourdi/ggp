#ifndef __GGO_TOPODOKO_BITMAP_ARTIST__
#define __GGO_TOPODOKO_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class topodoko_artist : public bitmap_artist_abc
  {
  public:

          topodoko_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    struct color_square
    {
      ggo::polygon2d_f	_square;
      ggo::rgb_32f      _color;
    };

  private:

    void	render_bitmap(void * buffer) const override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_t(void * buffer, float hue1, float hue2, float square_size, const std::vector<color_square> & color_squares) const;
  };
}

#endif
