#ifndef __GGO_IFS_ARTIST__
#define __GGO_IFS_ARTIST__

#include <ggo_artist.h>
#include <ggo_array.h>

namespace ggo
{
  class ifs_artist : public artist
  {
  public:

          ifs_artist(int width, int height);

    void  render(void * buffer, int line_step, ggo::pixel_buffer_format pbf, float transform[4], float hue, float angle1, float angle2) const;

  private:

    void  update(ggo::pos3f & point, const float transform[4]) const;

    void  paint_buffer(void * buffer, int line_step, ggo::pixel_buffer_format pbf, uint8_t color, const ggo::array_float & accumul_buffer) const;
  };
}

#endif


