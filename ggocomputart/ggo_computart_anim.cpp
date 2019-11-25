#include "ggo_animation_artist.h"
#include <kernel/memory/ggo_array.h>
#include <kernel/time/ggo_chronometer.h>
#include <2d/io/ggo_bmp.h>
#include <sstream>
#include <iostream>

struct ggo_params
{
  ggo::animation_artist_id  _artist_id;
  std::string				        _output_directory;
  int						            _width;
  int						            _height;
  ggo::ratio                _fps{ 25, 1 }; // For now this is fixed.
};

//////////////////////////////////////////////////////////////
bool parse_args(int argc, char ** argv, ggo_params & params)
{
  if (argc == 1)
  {
    std::cout << "ggocomputartanim ARTIST_ID [-d OUTPUT_DIRECTORY] [-s OUTPUT_WIDTH OUTPUT_HEIGHT]" << std::endl;
    return false;
  }

  params._width = 1280;
  params._height = 720;

  for (int i = 1; i < argc; ++i)
  {
    std::string arg(argv[i]);

    if (i == 1)
    {
      params._artist_id = static_cast<ggo::animation_artist_id>(ggo::to<int>(arg));
    }
    else if (arg.compare("-d") == 0)
    {
      ++i;
      if (i >= argc)
      {
        std::cout << "Error: missing directory parameter" << std::endl;
        return false;
      }

      params._output_directory = argv[i];
    }
    else if (arg.compare("-s") == 0)
    {
      ++i;
      if (i >= argc)
      {
        std::cout << "Error : missing width parameter" << std::endl;
        return false;
      }
      params._width = ggo::to<int>(argv[i]);

      ++i;
      if (i >= argc)
      {
        std::cout << "Error : missing height parameter" << std::endl;
        return false;
      }
      params._height = ggo::to<int>(argv[i]);
    }
    else
    {
      std::cout << "Error: invalid command line parameter" << std::endl;
      return false;
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  ggo_params params;
  if (parse_args(argc, argv, params) == false)
  {
    return 1;
  }

  int line_byte_step = 3 * params._width;
  ggo::array_8u buffer(line_byte_step * params._height);

  std::cout << "Artist ID: " << static_cast<int>(params._artist_id) << std::endl;
  std::cout << "Output resolution: " << params._width << 'x' << params._height << std::endl;
  std::cout << "Output directory: " << params._output_directory << std::endl;

  std::unique_ptr<ggo::animation_artist> artist(ggo::animation_artist::create(
    params._artist_id, params._width, params._height, line_byte_step, ggo::pixel_type::rgb_8u, ggo::lines_order::up, params._fps));

  if (artist.get() == nullptr)
  {
    std::cout << "Wrong artist ID" << std::endl;
    return 1;
  }

  ggo::chronometer animation_chronometer;

  int i = 0;
  while (true)
  {
    ggo::chronometer frame_chronometer;

    bool finished = true;
    artist->render_frame(buffer.data(), finished);
    if (finished == true)
    {
      break;
    }

    std::ostringstream filename;
    if (params._output_directory.length() > 0)
    {
      filename << params._output_directory << '/';
    }
    filename.width(8);
    filename.fill('0');
    filename << i << ".bmp";
    std::cout << "Saved image " << filename.str() << " (image computed in " << frame_chronometer.get_display_time() << ")" << std::endl;

    if (ggo::save_bmp(filename.str(), buffer.data(), artist->pixel_type(), artist->memory_lines_order(), params._width, params._height, line_byte_step) == false)
    {
      std::cerr << "Failed saving image " + filename.str() << std::endl;
    }

    ++i;
  }

  std::cout << "Animation computed in " << animation_chronometer.get_display_time() << std::endl;

  return 0;
}
