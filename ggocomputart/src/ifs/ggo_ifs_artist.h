#ifndef __GGO_IFS_ARTIST__
#define __GGO_IFS_ARTIST__

#include <ggo_artist.h>
#include <kernel/ggo_vec3.h>
#include <2d/ggo_image.h>

namespace ggo
{
  class ifs_artist : public artist
  {
  public:

          ifs_artist(int width, int height);

    void  render(void * buffer, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
      float transform[4], float hue, float angle1, float angle2) const;

  private:

    void  update(ggo::pos3_f & point, const float transform[4]) const;

    void  paint_buffer(void * buffer, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
      uint8_t color, const ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> & accumul_buffer) const;
  };
}

#endif


