#include "ggo_png.h"
#include <ggo_buffer.h>
#include <png.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  pixel_buffer load_png(const std::string & filename)
  {
    png_image image;
    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&image, filename.c_str()) == 0)
    {
      throw std::runtime_error("failed reading image properties");
    }

    ggo::pixel_buffer_format pbf;
    switch (image.format)
    {
    case PNG_FORMAT_RGB:
      pbf = ggo::rgb_8u_yd;
      break;
    case PNG_FORMAT_RGBA:
      pbf = ggo::rgba_8u_yd;
      break;
    default:
      throw std::runtime_error("unsupported image format");
    }

    int line_byte_size = PNG_IMAGE_ROW_STRIDE(image);
    ggo::pixel_buffer pixels(image.width, image.height, line_byte_size, pbf);
    
    if (png_image_finish_read(&image, NULL, pixels.data(), 0, NULL) == 0)
    {
      throw std::runtime_error("failed reading image pixels");
    }

    return pixels;
  }
}

