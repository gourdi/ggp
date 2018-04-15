#ifndef __GGO_VIVELLE_BLIT__
#define __GGO_VIVELLE_BLIT__

#include <map>
#include <string>

namespace ggo
{
  class pixel_buffer;
  class parameters;

  void blit(ggo::pixel_buffer & image, const parameters & params);
}

#endif