#include "ggo_image_io.h"
#include "ggo_bmp.h"
#include "ggo_tga.h"
#include "ggo_jpg.h"
#include "ggo_png.h"
#include <filesystem>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::image load_image(const std::string & filename)
  {
    std::string extension = std::filesystem::path(filename).extension().u8string();

    if (extension == "bmp")
    {
      return load_bmp(filename);
    }
    else if (extension == "tga")
    {
      return load_tga(filename);
    }
    else if (extension == "jpg" || extension == "jpeg")
    {
      return load_jpg(filename);
    }
    else if (extension == "png")
    {
      return load_png(filename);
    }
    else
    {
      throw std::runtime_error("unexpected file extension");
    }
  }
}

