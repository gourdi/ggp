#ifndef GGO_VIVELLE_IO__
#define GGO_VIVELLE_IO__

#include <string>
#include <2d/ggo_image.h>

namespace ggo
{
  ggo::image  load(const std::string & filename);
  void        save(const std::string & output_command, const ggo::image & image);
}

#endif

