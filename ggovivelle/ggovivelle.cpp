#include <kernel/ggo_string_helpers.h>
#include <kernel/ggo_color_stream.h>
#include <kernel/time/ggo_chronometer.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_io.h>
#include <vivelle/image_processings/ggo_image_processing_abc.h>

namespace ggo
{
  ggo::image process_image(ggo::image image,
    std::vector<std::string>::const_iterator begin,
    std::vector<std::string>::const_iterator end)
  {
    for (auto it = begin; it != end; ++it)
    {
      command cmd(*it);

      scoped_chronometer chrono(cmd.name(), true);

      auto processing = image_processing_abc::create(cmd.name());
      if (!processing)
      {
        throw std::runtime_error("unexpected command '" + cmd.name() + "'");
      }

      image = processing->process(std::move(image), cmd.parameters());
    }

    return image;
  }
}

int main(int argc, char ** argv)
{
  std::vector<std::string> args;
  for (int arg = 1; arg < argc; ++arg)
  {
    args.emplace_back(argv[arg]);
  }

  if (args.size() < 2)
  {
    std::cerr << ggo::red_color << "expecting at least 2 arguments: input and output" << ggo::default_color << std::endl;
    return 1;
  }

  bool image_loaded = false;
  bool image_processed = false;
  bool image_saved = false;
  try
  {
    auto image = ggo::open_image(args.front());
    image_loaded = true;

    image = process_image(std::move(image), args.begin() + 1, args.end() - 1);
    image_processed = true;

    ggo::save_image(args.back(), image);
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
