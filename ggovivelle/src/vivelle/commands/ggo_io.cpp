#include "ggo_io.h"
#include <kernel/ggo_string_helpers.h>
#include <2d/io/ggo_tga.h>
#include <2d/io/ggo_bmp.h>
#include <2d/io/ggo_png.h>
#include <2d/io/ggo_jpg.h>
#include <2d/io/ggo_image_io.h>
#include <vivelle/ggo_command.h>
#include <filesystem>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::image load(const std::string & filename)
  {
    return load_image(filename);
  }

  //////////////////////////////////////////////////////////////
  void save(const std::string & output_command, const ggo::image & image)
  {
    command cmd(output_command, { command::policy::filename });

    bool io_success = true;
    std::string extension = std::filesystem::path(cmd.name()).extension().u8string();
    if (extension == ".bmp")
    {
      if (cmd.parameters().empty() == false)
      {
        throw std::runtime_error("invalid output command");
      }
      io_success = ggo::save_bmp(cmd.name(), image.data(), image.pixel_type(), image.memory_lines_order(), image.width(), image.height(), image.line_byte_step());
    }
    else if (extension == ".tga")
    {
      if (cmd.parameters().empty() == false)
      {
        throw std::runtime_error("invalid output command");
      }
      io_success = ggo::save_tga(cmd.name(), image.data(), image.pixel_type(), image.memory_lines_order(), image.width(), image.height(), image.line_byte_step());
    }
    else if (extension == ".png")
    {
      if (cmd.parameters().empty() == false)
      {
        throw std::runtime_error("invalid output command");
      }
      io_success = ggo::save_png(cmd.name(), image.data(), image.pixel_type(), image.memory_lines_order(), image.width(), image.height(), image.line_byte_step());
    }
    else if (extension == ".jpg" || extension == ".jpeg")
    {
      int quality = 95;
      switch (cmd.parameters().size())
      {
      case 0:
        break;
      case 1:
      {
        auto quality_param = cmd.parameters().get<int>({ "q", "quality" });
        if (!quality_param)
        {
          throw std::runtime_error("invalid quality parameter");
        }

        quality = *quality_param;

        if (quality <= 0 || quality > 100)
        {
          throw std::runtime_error("out of range quality value");
        }
      }
        break;
      default:
        throw std::runtime_error("invalid output command");
        break;
      }
      io_success = ggo::save_jpg(cmd.name(), quality, image.data(), image.pixel_type(), image.memory_lines_order(), image.width(), image.height(), image.line_byte_step());
    }
    else
    {
      throw std::runtime_error("invalid or missing extension");
    }

    if (io_success == false)
    {
      throw std::runtime_error("i/o error");
    }
  }
}
