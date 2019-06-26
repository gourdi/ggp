namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <image_format img_format>
  image_t<img_format>::image_t(image_t && img)
    :
    image_wrapper_t<img_format>(img._buffer, img._size, img._line_byte_step)
  {
    img._buffer = nullptr;
  }

  //////////////////////////////////////////////////////////////
  template <image_format img_format>
  void image_t<img_format>::operator=(image_t && img)
  {
    free(_buffer);

    _size = img._size;
    _line_byte_step = img._line_byte_step;
    _buffer = img._buffer;

    img._buffer = nullptr;
  }
}

