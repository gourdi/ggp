#ifndef __GGO_POLYGUS_BITMAP_ARTIST__
#define __GGO_POLYGUS_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class polygus_bitmap_artist : public bitmap_artist_abc
  {
  public:

    polygus_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_bitmap(void * buffer) const override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_polygus(void * buffer) const;

    std::vector<ggo::pos2_f> create_head() const;
  };
}

#endif
