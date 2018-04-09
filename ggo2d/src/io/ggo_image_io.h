#ifndef __GGO_IMAGE_IO__
#define __GGO_IMAGE_IO__

#include <ggo_pixel_buffer.h>
#include <string>

namespace ggo
{
  ggo::pixel_buffer load_image(const std::string & filename);
}

#endif
