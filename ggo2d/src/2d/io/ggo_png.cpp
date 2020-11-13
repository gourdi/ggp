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

    ggo::size s = { int(png_image.width), int(png_image.height) };
    int line_byte_step = PNG_IMAGE_ROW_STRIDE(png_image);
    std::unique_ptr<ggo::memory_layout> mem_layout;

    ggo::pixel_type pixel_type;
    switch (png_image.format)
    {
    case PNG_FORMAT_RGB:
      pixel_type = ggo::pixel_type::rgb_8u;
      mem_layout = std::make_unique<ggo::rows_memory_layout<3, vertical_direction::down>>(s, line_byte_step);
      break;
    case PNG_FORMAT_RGBA:
      pixel_type = ggo::pixel_type::rgba_8u;
      mem_layout = std::make_unique<ggo::rows_memory_layout<4, vertical_direction::down>>(s, line_byte_step);
      break;
    case PNG_FORMAT_GA:
      pixel_type = ggo::pixel_type::ya_8u;
      mem_layout = std::make_unique<ggo::rows_memory_layout<2, vertical_direction::down>>(s, line_byte_step);
      break;
    default:
      throw std::runtime_error("unsupported image format");
    }

    image image(pixel_type, std::move(mem_layout));

    if (png_image_finish_read(&png_image, NULL, image.data(), 0, NULL) == 0)
    {
      throw std::runtime_error("failed reading image pixels");
    }

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_png(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, const ggo::memory_layout & mem_layout)
  {
    png_image png_image;

    memset(&png_image, 0, sizeof(png_image));

    png_image.version = PNG_IMAGE_VERSION;
    png_image.width = mem_layout.size().width();
    png_image.height = mem_layout.size().height();

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

    ggo::image_t<pixel_type::rgb_8u, rows_memory_layout<3, vertical_direction::down>> image(mem_layout.size());

    image.for_each_pixel([&](void *, int x, int y)
    {
      ggo::rgb_8u rgb = ggo::read_rgb_8u(mem_layout.at(buffer, x, y), pixel_type);

      image.write_pixel(x, y, rgb);
    });

    if (png_image_write_to_file(&png_image, filename.c_str(), 0, image.data(), image.memory_layout()._line_byte_step, nullptr) == 0)
    {
      return false;
    }

    return true;
  }
}

