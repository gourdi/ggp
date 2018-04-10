#include "ggo_command.h"
#include <ggo_file_helpers.h>
#include <ggo_string_helpers.h>
#include <ggo_tga.h>
#include <ggo_bmp.h>
#include <ggo_jpg.h>
#include <ggo_image_io.h>
#include <ggo_color_stream.h>
#include <iostream>

void save_image(const std::string & output, const ggo::pixel_buffer & image)
{
  auto command = ggo::parse_command(output, true);

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
      if (command._parameters[0]._key != "q" && command._parameters[0]._key != "quality")
      {
        throw std::runtime_error("invalid quality parameter");
      }
      quality = ggo::to<int>(command._parameters[0]._value);
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

int main(int argc, char ** argv)
{
  std::vector<std::string> args;
  for (int arg = 1; arg < argc; ++arg)
  {
    args.emplace_back(argv[arg]);
  }

  if (args.size() != 2)
  {
    std::cerr << ggo::red_color << "expecting 2 arguments: input and output" << ggo::default_color << std::endl;
    return 1;
  }

  bool image_loaded = false;
  bool image_saved = false;
  try
  {
    auto image = ggo::load_image(args[0]);
    image_loaded = true;

    save_image(args.back(), image);
    image_saved = true;
  }
  catch (std::exception e)
  {
    if (image_loaded == false)
    {
      std::cerr << ggo::red_color << "failed loading image '" << args.front() << "': " << e.what() << ggo::default_color << std::endl;
    }
    else if (image_saved == false)
    {
      std::cerr << ggo::red_color << "failed saving image '" << args.back() << "': " << e.what() << ggo::default_color << std::endl;
    }

    return 1;
  }

  return 0;
}
