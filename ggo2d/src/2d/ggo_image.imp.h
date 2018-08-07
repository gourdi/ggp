namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  static_image<image_format>::static_image(int width, int height)
  {
    using format_traits = image_format_traits<image_format>;

    _line_byte_step = width * format_traits::pixel_byte_size;
    _buffer = malloc(height * _line_byte_step);
    _width = width;
    _height = height;
  }

  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  void * static_image<image_format>::line_ptr(int y)
  {
    using format_traits = image_format_traits<image_format>;

    return ggo::get_line_ptr<format_traits::lines_order>(_buffer, y, _height, _line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  const void * static_image<image_format>::line_ptr(int y) const
  {
    using format_traits = image_format_traits<image_format>;

    return ggo::get_line_ptr<format_traits::lines_order>(_buffer, y, _height, _line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  void * static_image<image_format>::pixel_ptr(int x, int y)
  {
    return ggo::get_pixel_ptr<image_format>(_buffer, x, y, _height, _line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  const void * static_image<image_format>::pixel_ptr(int x, int y) const
  {
    return ggo::get_pixel_ptr<image_format>(_buffer, x, y, _height, _line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  void static_image<image_format>::fill(const typename image_format_traits<image_format>::color_t & c)
  {
    using format_traits = image_format_traits<image_format>;

    for (int y = 0; y < _height; ++y)
    {
      void * ptr = this->line_ptr(y);
      for (int x = 0; x < _width; ++x)
      {
        ggo::write_pixel<image_format>(ptr, c);
        ptr = ptr_offset<format_traits::pixel_byte_size>(ptr);
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  typename image_format_traits<image_format>::color_t static_image<image_format>::read_pixel(int x, int y) const
  {
    return ggo::read_pixel<image_format>(_buffer, x, y, _height, _line_byte_step);
  }

  //////////////////////////////////////////////////////////////
  template <image_format image_format>
  void static_image<image_format>::write_pixel(int x, int y, const typename image_format_traits<image_format>::color_t & c)
  {
    ggo::write_pixel<image_format>(_buffer, x, y, _height, _line_byte_step, c);
  }
}
