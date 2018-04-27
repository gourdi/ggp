#ifndef __GGO_IMAGE_IO__
#define __GGO_IMAGE_IO__

#include <2d/ggo_image.h>
#include <string>

namespace ggo
{
  ggo::image load_image(const std::string & filename);
}

#endif