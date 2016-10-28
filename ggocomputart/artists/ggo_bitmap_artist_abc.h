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

    static bitmap_artist_abc * create(bitmap_artist_id artist_id);

    bitmap_artist_abc(int render_width, int render_height);

    virtual	void render_bitmap(void * buffer) const = 0;

  protected:

    bitmap_artist_abc();
  };
}

#endif
