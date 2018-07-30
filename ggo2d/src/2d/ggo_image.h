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
  template <image_format format>
  class static_image final
  {
  public:

    static_image(ggo::size s) : static_image(s.width(), s.height()) {}
    static_image(int width, int height);
    ~static_image() { free(_buffer); }

    // No move.
    static_image(static_image && i) = delete;
    void operator=(static_image && i) = delete;

    // No copy.
    static_image(const static_image &) = delete;
    void operator=(const static_image &) = delete;

    int                 width() const { return _width; }
    int                 height() const { return _height; }
    int                 min_size() const { return std::min(_width, _height); }
    ggo::size           size() const { return { _width, _height }; }
    int                 line_byte_step() const { return _line_byte_step; }
    void *              data() { return _buffer; }
    const void *        data() const { return _buffer; }

    void *              line_ptr(int y);
    const void *        line_ptr(int y) const;
    void *              pixel_ptr(int x, int y);
    const void *        pixel_ptr(int x, int y) const;

    void                fill(const typename image_format_traits<format>::color_t & c);

  private:

    void *  _buffer;
    int     _width;
    int     _height;
    int     _line_byte_step;
  };

  //////////////////////////////////////////////////////////////
  // Implementation.
  template <image_format format>
  static_image<format>::static_image(int width, int height)
  {
    using format_traits = image_format_traits<format>;

    _line_byte_step = width * format_traits::pixel_byte_size;
    _buffer = malloc(height * _line_byte_step);
    _width = width;
    _height = height;
  }

  template <image_format format>
  void * static_image<format>::line_ptr(int y)
  {
    using format_traits = image_format_traits<format>;

    return ggo::get_line_ptr<format_traits::lines_order>(_buffer, y, _height, _line_byte_step);
  }
  
  template <image_format format>
  const void * static_image<format>::line_ptr(int y) const
  {
    using format_traits = image_format_traits<format>;

    return ggo::get_line_ptr<format_traits::lines_order>(_buffer, y, _height, _line_byte_step);
  }
  
  template <image_format format>
  void * static_image<format>::pixel_ptr(int x, int y)
  {
    return ggo::get_pixel_ptr<format>(_buffer, x, y, _height, _line_byte_step);
  }

  template <image_format format>
  const void * static_image<format>::pixel_ptr(int x, int y) const
  {
    return ggo::get_pixel_ptr<format>(_buffer, x, y, _height, _line_byte_step);
  }

  template <image_format format>
  void static_image<format>::fill(const typename image_format_traits<format>::color_t & c)
  {
    using format_traits = image_format_traits<format>;

    for (int y = 0; y < _height; ++y)
    {
      void * ptr = this->line_ptr(y);
      for (int x = 0; x < _width; ++x)
      {
        ggo::write_pixel<format>(ptr, c);
        ptr = ptr_offset<format_traits::pixel_byte_size>(ptr);
      }
    }
  }
}

#endif

