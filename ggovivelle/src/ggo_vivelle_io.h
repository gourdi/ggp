#ifndef GGO_VIVELLE_IO__
#define GGO_VIVELLE_IO__

#include <string>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  ggo::pixel_buffer load(const std::string & filename);
  void              save(const std::string & output_command, const ggo::pixel_buffer & image);
}

#endif

