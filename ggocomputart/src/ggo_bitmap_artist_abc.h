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

    static bitmap_artist_abc * create(ggo::bitmap_artist_id artist_id, int width, int height, int line_step, ggo::image_format format);

    virtual	void  render_bitmap(void * buffer) const = 0;

  protected:

                  bitmap_artist_abc(int width, int height, int line_step, ggo::image_format format);
                  bitmap_artist_abc() = delete;
  };
}

#endif
