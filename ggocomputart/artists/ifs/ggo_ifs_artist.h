#ifndef __GGO_IFS_ARTIST__
#define __GGO_IFS_ARTIST__

#include <ggo_artist.h>
#include <ggo_array.h>

namespace ggo
{
  class ifs_artist : public artist
  {
  public:

    ifs_artist(int render_width, int render_height);

    void render(void * buffer, float transform[4], float hue, float angle1, float angle2);

  private:

    void update(ggo::pos3f & point, const float transform[4]);

    void paint_buffer(void * buffer, uint8_t color, const ggo::array_float & accumul_buffer) const;
  };
}

#endif


