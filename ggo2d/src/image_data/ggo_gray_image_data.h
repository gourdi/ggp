#ifndef __GGO_GRAY_IMAGE_DATA__
#define __GGO_GRAY_IMAGE_DATA__

#include <ggo_image_data_abc.h>

namespace ggo
{
  class gray_image_data_abc : public image_data_abc<float>
  {
  public:

    gray_image_data_abc(int width, int height) : ggo::image_data_abc<float>(width, height) {}
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class gray_image_data_uint8 : public gray_image_data_abc
  {
  public:

                    gray_image_data_uint8(uint8_t * buffer, int width, int height);
                    gray_image_data_uint8(int width, int height);
    virtual        ~gray_image_data_uint8();

    void            do_pack(int x, int y, const float & value) override;
    float           do_unpack(int x, int y) const override;
    
    const uint8_t * get_buffer() const { return _buffer; }
    uint8_t *       get_buffer() { return _buffer; }
    
    uint8_t         operator[](int i) const { return _buffer[i]; }
    uint8_t &       operator[](int i) { return _buffer[i]; }
    
  private:

    bool      _delete_buffer = false;
    uint8_t * _buffer = nullptr;
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class gray_image_data_float : public gray_image_data_abc
  {
  public:

                  gray_image_data_float(float * buffer, int width, int height);
                  gray_image_data_float(int width, int height);
    virtual      ~gray_image_data_float();
        
    void          do_pack(int x, int y, const float & value) override;
    float         do_unpack(int x, int y) const override;
    
    const float * get_buffer() const { return _buffer; }
    float *       get_buffer() { return _buffer; }
    
    float         operator[](int i) const { return _buffer[i]; }
    float &       operator[](int i) { return _buffer[i]; }

  private:

    bool    _delete_buffer = false;
    float * _buffer = nullptr;
  };
}

#endif
