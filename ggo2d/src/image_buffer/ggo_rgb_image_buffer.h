#ifndef __GGO_RGB_IMAGE_BUFFER__
#define __GGO_RGB_IMAGE_BUFFER__

#include <ggo_image_buffer_abc.h>

namespace ggo
{
  template <typename data_type>
  class rgb_image_buffer_abc : public image_buffer_abc<ggo::color, data_type>
  {
  public:

    using ggo::image_abc<ggo::color>::from;

          rgb_image_buffer_abc(int width, int height, data_type * buffer, bool delete_buffer) : image_buffer_abc<ggo::color, data_type>(width, height, buffer, delete_buffer) { }

    void  from(const ggo::gray_image_abc & gray_image)
    {
      if (get_width() != gray_image.get_width() ||
          get_height() != gray_image.get_height())
      {
        throw dimension_mismatch_exception();
      }
      
      for_each_pixel([&](int x, int y)
      {
        float gray = gray_image.read(x, y);
        write(x, y, ggo::color(gray));
      });
    }
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class rgb_image_buffer_uint8 : public rgb_image_buffer_abc<uint8_t>
  {
  public:
    
                rgb_image_buffer_uint8(int width, int height);
                rgb_image_buffer_uint8(int width, int height, uint8_t * buffer);
                rgb_image_buffer_uint8(int width, int height, const ggo::color & color);

    void        write(int x, int y, const ggo::color & color) override;
    ggo::color  read(int x, int y) const override;
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
    
    void        write(int x, int y, const ggo::color & color) override;
    ggo::color  read(int x, int y) const override;
  };
}

#endif
