#ifndef __GGO_VIVELLE_BLUR__
#define __GGO_VIVELLE_BLUR__

#include <2d/ggo_image.h>

namespace ggo
{
  class parameters;

  void blur(ggo::image & image, const parameters & params);
}

#endif
