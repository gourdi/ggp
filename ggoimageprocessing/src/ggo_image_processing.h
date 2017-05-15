#ifndef _GGO_IMAGE_PROCESSING__
#define _GGO_IMAGE_PROCESSING__

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

  ggo::rect_data<float> get_view_pixel_rect(const ggo::basis2d<float> & view_basis, const ggo::pos2i & pixel);
  
  ggo::basis2d<float> compute_fit_view_basis(int image_width, int image_height, int view_width, int view_height);
}

#endif
