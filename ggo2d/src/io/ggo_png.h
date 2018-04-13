#ifndef __GGO_PNG__
#define __GGO_PNG__

#include <string>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  pixel_buffer  load_png(const std::string & filename);
}

#endif


