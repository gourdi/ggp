#include <kernel/ggo_color_stream.h>
#include <vivelle/ggo_command.h>
#include <vivelle/commands/ggo_io.h>
#include <vivelle/commands/ggo_blit.h>
#include <vivelle/commands/ggo_rescale.h>
#include <vivelle/commands/ggo_crop.h>
#include <iostream>
#include <vector>
#include <string>

namespace ggo
{
  void process_image(ggo::image & image, 
    std::vector<std::string>::const_iterator begin,
    std::vector<std::string>::const_iterator end)
  {
    for (auto it = begin; it != end; ++it)
    {
      command cmd(*it);
      if (cmd.name() == "blit")
      {
        blit(image, cmd.parameters());
      }
      else if (cmd.name() == "rescale")
      {
        image = rescale(image, cmd.parameters());
      }
      else if (cmd.name() == "crop")
      {
        image = crop(image, cmd.parameters());
      }
      else
      {
        throw std::runtime_error("unexpected command '" + cmd.name() + "'");
      }
    }
  }
}

int main(int argc, char ** argv)
{
  std::vector<std::string> args;
  for (int arg = 1; arg < argc; ++arg)
  {
    args.emplace_back(argv[arg]);
  }

  if (args.size() <= 2)
  {
    std::cerr << ggo::red_color << "expecting at least 2 arguments: input and output" << ggo::default_color << std::endl;
    return 1;
  }

  bool image_loaded = false;
  bool image_processed = false;
  bool image_saved = false;
  try
  {
    auto image = ggo::load(args.front());
    image_loaded = true;

    process_image(image, args.begin() + 1, args.end() - 1);
    image_processed = true;

    ggo::save(args.back(), image);
    image_saved = true;
  }
  catch (std::exception & e)
  {
    if (image_loaded == false)
    {
      std::cerr << ggo::red_color << "failed loading image '" << args.front() << "': " << e.what() << ggo::default_color << std::endl;
    }
    else if (image_processed == false)
    {
      std::cerr << ggo::red_color << "failed processing image '" << args.back() << "': " << e.what() << ggo::default_color << std::endl;
    }
    else if (image_saved == false)
    {
      std::cerr << ggo::red_color << "failed saving image '" << args.back() << "': " << e.what() << ggo::default_color << std::endl;
    }

    return 1;
  }

  return 0;
}
