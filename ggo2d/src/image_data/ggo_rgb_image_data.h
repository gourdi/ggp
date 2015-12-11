#ifndef __GGO_RGB_IMAGE_DATA__
#define __GGO_RGB_IMAGE_DATA__

#include <ggo_image_data_abc.h>
#include <ggo_color.h>

namespace ggo
{
  class rgb_image_data_abc : public image_data_abc<ggo::color>
  {
  public:

          rgb_image_data_abc(int width, int height) : ggo::image_data_abc<ggo::color>(width, height) {}
          
    using ggo::image_data_abc<ggo::color>::fill;

    void  fill(const ggo::image_data_abc<float> & gray_image_data)
    {
      if (get_width() != gray_image_data.get_width() ||
          get_height() != gray_image_data.get_height())
      {
        throw dimension_mismatch_exception();
      }
      
      for_each_pixel([&](int x, int y, int width, int height)
      {
        float gray = gray_image_data.unpack(x, y);
        pack(x, y, ggo::color(gray, gray, gray));
      });
    }
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class rgb_image_data_uint8 : public ggo::rgb_image_data_abc
  {
  public:
    
                    rgb_image_data_uint8(uint8_t * buffer, int width, int height);
                    rgb_image_data_uint8(int width, int height);
                    rgb_image_data_uint8(int width, int height, const ggo::color & fill_color);
    virtual        ~rgb_image_data_uint8();

    void            do_pack(int x, int y, const ggo::color & color) override;
    ggo::color      do_unpack(int x, int y) const override;
    
    const uint8_t * get_buffer() const { return _buffer; }
    uint8_t *       get_buffer() { return _buffer; }
    
  private:

    bool      _delete_buffer = false;
    uint8_t * _buffer = nullptr;
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class rgb_image_data_float : public ggo::rgb_image_data_abc
  {
  public:
    
                  rgb_image_data_float(float * buffer, int width, int height);
                  rgb_image_data_float(int width, int height);
    virtual      ~rgb_image_data_float();

    void          do_pack(int x, int y, const ggo::color & color) override;
    ggo::color    do_unpack(int x, int y) const override;
    
    const float * get_buffer() const { return _buffer; }
    float *       get_buffer() { return _buffer; }
    
  private:

    bool    _delete_buffer = false;
    float * _buffer = nullptr;
  };
}

#endif
