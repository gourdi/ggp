#include "ggo_open.h"
#include "ggo_io.h"
#include <filesystem>
#include <vivelle/ggo_command.h>

namespace ggo
{
  ggo::image open_image(const std::string & filename)
  {
    // Open a file.
    if (std::filesystem::exists(filename) == true)
    {
      return load(filename);
    }
    else // Allocate an image.
    {
      const parameters params(filename);

      auto width = params.get<int>({ "w", "width" });
      auto height = params.get<int>({ "h", "height" });
      if (!width || !height)
      {
        throw std::runtime_error("missing width or height");
      }

      return ggo::image({ *width, *height }, ggo::pixel_type::rgb_8u, ggo::lines_order::down);
    }
  }
}
