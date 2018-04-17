#ifndef __GGO_VIVELLE_RESCALE__
#define __GGO_VIVELLE_RESCALE__

#include <ggo_image.h>

namespace ggo
{
  class parameters;

  ggo::image rescale(ggo::image & image, const parameters & params);
}

#endif
