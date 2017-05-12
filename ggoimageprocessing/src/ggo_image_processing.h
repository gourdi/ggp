#ifndef _GGO_PHOTO_PROCESSOR__
#define _GGO_PHOTO_PROCESSOR__

#include <ggo_pixel_buffer.h>
#include <ggo_shapes2d.h>

namespace ggo
{
  class image_processor final
  {
  public:

    image_processor(const char * filename);

    void  render(void * buffer, int width, int height, int line_byte_step, ggo::pixel_buffer_format pbf, const basis2d<float> & view_basis) const;

  private:

    ggo::pixel_buffer _pixel_buffer;
  };
}

#endif
