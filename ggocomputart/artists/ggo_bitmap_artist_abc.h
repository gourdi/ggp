#ifndef __GGO_BITMAP_ARTIST_ABC__
#define __GGO_BITMAP_ARTIST_ABC__

#include <ggo_artist.h>
#include <ggo_artist_ids.h>

//////////////////////////////////////////////////////////////
namespace ggo
{
  class bitmap_artist_abc : public artist
  {
  public:

    static bitmap_artist_abc * create(ggo::bitmap_artist_id artist_id, int width, int height, int line_step, ggo::pixel_buffer_format pbf);

    virtual	void  render_bitmap(void * buffer) const = 0;

    int                       get_line_step() const { return _line_step; }
    ggo::pixel_buffer_format  get_pixel_buffer_format() const { return _pbf; }

  protected:

                  bitmap_artist_abc(int width, int height, int line_step, ggo::pixel_buffer_format pbf);
                  bitmap_artist_abc() = delete;

  private:

    const int _line_step;
    const ggo::pixel_buffer_format _pbf;
  };
}

#endif
