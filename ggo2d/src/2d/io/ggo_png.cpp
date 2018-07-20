#include "ggo_png.h"
#include <kernel/memory/ggo_array.h>
#include <2d/ggo_blit.h>
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

  //////////////////////////////////////////////////////////////
  bool save_png(const std::string & filename, const void * buffer, image_format format, int width, int height, int line_byte_step)
  {
    png_image png_image;

    memset(&png_image, 0, sizeof(png_image));

    png_image.version = PNG_IMAGE_VERSION;
    png_image.width = width;
    png_image.height = height;

    std::unique_ptr<image> image;

    switch (format)
    {
    case ggo::rgb_8u_yd:
      png_image.format = PNG_FORMAT_RGB;
      break;
    case ggo::bgr_8u_yd:
      png_image.format = PNG_FORMAT_BGR;
      break;
    case ggo::rgba_8u_yd:
      png_image.format = PNG_FORMAT_RGBA;
      break;
    case ggo::bgr_8u_yu:
      png_image.format = PNG_FORMAT_RGB;
      image = std::make_unique<ggo::image>(width, height, ggo::rgb_8u_yd);
      ggo::blit<ggo::bgr_8u_yu, ggo::rgb_8u_yd>(buffer, width, height, line_byte_step, image->data(), width, height, image->line_byte_step(), 0, 0);
      break;
    case ggo::rgb_8u_yu:
      png_image.format = PNG_FORMAT_RGB;
      image = std::make_unique<ggo::image>(width, height, ggo::rgb_8u_yd);
      ggo::blit<ggo::rgb_8u_yu, ggo::rgb_8u_yd>(buffer, width, height, line_byte_step, image->data(), width, height, image->line_byte_step(), 0, 0);
      break;

    default:
      return false;
    }

    if (image)
    {
      buffer = image->data();
      line_byte_step = image->line_byte_step();
    }

    if (png_image_write_to_file(&png_image, filename.c_str(), 0, buffer, line_byte_step, NULL) == 0)
    {
      return false;
    }

    return true;
  }
}

