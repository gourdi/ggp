#ifndef __GGO_IMAGE_BUFFER_ABC__
#define __GGO_IMAGE_BUFFER_ABC__

#include <ggo_image_abc.h>
#include <ggo_kernel.h>
#include <ggo_2d.h>

/////////////////////////////////////////////////////////////////////
// Color are stored in buffers.
namespace ggo
{
  template <typename color_t, pixel_buffer_format pbf>
  class image_buffer_abc : public image_abc<color_t>
  {
  public:

                            image_buffer_abc(image_buffer_abc<color_t, pbf> && rhs);
                            image_buffer_abc(int width, int height);
                            image_buffer_abc(int width, int height, int line_step);
                            image_buffer_abc(void * buffer, int width, int height, int line_step, bool delete_buffer = false);
    virtual                ~image_buffer_abc();

            void            set(int x, int y, const color_t & value) override;
            color_t         get(int x, int y) const override;

            const void *    get_line_ptr(int y) const;
            void *          get_line_ptr(int y);

            const void *    data() const { return _buffer; }
            void *          data() { return _buffer; }

            void            operator=(image_buffer_abc<color_t, pbf> && rhs);

  // No copy.
                            image_buffer_abc(const image_buffer_abc<color_t, pbf> & rhs) = delete;
            void            operator=(const image_buffer_abc<color_t, pbf> & rhs) = delete;

  protected:
    
    void *  _buffer = nullptr;
    bool    _delete_buffer = false;
    int     _line_step;
  };
}

/////////////////////////////////////////////////////////////////////
// Implementation
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  image_buffer_abc<color_t, pbf>::image_buffer_abc(image_buffer_abc<color_t, pbf> && rhs)
    :
    image_abc<color_t>(width, height),
    _buffer(rhs._buffer),
    _delete_buffer(rhs._delete_buffer),
    _line_step(rhs._line_step)
  {
    rhs._buffer = nullptr;
    rhs._delete_buffer = false;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  image_buffer_abc<color_t, pbf>::image_buffer_abc(int width, int height)
    :
    image_abc<color_t>(width, height),
    _buffer(new uint8_t[height * width * pixel_byte_size<pbf>()]),
    _delete_buffer(false),
    _line_step(width * pixel_byte_size<pbf>())
  {
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  image_buffer_abc<color_t, pbf>::image_buffer_abc(int width, int height, int line_step)
    :
    image_abc<color_t>(width, height),
    _buffer(new uint8_t[height * line_step]),
    _delete_buffer(false),
    _line_step(line_step)
  {
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  image_buffer_abc<color_t, pbf>::image_buffer_abc(void * buffer, int width, int height, int line_step, bool delete_buffer)
    :
    image_abc<color_t>(width, height),
    _buffer(buffer),
    _delete_buffer(delete_buffer)
    _line_step(line_step)
  {
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  image_buffer_abc<color_t, pbf>::~image_buffer_abc()
  {
    if (_delete_buffer)
    {
      delete[] _buffer;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  void image_buffer_abc<color_t, pbf>::operator=(image_buffer_abc<color_t, pbf> && rhs)
  {
    if (_delete_buffer == true)
    {
      delete[] _buffer;
    }

    _width = rhs._width;
    _height = rhs._height;
    _buffer = rhs._buffer;
    _delete_buffer = rhs._delete_buffer;

    rhs._buffer = nullptr;
    rhs._delete_buffer = false;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  void image_buffer_abc<color_t, pbf>::set(int x, int y, const color_t & value)
  {
    void * ptr = static_cast<uint8_t *>(get_line_ptr(y)) + x * pixel_byte_size<pbf>();
    set_pixel<pbf>(ptr, value);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  color_t image_buffer_abc<color_t, pbf>::get(int x, int y) const
  {
    const void * ptr = static_cast<uint8_t *>(get_line_ptr(y)) + x * pixel_byte_size<pbf>();
    return get_pixel<pbf>(ptr);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  const void * image_buffer_abc<color_t, pbf>::get_line_ptr(int y) const
  {
    return static_cast<const uint8_t *>(_buffer) + (is_y_down<pbf>() ? (height - y - 1) : y) * _line_step;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, pixel_buffer_format pbf>
  void * image_buffer_abc<color_t, pbf>::get_line_ptr(int y)
  {
    return static_cast<uint8_t *>(_buffer) + (is_y_down<pbf>() ? (height - y - 1) : y) * _line_step;
  }
}

/////////////////////////////////////////////////////////////////////
// Aliases
namespace ggo
{
  using image_buffer_gray_8u_yu = image_buffer_abc<uint8_t, ggo::pixel_buffer_format::gray_8u_yu>;
  using image_buffer_rgb_8u_yu  = image_buffer_abc<ggo::color_8u, ggo::pixel_buffer_format::rgb_8u_yu>;
  using image_buffer_bgra_8u_yd = image_buffer_abc<ggo::color_8u, ggo::pixel_buffer_format::bgra_8u_yd>;
}

#endif
