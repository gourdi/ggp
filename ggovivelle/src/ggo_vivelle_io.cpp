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
    auto command = ggo::parse_command(output_command, true);

    bool io_success = true;
    std::string extension = ggo::get_file_extension(command._name);
    if (extension == "bmp")
    {
      if (command._parameters.empty() == false)
      {
        throw std::runtime_error("invalid output command");
      }
      io_success = ggo::save_bmp(command._name, image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
    }
    else if (extension == "tga")
    {
      if (command._parameters.empty() == false)
      {
        throw std::runtime_error("invalid output command");
      }
      io_success = ggo::save_tga(command._name, image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
    }
    else if (extension == "jpg" || extension == "jpeg")
    {
      int quality = 95;
      switch (command._parameters.size())
      {
      case 0:
        break;
      case 1:
        if (command.has_key("q") == true)
        {
          quality = ggo::to<int>(command._parameters["q"]);
        }
        else if (command.has_key("quality") == true)
        {
          quality = ggo::to<int>(command._parameters["quality"]);
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
      io_success = ggo::save_jpg(command._name, quality, image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
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
