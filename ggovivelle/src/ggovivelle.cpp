#include <ggo_file_helpers.h>
#include <ggo_tga.h>
#include <ggo_bmp.h>
#include <ggo_jpg.h>
#include <ggo_image_io.h>
#include <ggo_color_stream.h>
#include <iostream>

bool save_image(const std::string & output, const ggo::pixel_buffer & image, std::string & error_msg)
{
  std::string extension = ggo::get_file_extension(output);
  if (extension == "bmp")
  {
    return ggo::save_bmp(output, image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
  }
  else if (extension == "tga")
  {
    return ggo::save_tga(output, image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
  }
  else if (extension == "jpg" || extension == "jpeg")
  {
    return ggo::save_jpg(output, 95, image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
  }
  else
  {
    error_msg = std::string("missing or invalid file extension for filename name '") + output + "'";
    return false;
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

  auto image = ggo::load_image(args[0]);

  std::string error_msg;
  if (save_image(args.back(), image, error_msg) == false)
  {
    if (error_msg.empty() == true)
    {
      error_msg = std::string("failed saving file '") + args.back() + "'";
    }
    std::cerr << ggo::red_color << error_msg << ggo::default_color << std::endl;
    return 1;
  }

  return 0;
}
