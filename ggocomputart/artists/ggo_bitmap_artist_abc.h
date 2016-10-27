#ifndef __GGO_BITMAP_ARTIST_ABC__
#define __GGO_BITMAP_ARTIST_ABC__

#include <ggo_artist_abc.h>
#include <ggo_artist_ids.h>

//////////////////////////////////////////////////////////////
namespace ggo
{
  class bitmap_artist_abc
  {
  public:

    static bitmap_artist_abc * create(bitmap_artist_id artist_id);

    virtual	void render_bitmap(void * buffer, int render_width, int render_height) = 0;

  protected:

    bitmap_artist_abc();
  };
}

#endif
