#include "ggo_png.h"
#include <kernel/memory/ggo_array.h>
#include <2d/processing/ggo_blit.h>
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

    ggo::pixel_type pixel_type;
    switch (png_image.format)
    {
    case PNG_FORMAT_RGB:
      pixel_type = ggo::pixel_type::rgb_8u;
      break;
    case PNG_FORMAT_RGBA:
      pixel_type = ggo::pixel_type::rgba_8u;
      break;
    case PNG_FORMAT_GA:
      pixel_type = ggo::pixel_type::ya_8u;
      break;
    default:
      throw std::runtime_error("unsupported image format");
    }

    int line_byte_size = PNG_IMAGE_ROW_STRIDE(png_image);
    ggo::image image({ int(png_image.width), int(png_image.height) }, pixel_type, lines_order::down, line_byte_size);
    
    if (png_image_finish_read(&png_image, NULL, image.data(), 0, NULL) == 0)
    {
      throw std::runtime_error("failed reading image pixels");
    }

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_png(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, ggo::lines_order lines_order, int width, int height, int line_byte_step)
  {
    ggo::image i1({ 10, 10 }, ggo::pixel_type::rgb_8u, ggo::lines_order::up);
    ggo::image i2({ 10, 10 }, ggo::pixel_type::rgb_8u, ggo::lines_order::up);
    ggo::blit(i1, i2, 0, 0);


    png_image png_image;

    memset(&png_image, 0, sizeof(png_image));

    png_image.version = PNG_IMAGE_VERSION;
    png_image.width = width;
    png_image.height = height;

    std::unique_ptr<image> image;

    switch (lines_order)
    {
    case lines_order::up:
      switch (pixel_type)
      {
      case ggo::pixel_type::rgb_8u:
        png_image.format = PNG_FORMAT_RGB;
        break;
      case ggo::pixel_type::bgr_8u:
        png_image.format = PNG_FORMAT_BGR;
        break;
      case ggo::pixel_type::rgba_8u:
        png_image.format = PNG_FORMAT_RGBA;
        break;
      default:
        return false;
      }
      break;
    //case lines_order::down:
    //  switch (pixel_type)
    //  {
    //  //case ggo::pixel_type::bgr_8u:
    //  //  png_image.format = PNG_FORMAT_RGB;
    //  //  image = std::make_unique<ggo::image>(ggo::size(width, height), ggo::pixel_type::rgb_8u, ggo::lines_order::down);
    //  //  ggo::blit<ggo::pixel_type::bgr_8u, ggo::lines_order::up, ggo::pixel_type::rgb_8u, ggo::lines_order::down>(
    //  //    buffer, width, height, line_byte_step, image->data(), width, height, image->line_byte_step());
    //  //  break;
    //  //case ggo::pixel_type::rgb_8u:
    //  //  png_image.format = PNG_FORMAT_RGB;
    //  //  image = std::make_unique<ggo::image>(ggo::size(width, height), ggo::pixel_type::rgb_8u, ggo::lines_order::down);
    //  //  ggo::blit<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::pixel_type::rgb_8u, ggo::lines_order::down>(
    //  //    buffer, width, height, line_byte_step, image->data(), width, height, image->line_byte_step());
    //  //  break;
    //  default:
    //    return false;
    //  }
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

