#include "ggo_gray_image_buffer.h"
#include <ggo_fill.h>

/////////////////////////////////////////////////////////////////////
// gray_image_buffer_uint8
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_uint8::gray_image_buffer_uint8(int width, int height)
  :
  gray_image_buffer_abc<uint8_t>(width, height, new uint8_t[width * height], true)
  {

  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_uint8::gray_image_buffer_uint8(int width, int height, uint8_t * buffer)
  :
  gray_image_buffer_abc<uint8_t>(width, height, buffer, false)
  {

  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_uint8::gray_image_buffer_uint8(int width, int height, float fill_value)
  :
  gray_image_buffer_abc<uint8_t>(width, height, new uint8_t[width * height], true)
  {
    std::fill(_buffer, _buffer + width * height, ggo::to<uint8_t>(fill_value * 255.f));
  }
  
  /////////////////////////////////////////////////////////////////////
  void gray_image_buffer_uint8::write(int x, int y, const float & value)
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);

    _buffer[y * _width + x] = ggo::to<uint8_t>(value * 255.f);
  }
      
  /////////////////////////////////////////////////////////////////////
  float gray_image_buffer_uint8::read(int x, int y) const
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);

    return _buffer[y * _width + x] / 255.f;
  }
}

/////////////////////////////////////////////////////////////////////
// gray_image_buffer_float
namespace ggo
{
  gray_image_buffer_float::gray_image_buffer_float(int width, int height)
    :
    gray_image_buffer_abc<float>(width, height, new float[width * height], true)
  {

  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_float::gray_image_buffer_float(int width, int height, float * buffer)
  :
  gray_image_buffer_abc<float>(width, height, buffer, false)
  {

  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_float::gray_image_buffer_float(int width, int height, float fill_value)
  :
  gray_image_buffer_abc<float>(width, height, new float[width * height], true)
  {
    std::fill(_buffer, _buffer + width * height, fill_value);
  }

  /////////////////////////////////////////////////////////////////////
  void gray_image_buffer_float::write(int x, int y, const float & value)
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);

    _buffer[y * _width + x] = value;
  }
      
  /////////////////////////////////////////////////////////////////////
  float gray_image_buffer_float::read(int x, int y) const
  {
    GGO_ASSERT(_buffer);
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);

    return _buffer[y * _width + x];
  }
}

