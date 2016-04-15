#include "ggo_gray_image_data.h"

/////////////////////////////////////////////////////////////////////
// gray_image_buffer_uint8
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_uint8::gray_image_buffer_uint8(uint8_t * buffer, int width, int height)
  :
  gray_image_buffer_abc(width, height),
  _delete_buffer(false),
  _buffer(buffer)
  {
  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_uint8::gray_image_buffer_uint8(int width, int height)
  :
  gray_image_buffer_abc(width, height),
  _delete_buffer(true),
  _buffer(new uint8_t[width * height])
  {
  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_uint8::~gray_image_buffer_uint8()
  {
    if (_delete_buffer)
    {
      delete[] _buffer;
    }
  }

  /////////////////////////////////////////////////////////////////////  
  void gray_image_buffer_uint8::do_pack(int x, int y, const float & value)
  {
    _buffer[y * get_width() + x] = ggo::to<uint8_t>(value * 255.f);
  }
      
  /////////////////////////////////////////////////////////////////////
  float gray_image_buffer_uint8::do_unpack(int x, int y) const
  {
    return _buffer[y * get_width() + x] / 255.f;
  }
}

/////////////////////////////////////////////////////////////////////
// gray_image_buffer_float
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_float::gray_image_buffer_float(float * buffer, int width, int height)
  :
  gray_image_buffer_abc(width, height),
  _delete_buffer(false),
  _buffer(buffer)
  {
  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_float::gray_image_buffer_float(int width, int height)
  :
  gray_image_buffer_abc(width, height),
  _delete_buffer(true),
  _buffer(new float[width * height])
  {
  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_float::gray_image_buffer_float(int width, int height, float fill_value)
  :
  gray_image_buffer_abc(width, height),
  _delete_buffer(true),
  _buffer(new float[width * height]) 
  {
    std::fill(_buffer, _buffer + width * height, fill_value);
  }

  /////////////////////////////////////////////////////////////////////
  gray_image_buffer_float::~gray_image_buffer_float()
  {
    if (_delete_buffer)
    {
      delete[] _buffer;
    }
  }

  /////////////////////////////////////////////////////////////////////  
  void gray_image_buffer_float::do_pack(int x, int y, const float & value)
  {
    _buffer[y * get_width() + x] = value;
  }
      
  /////////////////////////////////////////////////////////////////////
  float gray_image_buffer_float::do_unpack(int x, int y) const
  {
    return _buffer[y * get_width() + x];
  }
}

