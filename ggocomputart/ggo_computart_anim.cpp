#include "ggo_animation_artist_abc.h"
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
      auto artist_id = ggo::str_to<int>(arg);

      if (artist_id.has_value() == false || *artist_id < 0)
      {
        std::cerr << "Error : invalid artist id argument" << std::endl;
        return false;
      }
      params._artist_id = static_cast<ggo::animation_artist_id>(*artist_id);
    }
    else
      if (arg.compare("-d") == 0)
      {
        ++i;
        if (i >= argc)
        {
          std::cout << "Error: missing directory parameter" << std::endl;
          return false;
        }

        params._output_directory = argv[i];
      }
      else
        if (arg.compare("-s") == 0)
        {
          ++i;
          if (i >= argc)
          {
            std::cout << "Error : missing width parameter" << std::endl;
            return false;
          }
          auto width = ggo::str_to<int>(argv[i]);
          if (width.has_value() == false || *width <= 0)
          {
            std::cout << "Error : invalid width argument" << std::endl;
            return false;
          }
          params._width = *width;

          ++i;
          if (i >= argc)
          {
            std::cout << "Error : missing height parameter" << std::endl;
            return false;
          }
          auto height = ggo::str_to<int>(argv[i]);
          if (height.has_value() == false || *height <= 0)
          {
            std::cout << "Error : invalid height argument" << std::endl;
            return false;
          }
          params._height = *height;
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

  std::unique_ptr<ggo::animation_artist_abc> artist(ggo::animation_artist_abc::create(
    params._artist_id, params._width, params._height, line_byte_step, ggo::rgb_8u_yu, ggo::animation_artist_abc::offscreen_rendering));

  if (artist.get() == nullptr)
  {
    std::cout << "Wrong artist ID" << std::endl;
    return 1;
  }

  artist->init_animation();

  ggo::chronometer animation_chronometer;

  int i = 0;
  while (true)
  {
    ggo::chronometer frame_chronometer;

    if (artist->prepare_frame(buffer.data()) == false)
    {
      break;
    }

    artist->render_frame(buffer.data(), ggo::rect_int::from_left_right_bottom_top(0, params._width - 1, 0, params._height - 1));

    std::ostringstream filename;
    if (params._output_directory.length() > 0)
    {
      filename << params._output_directory << '/';
    }
    filename.width(8);
    filename.fill('0');
    filename << i << ".bmp";
    std::cout << "Saved image " << filename.str() << " (image computed in " << frame_chronometer.get_display_time() << ")" << std::endl;

    if (ggo::save_bmp(filename.str(), buffer.data(), ggo::rgb_8u_yu, params._width, params._height, line_byte_step) == false)
    {
      std::cerr << "Failed saving image " + filename.str() << std::endl;
    }

    ++i;
  }

  std::cout << "Animation computed in " << animation_chronometer.get_display_time() << std::endl;

  return 0;
}
