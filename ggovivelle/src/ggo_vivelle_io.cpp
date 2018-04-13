#include "ggo_vivelle_io.h"
#include "ggo_command.h"
#include <ggo_file_helpers.h>
#include <ggo_string_helpers.h>
#include <ggo_tga.h>
#include <ggo_bmp.h>
#include <ggo_jpg.h>
#include <ggo_image_io.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::pixel_buffer load(const std::string & filename)
  {
    return load_image(filename);
  }

  //////////////////////////////////////////////////////////////
  void save(const std::string & output_command, const ggo::pixel_buffer & image)
  {
    command cmd(output_command, true);

    bool io_success = true;
    std::string extension = ggo::get_file_extension(cmd.name());
    if (extension == "bmp")
    {
      if (cmd.parameters().empty() == false)
      {
        throw std::runtime_error("invalid output command");
      }
      io_success = ggo::save_bmp(cmd.name(), image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
    }
    else if (extension == "tga")
    {
      if (cmd.parameters().empty() == false)
      {
        throw std::runtime_error("invalid output command");
      }
      io_success = ggo::save_tga(cmd.name(), image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
    }
    else if (extension == "jpg" || extension == "jpeg")
    {
      int quality = 95;
      switch (cmd.parameters().size())
      {
      case 0:
        break;
      case 1:
        if (auto quality_param = cmd["q"])
        {
          quality = ggo::to<int>(*quality_param);
        }
        if (auto quality_param = cmd["quality"])
        {
          quality = ggo::to<int>(*quality_param);
        }
        else
        {
          throw std::runtime_error("invalid quality parameter");
        }

        if (quality <= 0 || quality > 100)
        {
          throw std::runtime_error("out of range quality value");
        }
        break;
      default:
        throw std::runtime_error("invalid output command");
        break;
      }
      io_success = ggo::save_jpg(cmd.name(), quality, image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
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
