#ifndef __GGO_VIVELLE_CROP__
#define __GGO_VIVELLE_CROP__

#include <2d/ggo_image.h>

namespace ggo
{
  class parameters;

  ggo::image crop(ggo::image & image, const parameters & params);
}

#endif
