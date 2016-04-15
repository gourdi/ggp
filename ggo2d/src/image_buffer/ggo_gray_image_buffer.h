#ifndef __GGO_GRAY_IMAGE_BUFFER__
#define __GGO_GRAY_IMAGE_BUFFER__

#include <ggo_image_buffer_abc.h>

namespace ggo
{
  template <typename data_type>
  class gray_image_buffer_abc : public image_buffer_abc<float, data_type>
  {
  public:

                      gray_image_buffer_abc(int width, int height, data_type * buffer, bool delete_buffer) : image_buffer_abc<float, data_type>(width, height, buffer, delete_buffer) { }

    data_type         operator[](int i) const { return _buffer[i]; }
    data_type &       operator[](int i) { return _buffer[i]; }

    data_type *       begin() { return _buffer; }
    data_type *       end() { return _buffer + get_width() * get_height(); }

    const data_type * begin() const { return _buffer; }
    const data_type * end() const { return _buffer + get_width() * get_height(); }
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class gray_image_buffer_uint8 : public gray_image_buffer_abc<uint8_t>
  {
  public:

          gray_image_buffer_uint8(int width, int height);
          gray_image_buffer_uint8(int width, int height, uint8_t * buffer);
          gray_image_buffer_uint8(int width, int height, float fill_value);

    void  write(int x, int y, const float & value) override;
    float read(int x, int y) const override;
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class gray_image_buffer_float : public gray_image_buffer_abc<float>
  {
  public:

          gray_image_buffer_float(int width, int height);
          gray_image_buffer_float(int width, int height, float * buffer);
          gray_image_buffer_float(int width, int height, float fill_value);

    void  write(int x, int y, const float & value) override;
    float read(int x, int y) const override;
  };
}

#endif
