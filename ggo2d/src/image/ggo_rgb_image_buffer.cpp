#include "ggo_rgb_image_buffer.h"
#include <ggo_fill.h>

/////////////////////////////////////////////////////////////////////
// rgb_image_buffer_uint8
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_uint8::rgb_image_buffer_uint8(int width, int height)
  :
  rgb_image_buffer_abc<uint8_t>(width, height, new uint8_t[3 * width * height], true)
  {

  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_uint8::rgb_image_buffer_uint8(int width, int height, uint8_t * buffer)
  :
  rgb_image_buffer_abc<uint8_t>(width, height, buffer, false)
  {

  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_uint8::rgb_image_buffer_uint8(int width, int height, const ggo::color & color)
  :
  rgb_image_buffer_abc<uint8_t>(width, height, new uint8_t[3 * width * height], true)
  {
    fill_solid_rgb(_buffer, width * height, color);
  }

  /////////////////////////////////////////////////////////////////////
  void rgb_image_buffer_uint8::write(int x, int y, const ggo::color & color)
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);
    
    uint8_t * ptr = _buffer + 3 * (_width * y + x);
    
    ptr[0] = color.r8();
    ptr[1] = color.g8();
    ptr[2] = color.b8();
  }

  /////////////////////////////////////////////////////////////////////
  ggo::color rgb_image_buffer_uint8::read(int x, int y) const
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);
    
    const uint8_t * ptr = _buffer + 3 * (_width * y + x);
    
    return ggo::color(ptr[0] / 255.f, ptr[1] / 255.f, ptr[2] / 255.f);
  }
}

/////////////////////////////////////////////////////////////////////
// rgb_image_buffer_float
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_float::rgb_image_buffer_float(int width, int height)
  :
  rgb_image_buffer_abc<float>(width, height, new float[3 * width * height], true)
  {

  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_float::rgb_image_buffer_float(int width, int height, float * buffer)
  :
  rgb_image_buffer_abc<float>(width, height, buffer, false)
  {

  }

  /////////////////////////////////////////////////////////////////////
  rgb_image_buffer_float::rgb_image_buffer_float(int width, int height, const ggo::color & fill_value)
  :
  rgb_image_buffer_abc<float>(width, height, new float[3 * width * height], true)
  {
    float * it = _buffer;
    for (int i = 0; i < width * height; ++i)
    {
      it[0] = fill_value.r();
      it[1] = fill_value.g();
      it[2] = fill_value.b();
    }
  }


  /////////////////////////////////////////////////////////////////////
  void rgb_image_buffer_float::write(int x, int y, const ggo::color & color)
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);
    
    float * ptr = _buffer + 3 * (_width * y + x);
    
    ptr[0] = color.r();
    ptr[1] = color.g();
    ptr[2] = color.b();
  }

  /////////////////////////////////////////////////////////////////////
  ggo::color rgb_image_buffer_float::read(int x, int y) const
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);
    
    const float * ptr = _buffer + 3 * (_width * y + x);
    
    return ggo::color(ptr[0], ptr[1], ptr[2]);
  }
}
