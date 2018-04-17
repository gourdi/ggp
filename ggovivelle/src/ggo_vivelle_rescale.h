#ifndef __GGO_VIVELLE_RESCALE__
#define __GGO_VIVELLE_RESCALE__

#include <ggo_pixel_buffer.h>

namespace ggo
{
  class parameters;

  ggo::pixel_buffer rescale(ggo::pixel_buffer & image, const parameters & params);
}

#endif
