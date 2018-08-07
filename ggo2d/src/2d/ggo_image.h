#ifndef __GGO_IMAGE__
#define __GGO_IMAGE__

#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_size.h>
#include <2d/ggo_image_format.h>

namespace ggo
{
  class image final
  {
  public:

    image(ggo::size s, image_format format); // Line step is the natural one: no padding at the end of lines.
    image(int width, int height, image_format format); // Line step is the natural one: no padding at the end of lines.
    image(int width, int height, int line_byte_step, image_format format);
    ~image();

    // Move.
    image(image && i);
    void operator=(image && i);

    // No copy.
    image(const image &) = delete;
    void operator=(const image &) = delete;

    int           width() const { return _width; }
    int           height() const { return _height; }
    int           min_size() const { return std::min(_width, _height); }
    ggo::size     size() const { return { _width, _height }; }
    int           line_byte_step() const { return _line_byte_step; }
    image_format  format() const { return _format; }
    void *        data() { return _buffer; }
    const void *  data() const { return _buffer; }

    void *        line_ptr(int y);
    const void *  line_ptr(int y) const;
    void *        pixel_ptr(int x, int y);
    const void *  pixel_ptr(int x, int y) const;

    template <typename func_t>
    void          for_each_pixel(func_t f) const
    { 
      for (int y = 0; y < _height; ++y)
      {
        for (int x = 0; x < _width; ++x)
        {
          f(x, y);
        }
      }
    }

  private:

    void *        _buffer;
    int           _width;
    int           _height;
    int           _line_byte_step;
    image_format  _format;
  };
}

namespace ggo
{
  template <image_format image_format>
  class static_image final
  {
  public:

    static_image(ggo::size s) : static_image(s.width(), s.height()) {}
    static_image(int width, int height);
    ~static_image() { free(_buffer); }

    // No move.
    static_image(static_image<image_format> && i) = delete;
    void operator=(static_image<image_format> && i) = delete;

    // No copy.
    static_image(const static_image<image_format> &) = delete;
    void operator=(const static_image<image_format> &) = delete;

    int                 width() const { return _width; }
    int                 height() const { return _height; }
    int                 min_size() const { return std::min(_width, _height); }
    ggo::size           size() const { return { _width, _height }; }
    int                 line_byte_step() const { return _line_byte_step; }
    ggo::image_format   format() const { return image_format; }
    void *              data() { return _buffer; }
    const void *        data() const { return _buffer; }

    void *              line_ptr(int y);
    const void *        line_ptr(int y) const;
    void *              pixel_ptr(int x, int y);
    const void *        pixel_ptr(int x, int y) const;

    void                fill(const typename image_format_traits<image_format>::color_t & c);
    typename image_format_traits<image_format>::color_t    read_pixel(int x, int y) const;
    void                write_pixel(int x, int y, const typename image_format_traits<image_format>::color_t & c);

    template <typename func_t>
    void                for_each_pixel(func_t f) const
    {
      for (int y = 0; y < _height; ++y)
      {
        for (int x = 0; x < _width; ++x)
        {
          f(x, y);
        }
      }
    }

  private:

    void *  _buffer;
    int     _width;
    int     _height;
    int     _line_byte_step;
  };
}

#include <2d/ggo_image.imp.h>

#endif

