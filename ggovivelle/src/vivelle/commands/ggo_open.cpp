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

      auto format = params.get({ "format" });
      if (!format || *format == "rgb8uyd")
      {
        return ggo::image(*width, *height, ggo::rgb_8u_yd);
      }
      else if (*format == "rgb8uyu")
      {
        return ggo::image(*width, *height, ggo::rgb_8u_yu);
      }
      else
      {
        throw std::runtime_error("invalid iamge format");
      }
    }
  }
}
