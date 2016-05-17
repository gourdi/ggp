#ifndef __GGO_GRAY_IMAGE_BUFFER__
#define __GGO_GRAY_IMAGE_BUFFER__

#include <ggo_image_buffer_abc.h>

namespace ggo
{
  template <typename data_t>
  class gray_image_buffer_abc : public image_buffer_abc<float, data_t>
  {
  public:

                    gray_image_buffer_abc(int width, int height, data_t * buffer, bool delete_buffer) : image_buffer_abc<float, data_t>(width, height, buffer, delete_buffer) { }

    data_t          operator[](int i) const { return this->_buffer[i]; }
    data_t &        operator[](int i) { return this->_buffer[i]; }

    data_t *        begin() { return this->_buffer; }
    data_t *        end() { return this->_buffer + this->get_width() * this->get_height(); }

    const data_t *  begin() const { return this->_buffer; }
    const data_t *  end() const { return this->_buffer + this->get_width() * this->get_height(); }
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

    void  set(int x, int y, const float & value) override;
    float get(int x, int y) const override;
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

    void  set(int x, int y, const float & value) override;
    float get(int x, int y) const override;
  };
}

#endif
