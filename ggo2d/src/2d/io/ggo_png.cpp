#include "ggo_png.h"
#include <kernel/memory/ggo_buffer.h>
#include <png.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  image load_png(const std::string & filename)
  {
    png_image png_image;
    memset(&png_image, 0, (sizeof png_image));
    png_image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&png_image, filename.c_str()) == 0)
    {
      throw std::runtime_error("failed reading image properties");
    }

    ggo::image_format format;
    switch (png_image.format)
    {
    case PNG_FORMAT_RGB:
      format = ggo::rgb_8u_yd;
      break;
    case PNG_FORMAT_RGBA:
      format = ggo::rgba_8u_yd;
      break;
    default:
      throw std::runtime_error("unsupported image format");
    }

    int line_byte_size = PNG_IMAGE_ROW_STRIDE(png_image);
    ggo::image image(png_image.width, png_image.height, line_byte_size, format);
    
    if (png_image_finish_read(&png_image, NULL, image.data(), 0, NULL) == 0)
    {
      throw std::runtime_error("failed reading image pixels");
    }

    return image;
  }
}

