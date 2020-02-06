#ifndef __GGO_SONSON_REALTIME_ARTIST__
#define __GGO_SONSON_REALTIME_ARTIST__

#include <ggo_realtime_artist.h>
#include <sonson/ggo_sonson_artist.h>

namespace ggo
{
  class sonson_realtime_artist : public progress_realtime_artist
  {
  public:

    sonson_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void preprocess_frame(void* buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos, float progress) override;
    void render_tile(void * buffer, const ggo::rect_int & clipping) override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_tile_t(void * buffer, const ggo::rect_int & clipping) const;

  private:

    ggo::sonson_artist _artist;
    std::vector<ggo::sonson_artist::glowing_circle> _circles;
  };
}

#endif