#ifndef __GGO_RGB_IMAGE_BUFFER__
#define __GGO_RGB_IMAGE_BUFFER__

#include <ggo_image_buffer_abc.h>

namespace ggo
{
  template <typename data_t>
  class rgb_image_buffer_abc : public image_buffer_abc<ggo::color, data_t>
  {
  public:

    using ggo::image_abc<ggo::color>::from;

    rgb_image_buffer_abc(int width, int height, data_t * buffer, bool delete_buffer) : image_buffer_abc<ggo::color, data_t>(width, height, buffer, delete_buffer) { }
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class rgb_image_buffer_uint8 : public rgb_image_buffer_abc<uint8_t>
  {
  public:

    using image_abc::set;
    
                rgb_image_buffer_uint8(int width, int height);
                rgb_image_buffer_uint8(int width, int height, uint8_t * buffer);
                rgb_image_buffer_uint8(int width, int height, const ggo::color & color);

    void        set(int x, int y, const ggo::color & color) override;
    ggo::color  get(int x, int y) const override;
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class rgb_image_buffer_float : public rgb_image_buffer_abc<float>
  {
  public:

                rgb_image_buffer_float(int width, int height);
                rgb_image_buffer_float(int width, int height, float * buffer);
                rgb_image_buffer_float(int width, int height, const ggo::color & fill_value);
    
    void        set(int x, int y, const ggo::color & color) override;
    ggo::color  get(int x, int y) const override;
  };
}

#endif
