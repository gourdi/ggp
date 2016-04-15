#include "ggo_rgb_image_data.h"
#include <ggo_fill.h>

/////////////////////////////////////////////////////////////////////
// rgb_image_buffer_uint8
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_uint8::rgb_image_buffer_uint8(uint8_t * buffer, int width, int height)
  :
  rgb_image_buffer_abc(width, height),
  _delete_buffer(false),
  _buffer(buffer)
  {
  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_uint8::rgb_image_buffer_uint8(int width, int height)
  :
  rgb_image_buffer_abc(width, height),
  _delete_buffer(true),
  _buffer(new uint8_t[3 * width * height])
  {
  }
  
  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_uint8::rgb_image_buffer_uint8(int width, int height, const ggo::color & fill_color)
  :
  rgb_image_buffer_uint8(width, height)
  {
    fill_solid_rgb(_buffer, width * height, fill_color);
  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_uint8::~rgb_image_buffer_uint8()
  {
    if (_delete_buffer)
    {
      delete[] _buffer;
    }
  }

  /////////////////////////////////////////////////////////////////////
  void rgb_image_buffer_uint8::do_pack(int x, int y, const ggo::color & color)
  {
    GGO_ASSERT(_buffer);
    
    uint8_t * ptr = _buffer + 3 * (get_width() * y + x);
    
    ptr[0] = color.r8();
    ptr[1] = color.g8();
    ptr[2] = color.b8();
  }

  /////////////////////////////////////////////////////////////////////
  ggo::color rgb_image_buffer_uint8::do_unpack(int x, int y) const
  {
    GGO_ASSERT(_buffer);
    
    const uint8_t * ptr = _buffer + 3 * (get_width() * y + x);
    
    return ggo::color(ptr[0] / 255.f, ptr[1] / 255.f, ptr[2] / 255.f);
  }
}

/////////////////////////////////////////////////////////////////////
// rgb_image_buffer_float
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_float::rgb_image_buffer_float(float * buffer, int width, int height)
  :
  rgb_image_buffer_abc(width, height),
  _delete_buffer(false),
  _buffer(buffer)
  {
  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_float:: rgb_image_buffer_float(int width, int height)
  :
  rgb_image_buffer_abc(width, height),
  _delete_buffer(true),
  _buffer(new float[3 * width * height])
  {
  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_float::~rgb_image_buffer_float()
  {
    if (_delete_buffer)
    {
      delete[] _buffer;
    }
  }

  /////////////////////////////////////////////////////////////////////
  void rgb_image_buffer_float::do_pack(int x, int y, const ggo::color & color)
  {
    GGO_ASSERT(_buffer);
    
    float * ptr = _buffer + 3 * (get_width() * y + x);
    
    ptr[0] = color.r();
    ptr[1] = color.g();
    ptr[2] = color.b();
  }

  /////////////////////////////////////////////////////////////////////
  ggo::color rgb_image_buffer_float::do_unpack(int x, int y) const
  {
    GGO_ASSERT(_buffer);
    
    const float * ptr = _buffer + 3 * (get_width() * y + x);
    
    return ggo::color(ptr[0], ptr[1], ptr[2]);
  }
}
